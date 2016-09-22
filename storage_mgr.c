#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "storage_mgr.h"
#include "dberror.h"
#include "test_helper.h"


RC readBlock (int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage)
{
	//Checks to see if the file has less than pageNum pages
	if(fHandle->totalNumPages < pageNum)
	{
		return RC_READ_NON_EXISTING_PAGE;
	}

	//Sets the position of the stream to pageNumth page. Since all pages are of size PAGE_SIZE,
	//the byte count that that the stream should start at is equal to the product of
	//pageNum-1 and PAGE_SIZE
	fseek(fHandle->mgmtInfo, (pageNum-1)*PAGE_SIZE , SEEK_SET);

	//reads from the pageNumth block to memPage
	fread(memPage, 1, PAGE_SIZE, fHandle->mgmtInfo);

	return RC_OK;
}

int getBlockPos (SM_FileHandle *fHandle)
{
	//returns current position of the pointer in the file
	return fHandle->curPagePos;
}

extern RC readFirstBlock (SM_FileHandle *fHandle, SM_PageHandle memPage)
{
	//Sets position of stream to first page
	fseek(fHandle->mgmtInfo, 0, SEEK_SET);

	//reads the first block
	fread(memPage, 1, PAGE_SIZE, fHandle->mgmtInfo);

	return RC_OK;
}

extern RC readLastBlock (SM_FileHandle *fHandle, SM_PageHandle memPage)
{
	//Sets position of stream to last page
	fseek(fHandle->mgmtInfo, -PAGE_SIZE, SEEK_END);

	//reads the last block
	fread(memPage, 1, PAGE_SIZE, fHandle->mgmtInfo);

	return RC_OK;
}
