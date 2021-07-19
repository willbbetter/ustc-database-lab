#include "File.h"
#include "state.h"
#include "DSMgr.h"
#include "BMgr.h"
#include <cstdio>
#include <cstring>
#include <iostream>
using namespace std;
int main()
{
	File file;
	//file.create();
	int pageId, WorR;
	FILE *fp = fopen("data-5w-50w-zipf.txt", "rt");
	BMgr bmgr;
	for (int i = 0; i < 500000 && fp != NULL; ++i) {
		if (fscanf(fp, "%d,%d", &WorR, &pageId)) {
			if (WorR == 0) {
				bmgr.read(pageId);
			}
			else if (WorR == 1) {
				bFrame input;
				strcpy(input.field, "this is a page");
				bmgr.write(pageId, input);
			}
		}
	}
	cout << "缓冲区页框数：" << DEFBUFSIZE << endl;
	cout << "I/O次数：" << bmgr.getDiskIoCount() << endl;
	cout << "命中次数：" << bmgr.getHitCount() << endl;
	cout << "命中率：" << (double)(bmgr.getHitCount()) / 500000 * 100 << "%" << endl;

	return 0;
}
