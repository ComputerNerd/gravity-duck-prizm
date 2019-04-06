#include <fxcg/file.h>

#include "save.h"

/* global variables */
static const unsigned short filename[] = {'\\','\\','f','l','s','0','\\','G','r','a','v','i','t','y','D','.','s','a','v',0};

/* public functions */
int save_read(void)
{
	int file, ret=1;
	file = Bfile_OpenFile_OS(filename, 1, 0); // _OPENMODE_READ = 1
	if(file >= 0) {
		Bfile_ReadFile_OS(file, &ret, 4, 0);
		Bfile_CloseFile_OS(file);
	}
	return ret;
}

void save_write(int level)
{
	int file;
	size_t size=4;
	Bfile_DeleteEntry(filename);
	Bfile_CreateEntry_OS(filename, 1, &size);
	file = Bfile_OpenFile_OS(filename, 2, 0); // _OPENMODE_WRITE = 2
	if(file >= 0) {
		Bfile_WriteFile_OS(file, &level, 4);
		Bfile_CloseFile_OS(file);
	}
}
