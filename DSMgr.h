#pragma once
#include "state.h"
#include "File.h"
class DSMgr
{
public:
	DSMgr();
	int openFile(char* fileName);
	void closeFile();
	void readPage(bFrame* frm, int pageId);
	void writePage(int frameId, bFrame* frm);
	int getDiskIOCount();

private:
	File* currFile;
	int numPages;
	int pages[MAXPAGES];
	int diskIOCount;
};