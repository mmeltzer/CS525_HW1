#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "storage_mgr.h"
#include "dberror.h"
#include "test_helper.h"

RC readBlock (int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage){
	if(pageNum>=1&&pageNum<=totalNumPages){
		fseek(fHandle->mgmtInfo, (pageNum-1)*PAGE_SIZE , SEEK_SET); 
		//printf("seeked");
		fread(memPage, 1, PAGE_SIZE, fHandle->mgmtInfo);
		//printf("read")
		return RC_OK;
	}
	else{
		return RC_FILE_NOT_FOUND;
	}
}

int getBlockPos (SM_FileHandle *fHandle)
{
	return fHandle->curPagePos; //returns current position of the pointer in the file
}

RC readFirstBlock (SM_FileHandle *fHandle, SM_PageHandle memPage){
	readBlock(1, fHandle, memPage); //returns the first block = 1
	
}

RC readPreviousBlock (SM_FileHandle *fHandle, SM_PageHandle memPage){
	readBlock(fHandle->curPagePos-1, fHandle, memPage); // returns the previous block
}

RC readCurrentBlock (SM_FileHandle *fHandle, SM_PageHandle memPage){
	readBlock(fHandle->curPagePos, fHandle, memPage); //returns the current block
}

RC readNextBlock (SM_FileHandle *fHandle, SM_PageHandle memPage){
	readBlock(fHandle->curPagePos+1, fHandle, memPage); // returns the next block
}

RC readLastBlock (SM_FileHandle *fHandle, SM_PageHandle memPage){
	readBlock(fHandle->totalNumPages, fHandle, memPage); //returns the last block
}

RC writeBlock (int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage){
	/* to check the number of pages lies inbetween the pagenumbers available and then add information */
	if(pageNum>0 && pageNum<=totalNumPages){
		fseek(fHandle->mgmtInfo,(pageNum-1)*PAGE_SIZE, SEEK_SET);
		fwrite(memPage,PAGE_SIZE,1,fHandle->mgmtInfo);
		pageNum = pageNum + 1;
		totalNumPages = totalNumPages + 1;
		return RC_OK;
	}
	/* to create NULL pages and add more information */
	else if(pageNum>totalNumPages){
		ensureCapacity(pageNum-totalNumPages,fHandle);
		fseek(fHandle->mgmtInfo,(pageNum-1)*PAGE_SIZE, SEEK_SET);
		fwrite(memPage,PAGE_SIZE,1,fHandle->mgmtInfo);
		pageNum = pageNum + 1;
		totalNumPages = totalNumPages + 1;
		return RC_OK;
	}
	else{
		return RC_FAILED_WRITE
	}

}

RC writeCurrentBlock (SM_FileHandle *fHandle, SM_PageHandle memPage){
	// to write in the current block
	writeBlock(fHandle->curPagePos, fHandle, memPage); 
}

RC appendEmptyBlock (SM_FileHandle *fHandle){
	//create an empty block with value = NULL ('/0')
	int i=0;
	char emptyString[PAGE_SIZE];
	for(i=0;i<PAGE_SIZE;i++){
		emptyString[i]='\0';
	}
	//find the last page
	fseek(fHandle->mgmtInfo,fHandle->totalNumPages*PAGE_SIZE,SEEK_SET);
	//write the empty block
	fwrite(emptyString,PAGE_SIZE,1,fHandle->mgmtInfo);
	return RC_OK;
	fHandle->curPagePos=lastPage;
	fHandle->totalNumPages = lastPage+1;
}

RC ensureCapacity (int numberOfPages, SM_FileHandle *fHandle){
	/*if there are lesser number of pages => totalNUmPages < numberOfPages
	appendEmptyBlock numofblocksadded = totalNumPages-numberOfPages
	location = curPagePos */
	int i=0;
	if(numberOfPages>totalNumPages){
		for(i=fHandle->totalNumPages;i<numberOfPages;i++){
			appendEmptyBlock(fHandle);
		}
		return RC_OK
		//printf("capacity ensured")
	}
	else{
		return RC_FAILED_WRITE
	}
}
