#include <string.h>
#include <stdio.h>

int main(void){
	char msg[100];
	memset(msg, 0, 100);
	char grade = 'A';
	for(int i = 0; i < (int)sizeof(msg); i++)
	       strncat(msg, &grade, 1);	
	
	for(int i = 0; i < (int)sizeof(msg); i++)
		printf("%s.\n", msg);

	return 0;
}
