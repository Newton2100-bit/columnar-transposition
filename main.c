/* vim: set linenumber */
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>

/* data to hold metadata of the keys */
typedef struct key_values{
	char value;
	int position;
}key_values;


typedef struct inc{
	int count;
	char array[4096];
}inc;

void push(inc*, char);



/* variables */
char *key = NULL;
char *filename;
struct stat file_size;
char *target_data;
int argument;
bool should_we_countinue = false;
bool using_file = false;
bool is_unmap = false;

/* functions */
void close_it(inc *ds);
void print_result(inc *);
int obtain_plain_text();
void finalize();
void transpose();
int cmp(const void*,const void*);

int main(int argc, char *argv[]){

	int argument;
	while((argument = getopt(argc, argv, ":k:f:")) != -1){
		switch(argument){
			case 'k':
				key = optarg;
				should_we_countinue = true;
				break;
			case 'f':
				filename = optarg;
				using_file = true;
				break;
			default:
				fprintf(stderr, "Usage :\n"
						"%s -k key [-f filename]"
						"\n", __FILE__);
		}

	}

	/* if we don't have the key then why continue */
	if(!should_we_countinue) return 1;

	obtain_plain_text();
	transpose();
	finalize();
	return 0;
}

int obtain_plain_text(){
	// printf("we abtained the key :: %s.\n", key);
	if(!using_file){
		/* Here things are going on an array of characters
		 * reading everything to a buffer 
		 */
		target_data = (char *)malloc(sizeof(char) * 4096 );
		fread(target_data , 1, 4096, stdin);
	}else{
		/* Things here will move away from stack and go to heap
		 * for easier management and simplicity 
		 */
		int fd = open(filename, O_RDONLY);
		fstat(fd, &file_size);

		target_data = mmap(NULL, file_size.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
		if(target_data == MAP_FAILED){
			perror("MMAP :: ");
			return 1;
		}

		close(fd);
		is_unmap = true;
	}
	return 0;
}

void transpose(){
	key_values  key_data[strlen(key)];
	/* constructing the array from the key */
	for(int i = 0; i < strlen(key); i++){
		key_data[i].position = i;
		key_data[i].value = key[i];
	}
	/* sort the array we created */
	qsort(&key_data, strlen(key), sizeof(key_values), cmp);

	int TEXT_LEN = strlen(target_data) - 1;
	int KEY_LEN = strlen(key);
	printf("TEXT LEN : %d\n", TEXT_LEN);
	printf("KEY LEN : %d\n", KEY_LEN);
	int temp =  TEXT_LEN / KEY_LEN;


	int add_ons = (TEXT_LEN % KEY_LEN)? 1 : 0;
	int iterations =  temp + add_ons;

	/* create the cipher now */
	int count = 0;
	inc result = {0};
	for(int i = 0; i < KEY_LEN; i++){

		int position = key_data[i].position;
		int lcount =  0;

		while(lcount <= iterations){
			if(position > TEXT_LEN){
				push(&result, '+');
				// printf("%c ", '+');
				break;
			}else{
				push(&result,  target_data[position]);
				//	printf("%c ", target_data[position]);
			}
			count++;
			position += KEY_LEN;
			lcount++;
		}
	}
	close_it(&result);
	print_result(&result);

}

void print_result(inc *result){
	for(int i = 0; i < result->count; i++)
		printf("%c", result->array[i]);
}

void close_it(inc *ds){
	ds->array[ds->count] = '\0';
}

void push(inc *ds, char value){
	ds->array[ds->count] = value;
	ds->count++;
}

#if 0
/* printing the array */
* for(int i = 0; i < strlen(key); i++){
	printf("[%d] %c => %d\n", i,key_data[i].value, key_data[i].position);
}
#endif

int cmp(const void *one,const void *two){
	return (((key_values*)one)->value < ((key_values*)(two))->value)? -1 : (((key_values*)(one))->value == ((key_values*)(two))->value)? 0 : 1;
}


void finalize(){
	if(is_unmap)
		munmap(target_data, file_size.st_size);
	else
		free(target_data);
}
