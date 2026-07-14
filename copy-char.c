#include <string.h>
#include <stdio.h>

int main(void){
	char *str = "newton";
	strcpy(str, "I");
	printf("FINAL:: %s\n", str);
	return 0;
}
