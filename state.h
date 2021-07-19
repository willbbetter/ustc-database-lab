#pragma once
#define FRAMESIZE 4096
#define DEFBUFSIZE 5120
#define MAXPAGES 50000

inline unsigned __int64 getCycleCount()
{
	__asm _emit 0x0F
	__asm _emit 0x31
}
struct bFrame
{
	char field[FRAMESIZE];
};