#include <ctype.h>
#include <stdio.h>


int main(void){
	char value ='!';
	if(isalpha(value))
		printf("%c is alpha.\n", value);
	else
		printf("%c is not alpha.\n", value);
	return 0;
}
