#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

int maskgen(int width);
int generate_address(char *pc, int mask);

int main(int argc, char *argv[]) {
	int predictions = 0;
	int mispredictions = 0;

	int max_entries = 1;
	int index_width = atoi(argv[1]);
	max_entries = max_entries << index_width;

//allocate memory for counters, ensure they start at 0	
	char *counters = calloc(max_entries,sizeof(char));
	char *pc = malloc(9);
	char *current_counter;
	int bht_address;
	int taken;
	int mask = maskgen(index_width);

//process file, update total number of predictions read and mispredictions.

	while(!feof(stdin)) {
		scanf("%s %d",pc,&taken);
		predictions++;

//		printf("%d: scanned %s, %d\n",predictions,pc,taken);

		bht_address = generate_address(pc, mask);
		current_counter = &counters[bht_address];

//		printf("%d: scanned %s, %d, counter: %d\n",predictions,pc,taken, *current_counter);
		if(taken) {
			if(*current_counter < 2) { mispredictions++;	}

			if(*current_counter < 3) { (*current_counter)++;}
		}
		else {
			if(*current_counter > 1) { mispredictions++; }

			if(*current_counter > 0) { (*current_counter)--; }
		}				
	}

//file processing complete, let's crunch some numbers!

	float percent_mispredict = (float)mispredictions/predictions;
	percent_mispredict *= 100;
	printf("Percent mispredicted: %0.2f\n",percent_mispredict);

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

int generate_address(char *pc, int mask) {
	int address = strtol(&pc[4],NULL,16);
	address = address & mask;
	return address;
}
	
