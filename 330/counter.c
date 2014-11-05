#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

int maskgen(int width);
int extract_lsbytes(char *pc);

int main(int argc, char *argv[]) {
	int correct_predictions;
	int wrong_predictions;

	int max_entries = 1;
	int index_width = atoi(argv[1]);
	max_entries = max_entries << index_width;

//allocate memory for counters, ensure they start at 0	
	char *counters = calloc(sizeof(char)*max_entries);
	char *pc = malloc(9);
	
	int bht_address;
	int taken;

	

	return 0;	
}

//generate a mask of 1's 'width' bits wide

int maskgen(int width) {
	int mask = 0;
	int i;
	for(i = 0; i < width; i++) {
		mask = mask << 1;
		mask = mask | 1;
	}
	return mask;
}

int extract_lsbytes(char *pc) {
	int extracted = strtol(&pc[6],NULL,16);
	return extracted;
}
	
