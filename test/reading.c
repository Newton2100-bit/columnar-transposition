#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){
	char file[4096];
	memset(file, 'A', 4096);
	FILE *ptr = fopen("wow.txt", "w+");

	fwrite(file, sizeof(char), 4095, ptr);
	memset(file, 'B', 4096);
	fwrite(file, sizeof(char), 4095, ptr);

	fclose(ptr);
	return 0;
}
