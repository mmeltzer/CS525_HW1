#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "storage_mgr.h"
#include "dberror.h"
#include "test_helper.h"


RC readBlock (int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage)
{
	//Checks for initialization of FileHandle
	if(fHandle == NULL)
	{
		return RC_FILE_HANDLE_NOT_INIT;
	}

	//Checks to see if the file has less than pageNum pages
	if(fHandle->totalNumPages < pageNum)
	{
		return RC_READ_NON_EXISTING_PAGE;
	}

	FILE *fp;
	fp = fHandle->fileName;

	//Sets the position of the stream to pageNumth page. Since all pages are of size PAGE_SIZE,
	//the byte count that that the stream should start at is equal to the product of
	//pageNum-1 and PAGE_SIZE
	fseek(fp, pageNum*PAGE_SIZE , SEEK_SET);

	//reads from the pageNumth block to memPage
	fread(memPage, 1, PAGE_SIZE, fp);

	return RC_OK;
}

int getBlockPos (SM_FileHandle *fHandle)
{
	//returns current position of the pointer in the file
	return fHandle->curPagePos;
}

RC readFirstBlock (SM_FileHandle *fHandle, SM_PageHandle memPage)
{
	//Checks for initialization of FileHandle
	if(fHandle == NULL)
	{
		return RC_FILE_HANDLE_NOT_INIT;
	}

	FILE *fp;
	fp = fHandle->fileName;
	//Sets position of stream to first page
	fseek(fp, 0, SEEK_SET);

	//reads the first block
	fread(memPage, 1, PAGE_SIZE, fp);

	return RC_OK;
}
RC readLastBlock (SM_FileHandle *fHandle, SM_PageHandle memPage)
{
	//Checks for initialization of FileHandle
	if(fHandle == NULL)
	{
		return RC_FILE_HANDLE_NOT_INIT;
	}

	FILE *fp;
	fp= fHandle->fileName;

	//Sets position of stream to last page
	fseek(fp, -PAGE_SIZE, SEEK_END);

	//reads the last block
	fread(memPage, 1, PAGE_SIZE, fp);

	return RC_OK;
}

RC readCurrentBlock (SM_FileHandle *fHandle, SM_PageHandle memPage)
{
	//Checks for initialization of FileHandle
	if(fHandle == NULL)
	{
		return RC_FILE_HANDLE_NOT_INIT;
	}

	if(fHandle->curPagePos > fHandle->totalNumPages)
	{
		return RC_READ_NON_EXISTING_PAGE;
	}

	int cp = fHandle->curPagePos;

	FILE *fp;
	fp = fHandle->fileName;

	//reads block
	fread(memPage, 1, PAGE_SIZE, fp);

	//Sets the pointer to page after the one that was just read
	fHandle->curPagePos = cp++;

	return RC_OK;
}

RC readPreviousBlock (SM_FileHandle *fHandle, SM_PageHandle memPage)
{
	if(fHandle->curPagePos == 0)
	{
		return RC_READ_NON_EXISTING_PAGE;
	}

	int page = fHandle->curPagePos--;

	readBlock(page, *fHandle, memPage);
}
