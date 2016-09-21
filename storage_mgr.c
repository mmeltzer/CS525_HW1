#ifndef STORAGE_MGR_H
#define STORAGE_MGR_H

#include <stdio.h>
#include <stdlib.h>
#include "dberror.h"
#include <string.h>

/* module wide constants */
#define META_SIZE 4096

/************************************************************
 *                    handle data structures                *
 ************************************************************/
typedef struct SM_FileHandle {
  char *fileName;
  int totalNumPages;
  int curPagePos;
  void *mgmtInfo;
} SM_FileHandle;

/* create a new structure to hold information in mgmtInfo*/
typedef struct SM_mgmtInfo {

	FILE *fp;

} SM_mgmtInfo;

typedef char* SM_PageHandle;

/************************************************************
 *                    interface                             *
 ************************************************************/
/* manipulating page files */
void initStorageManager (void) {
	return;
}

	/* 1, create a block in memory, containing the size of meta data and 1 page. In this case
	we only put the information of number of pages into meta data section.

	   2, set the content of this area in memory to be 0.

	   3, set the first 50 bytes to be the area exclusively containing the information of number of pages. The
	number of pages in this case is 1. So we store "1" as a string in this 50 bytes area.

	   4, we write this memory block into harddirve with fwrite(), with target area fp, where fp points to the 
	file that we've created just now.

	   5, at last, we free the memory and the memory we've used and close the FILE pointer.
	*/

RC createPageFile (char *fileName) {
	
	//declare a File object pointer
	FILE *fp;
	
	//create a binary File object
	fp=fopen(fileName, 'ab+'); //when manipulating passing string by pointer, use the name directly

	//malloc memory
	char *multipages=(char *)malloc(META_SIZE+PAGESIZE); // malloc returns void *
	memset(multipages, '\0', META_SIZE+PAGESIZE);
	
	//create a string, give it a value, and copy this string into memory
	char str[50] = {'\0'};
	strcpy(str, "1");
	memcpy(multipages, str, 50);
	
	//fwrite() 
	fwrite(multipages, 1, META_SIZE+PAGESIZE, fp);
	
	//free memory
	free(multipages);
	
	//close FILE pointer
	fclose(fp);
	
	//return code
	return RC_OK;

}


	/* 1, When openning a file, we only need to know the meta data secton from the area that stores the file.

	   2, we open the file first, by fopen().

	   3, then malloc a piece of memory for the meta data stored in harddrive.

	   4, then we read the meta data from hard drive to this piece of memory, with fread().

	   5, because we've stored the number information in string format in harddrive,  we need to create a temperory string 
	to hold the information when reading it back. So, we use memcpy() to copy this information from memory to this string. 
	That is because at this point the informatio has already been read from HardDrive to memory.

	   6, Then, we have to convert this information from str to integer.

	   7, With this information, coping with filename, current page(default 0), we fill up the passed filehandle.

	   8, however, we also need to fill the 'void *mgmtInfo'. To do that, we first create an object with SM_mgmtInfo. Then let
	the item *fp inside this SM_mgmtInfo object point to fp which is the pointer to the file stored in harddrive.

	   9, at last, we let SM_mgmtInfo point to 'void *mgmtInfo' in the filehandle (struct SM_FileHandle).
	*/



/*we assume the object for this method is the result from createPageFile*/
RC openPageFile (char *fileName, SM_FileHandle *fHandle) {
	
	//declare a File object
	File *fp;
	
	//open a File object
	if(fopen(fileName, 'r+')==NULL)
	{
		return RC_FILE_NOT_FOUND;
	}

	fp=fopen(fileName, 'r+');
	
	//malloc memory 
	char *metapage=(char *)malloc(META_SIZE);
	
	//read meta data to the malloced memory
	fread(metapage, 1, META_SIZE, fp)
	
	//create a string, read the information from memory to this string
	char str[50] = {'\0'};
	
	memcpy(str, metapage, 50);

	//convert it into int
	int total = atoi(str);
	
	//fill up the filehandle
	fHandle->fileName=fileName;
	fHandle->totalNumPages=total;
	fHandle->curPagePos=0;

	//fill up the void *mgmtInfo
    SM_mgmtInfo *mgmtInfomation=(SM_mgmtInfo *)malloc(sizeof(SM_mgmtInfo));
    	
    mgmtInfomation->fp=fp;

	fHandle->mgmtInfo=mgmtInfomation;
	
	return RC_OK;

}

/* 
	Simply close the file

*/
RC closePageFile (SM_FileHandle *fHandle) {

	fclose(fHandle->fileName);


	return RC_OK;

}

/* 
	Simply remove the file

*/

RC destroyPageFile (char *fileName) {

	if (remove(fileName)==-1) {
		return RC_FILE_NOT_FOUND;
	}

	return RC_OK;
}

/* reading blocks from disc */
RC readBlock (int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage);
int getBlockPos (SM_FileHandle *fHandle);
RC readFirstBlock (SM_FileHandle *fHandle, SM_PageHandle memPage);
RC readPreviousBlock (SM_FileHandle *fHandle, SM_PageHandle memPage);
RC readCurrentBlock (SM_FileHandle *fHandle, SM_PageHandle memPage);
RC readNextBlock (SM_FileHandle *fHandle, SM_PageHandle memPage);
RC readLastBlock (SM_FileHandle *fHandle, SM_PageHandle memPage);

/* writing blocks to a page file */
RC writeBlock (int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage);
RC writeCurrentBlock (SM_FileHandle *fHandle, SM_PageHandle memPage);
RC appendEmptyBlock (SM_FileHandle *fHandle);
RC ensureCapacity (int numberOfPages, SM_FileHandle *fHandle);

#endif
