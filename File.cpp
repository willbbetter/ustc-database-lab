#include "File.h"

void File::create()
{
	int temp;
	FILE* file_t = fopen("data.dbf", "wb+");
	temp = 4 * 50000;
	fwrite(&temp, sizeof(int), 1, file_t);
	temp = 4096;
	fwrite(&temp, sizeof(int), 1, file_t);
	if ((4 * 3 + 4 * 50000) % 4096 > 0)
		temp = (4 * 3 + 4 * 50000) / 4096 + 1;
	else
		temp = (4 * 3 + 4 * 50000) / 4096;
	fwrite(&temp, sizeof(int), 1, file_t);
	int dir_t[50000];
	for (int i = 0; i < 50000; ++i) {
		dir_t[i] = i;
	}
	fwrite(dir_t, sizeof(int), 50000, file_t);
	fseek(file_t, temp * 4096, 0);
	char chrs[4096];
	for (int i = 0; i < 50000; i++)
	{
		chrs[0] = i;
		fwrite(chrs, sizeof(chrs), 1, file_t);
	}
	fclose(file_t);
}

File::File(const char* fileName, int* dir)
{
	file = fopen(fileName, "rb+");
	fread(&dirSize, sizeof(int), 1, file);
	fread(&pageSize, sizeof(int), 1, file);
	fread(&posDataStart, sizeof(int), 1, file);
	fread(dir, sizeof(int), dirSize / sizeof(int), file);
	pageCount = dirSize / sizeof(int);
}

void File::close(int* dir)
{
	fwrite(&dirSize, sizeof(int), 1, file);
	fwrite(&pageSize, sizeof(int), 1, file);
	fwrite(&posDataStart, sizeof(int), 1, file);
	fwrite(dir, sizeof(int), dirSize / sizeof(int), file);
	fclose(file);
	return ;
}