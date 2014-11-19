#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

//not-so-magic numbers
#define SECTOR_SIZE 512
#define FAT_START 1
#define ROOT_DIR 19

//types

typedef struct file_str
{
	char name[8];
	char ext[3];
	char attribs;
	char ignored[12];
	uint16_t begin;
	int size;
	char *clusters
}file_str;

//helper function prototypes

file_str *parse_file(void *ptr);

uint16_t decode_fat12_nonsense(void *fat_ptr, uint16_t fat_index);


int main(int argc, char **argv)
{
	struct stat imgstat;
	stat(argv[1], &imgstat);
	int imgsize = imgstat.st_size;
	int image = open(argv[1],O_RDONLY);

	void *data = mmap(NULL, imgsize, PROT_READ, MAP_PRIVATE, image, 0);

	uint16_t decoded_indices[10];

	int i;

	void *fatptr = (char*)(data + FAT_START*SECTOR_SIZE + 3);


	for(i = 0; i < 10; i++)
	{
		decoded_indices[i] = decode_fat12_nonsense(fatptr, i);
		printf("Index %d: %x\n",i,decoded_indices[i]);
	}

	return 0;




}

uint16_t decode_fat12_nonsense(void *fat_ptr, uint16_t fat_index)
{
	uint16_t decoded;
	
	char *toDecode = (char*)(fat_ptr + ((fat_index/2)*3));

	char middlechar = toDecode[1];
	middlechar >>= (4 * (fat_index % 2));
	uint16_t otherhalf = (char)toDecode[(fat_index%2)*2];
	decoded = (uint16_t)middlechar;
	decoded &= 0x000f;
	decoded <<= 8 * ((fat_index % 2)%1);
	otherhalf <<= 8 * (fat_index % 2);
	decoded |= otherhalf;
	return decoded;
}