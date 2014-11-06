#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

typedef struct table_entry {
	char bhsr;
	char pht[16];
}table_entry;


int maskgen(int width);
int generate_address(char *pc, int mask);

int main(int argc, char *argv[]) {
	int predictions = 0;
	int mispredictions = 0;
	int index_width = atoi(argv[1]);
	int max_entries = 1;
	max_entries <<= index_width;
//since bhsr is only 4-bit, we'll mask out the remaining bits.
	int bhsr_mask = maskgen(4);

//allocate memory for table entries, ensure they start at 0	
	table_entry *table = calloc(max_entries, sizeof(table_entry));
	char *pc = malloc(9);
	char *current_counter;
	int bht_address;
	int taken;
	int pc_mask = maskgen(index_width);
	char *bhsr;
//process file, update total number of predictions read and mispredictions.

	while(!feof(stdin)) {
		scanf("%s %d",pc,&taken);
		predictions++;

//		printf("%d: scanned %s, %d\n",predictions,pc,taken);

		bht_address = generate_address(pc, pc_mask);

		current_counter = &(table[bht_address]).pht[(table->bhsr)];

//		printf("%d: scanned %s, %d, counter: %d\n",predictions,pc,taken, *current_counter);
		bhsr = &table->bhsr;
		*bhsr <<= 1;
		*bhsr &= bhsr_mask;
//		printf("BHSR: %d\n",*bhsr);
		if(taken) {
//			printf("Taken loop entered\n");
			if(*current_counter < 2) { mispredictions++;	}

			if(*current_counter < 3) { (*current_counter)++;}
			(*bhsr)++;
//			printf("New bhsr value: %d\n",*bhsr);
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
	
