#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "storage_mgr.h"
#include "dberror.h"
#include "test_helper.h"

RC createPageFile(char *filename){
	FILE *fp;
	fp = fopen(*filename, "w");
	fwrite("\0",1, 1,fp);
	fclose(fp);
	return(0);
}

