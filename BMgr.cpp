#include "BMgr.h"
#include <cstring>
BMgr::BMgr()
{
	numFreeFrames = DEFBUFSIZE;
	hitCount = 0;
	for (int i = 0; i < DEFBUFSIZE; ++i) {
		ftop[i] = -1;
		ptof[i] = NULL;
	}
}
int BMgr::getHitCount()
{
	return hitCount;
}
int BMgr::getDiskIoCount()
{
	return dsmgr.getDiskIOCount();
}
bFrame& BMgr::read(int pageId)
{
	return buf[fixPage(pageId, 0)];
}
void BMgr::write(int pageId, bFrame& input)
{
	strcpy(buf[fixPage(pageId, 1)].field, input.field);
}

BCB* BMgr::hash(int pageId)
{
	BCB* p = NULL;
	p = ptof[pageId % DEFBUFSIZE];
	while (p) {
		if (p->pageId == pageId)
			return p;
		p = p->next;
	}
	return NULL;
}

BCB * BMgr::selectVictim()
{
	unsigned __int64 min = getCycleCount();

	BCB* minP = ptof[0];
	for (int i = 0; i < DEFBUFSIZE; ++i) {
		BCB* p = ptof[i];
		while (p != NULL) {
			if (p->usedTime < min) {
				minP = p;
				min = p->usedTime;
			}
			p = p->next;
		}
	}
	if (minP->dirty) {
		dsmgr.writePage(minP->pageId, &buf[minP->frameId]);
	}
	removeBCB(minP);
	ftop[minP->frameId] = -1;
	return minP;
}
void BMgr::removeBCB(BCB* ptr)
{
	BCB* p = NULL;
	p = ptof[ptr->pageId % DEFBUFSIZE];
	if (ptr == p) {
		ptof[ptr->pageId % DEFBUFSIZE] = ptr->next;
	}
	else {
		while (p->next != ptr)
			p = p->next;
		p->next = ptr->next;
	}
}


int BMgr::fixPage(int pageId, int RorW)
{
	BCB* p = hash(pageId);
	if (p) {
		hitCount++;
		if (RorW == 1)
			p->dirty = RorW;
		p->usedTime = getCycleCount();
		return p->frameId;
	}
	else {
		if (numFreeFrames > 0) {
			numFreeFrames--;
			for (int i = 0; i < DEFBUFSIZE; ++i) {
				if (ftop[i] == -1) {
					p = new BCB();
					p->count = 1;
					p->dirty = RorW;
					p->frameId = i;
					p->latch = 0;
					p->next = NULL;
					p->pageId = pageId;
					p->usedTime = getCycleCount();

					ftop[i] = pageId;
					p->next = ptof[pageId % DEFBUFSIZE];
					ptof[pageId % DEFBUFSIZE] = p;
					if (RorW == 0)
						dsmgr.readPage(&buf[i], pageId);

					return i;
				}
			}
		}
		else {
			p = selectVictim();
			ftop[p->frameId] = pageId;
			p->count = 1;
			p->dirty = RorW;
			p->latch = 0;
			p->next = NULL;
			p->pageId = pageId;
			p->usedTime = getCycleCount();
			p->next = ptof[pageId % DEFBUFSIZE];
			ptof[pageId % DEFBUFSIZE] = p;
			if (RorW == 0)
				dsmgr.readPage(&buf[p->frameId], pageId);

			return p->frameId;
		}
	}
}
void BMgr::writeDirtys()
{
	BCB* p = NULL;
	for (int i = 0; i < DEFBUFSIZE; ++i) {
		p = ptof[i];
		while (p) {
			if (p->dirty) {
				dsmgr.writePage(p->pageId, &buf[p->frameId]);
			}
			p = p->next;
		}
	}
	dsmgr.closeFile();
}