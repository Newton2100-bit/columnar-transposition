/* vim: set linenumber */
#include <ctype.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

/* data to hold metadata of the keys */
typedef struct key_values{
	char value;
	int position;
}key_values;

typedef struct cipher_house{
	int count;
	char array[4096];
}cipher_house;

void push_character(cipher_house*, char);



/* variables */
bool is_unmap = false;
bool should_we_countinue = false;
bool using_file = false;
char *key = NULL;
char *filename;
char *file_output = NULL;
char *target_data;
int argument;
struct stat file_size;

/* functions */
int cmp(const void*,const void*);
int obtain_plain_text();
int read_from_file();
void create_cipher(key_values *key_data);
int read_from_stdin();
void terminate_the_string(cipher_house *ds);
void finalize();
void parse_arguments(int argc, char *argv[]);
void handle_output(cipher_house *);
void transpose();
void usage(void);

void parse_arguments(int argc, char *argv[]){
	int argument;
	while((argument = getopt(argc, argv, ":k:f:o:")) != -1){
		switch(argument){
			case 'k':
				key = optarg;
				should_we_countinue = true;
				break;
			case 'f':
				filename = optarg;
				using_file = true;
				break;
			case 'o':
				file_output = optarg;
				break;
			default:
				usage();
		}
	}
}

int main(int argc, char *argv[]){
	/* parse our arguments */
	parse_arguments(argc, argv);

	/* if we don't have the key then why continue */
	if(!should_we_countinue){
		printf("Enter the key kindly \n");
		usage();
		return 1;
	}

	if(obtain_plain_text() != 0)
		return 1;

	transpose();
	finalize();
	return 0;
}

void usage(){
	fprintf(stderr, "Usage :\n"
			"%s -k key [-f filename]"
			"\n", __FILE__);
}
int read_from_stdin(){
	target_data = (char *)malloc(sizeof(char) * 4096 );
	fread(target_data , 1, 4096, stdin);
	return 0;
}

int  read_from_file(){
	int fd = open(filename, O_RDONLY);
	if(fd  < 0){
		perror("OPEN:: ");
		exit(1);
	}
	fstat(fd, &file_size);

	target_data = mmap(NULL, file_size.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
	if(target_data == MAP_FAILED){
		perror("MMAP :: ");
		return 1;
	}

	close(fd);
	is_unmap = true;
	return 0;
}

int obtain_plain_text(){
	if(!using_file){
		/* Here things are going on an array of characters
		 * reading everything to a buffer 
		 */
		if(read_from_stdin() != 0)
			return 1;
	}else{
		/* Things here will move away from stack and go to heap
		 * for easier management and simplicity 
		 */
		if(read_from_file() != 0)
			return 1;
	}
	return 0; /* This indicates that we did acquaire data succesfully */
}

void create_cipher(key_values *key_data){
	int TEXT_LEN = strlen(target_data) ;
	int KEY_LEN = strlen(key);
	int iterations =  (TEXT_LEN / KEY_LEN) + ((TEXT_LEN % KEY_LEN)? 1 : 0);

	/* create the cipher now */
	cipher_house cipher = {0};
	for(int i = 0; i < KEY_LEN; i++){
	//	int position = ((key_values*)(key_data[i]))->position;
		int position = key_data[i].position;
		int lcount =  0;

		while(lcount <= iterations){
			if(position > TEXT_LEN){
				push_character(&cipher, '+');
				break;
			}else
				push_character(&cipher,  target_data[position]);

			position += KEY_LEN;
			lcount++;
		}
	}
	handle_output(&cipher);
}

void transpose(){
	/* The struct below holds two things 
	 * the key value and it's position in sequence 
	 * which we later sort so as to implement the alg
	 */
	key_values  key_data[strlen(key)];
	for(int i = 0; i < (int)strlen(key); i++){
		key_data[i].position = i;
		key_data[i].value = key[i];
	}

	/* sort the array we created */
	qsort(&key_data, strlen(key), sizeof(key_values), cmp);
	create_cipher(&key_data[0]);
}

void handle_output(cipher_house *cipher){
	if(file_output == NULL){
		for(int i = 0; i < cipher->count; i++)
			printf("%c", cipher->array[i]);

		printf("\n");
	}else{
		FILE *dst = fopen(file_output, "w+");
		fwrite(cipher->array,strlen(cipher->array),1, dst);
		fprintf(dst, "\n");
		fclose(dst);
		fprintf(stderr, "wrote to file %s successfully\n", file_output);
	}
}

void terminate_the_string(cipher_house *ds){
	/*Note that this is optional since 
	 * we intialized the whole array with nulls 
	 * just being over explicit
	 */
	ds->array[ds->count] = '\0';
}

void push_character(cipher_house *ds, char value){
	ds->array[ds->count] = value;
	ds->count++;
}

#if 0
/* printing the array */
for(int i = 0; i < strlen(key); i++){
	printf("[%d] %c => %d\n", i,key_data[i].value, key_data[i].position);
}
#endif

int cmp(const void *one,const void *two){
	return (((key_values*)one)->value < ((key_values*)(two))->value)? -1 :
		(((key_values*)(one))->value == ((key_values*)(two))->value)? 0 :
		1;
}


void finalize(){
	if(is_unmap)
		munmap(target_data, file_size.st_size);
	else
		free(target_data);
}
