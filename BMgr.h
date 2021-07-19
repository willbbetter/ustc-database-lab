#pragma once
#include "state.h"
#include "DSMgr.h"

struct BCB {
	int pageId;
	int frameId;
	int latch;
	int count;
	unsigned __int64 usedTime;
	int dirty;
	BCB* next;
};

class BMgr
{
public:
	BMgr();
	// Interface functions
	int getHitCount();
	int getDiskIoCount();
	bFrame& read(int pageId);
	void removeBCB(BCB* ptr);
	void write(int pageId, bFrame& input);
	BCB *selectVictim();
	int fixPage(int pageId, int prot);
	BCB* hash(int page_id);
	void writeDirtys();
private:
	//Hash Table
	DSMgr dsmgr;
	int numFreeFrames;
	int ftop[DEFBUFSIZE];
	BCB* ptof[DEFBUFSIZE];
	bFrame buf[DEFBUFSIZE];
	int hitCount;
};