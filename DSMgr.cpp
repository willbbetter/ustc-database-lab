#include "DSMgr.h"
DSMgr::DSMgr()
{
	char* path = "data.dbf";
	openFile(path);
	diskIOCount = 0;
}

int DSMgr::openFile(char* fileName)
{
	currFile = new File(fileName, pages);
	numPages = currFile->pageCount;
	if (currFile) return 1;
	else return 0;
}
void DSMgr::closeFile()
{
	currFile->pageCount = numPages;
	currFile->dirSize = numPages * sizeof(int);
	if ((sizeof(int)*3 + sizeof(int)*numPages) % currFile->pageSize > 0)
		currFile->posDataStart = (sizeof(int)*3+sizeof(int)*currFile->pageSize) / FRAMESIZE + 1;
	else
		currFile->posDataStart = (sizeof(int)*3+sizeof(int)*currFile->pageSize) / FRAMESIZE;
	currFile->close(pages);
}
void DSMgr::readPage(bFrame* frame, int pageId)
{
	int offset;
	diskIOCount++;
	offset = pages[pageId - 1];
	fseek(currFile->file, currFile->posDataStart + offset, 0);
	if (fread(frame, sizeof(bFrame), 1, currFile->file)) {
		return;
	}
}
void DSMgr::writePage(int pageId, bFrame* frm)
{
	int offset;
	diskIOCount++;
	offset = pages[pageId - 1];
	fseek(currFile->file, currFile->posDataStart + offset, 0);
	if (fwrite(frm, sizeof(bFrame), 1, currFile->file)) {
		return;
	}
}
int DSMgr::getDiskIOCount() {
	return diskIOCount;
}