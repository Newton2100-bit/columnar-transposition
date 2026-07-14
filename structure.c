#include <stdio.h>

int main()
{
	struct value{
		int value;
	}value;
	value.value = 90;
	struct value *ptr = &value;

	printf("The value is %d.\n", ptr->value);
}
