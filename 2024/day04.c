#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

#define DIM 140

char ARR[DIM][DIM];

static const char FORWARDS[4] = "XMAS";
static const char BACKWARDS[4] = "SAMX";

unsigned search(unsigned column, unsigned row, int stride){
	__auto_type d = (uint8_t*)ARR;
	unsigned offset = column + row * DIM;
	char buff[4];
	buff[0] = d[offset];
	buff[1] = d[offset + stride];
	buff[2] = d[offset + 2 * stride];
	buff[3] = d[offset + 3 * stride];

	unsigned s = 0;
	if(!strncmp(FORWARDS, buff, 4)){
		fprintf(stderr, "SF %u %u %u\n", stride, row, column);
		return 1;
	}
	if(!strncmp(BACKWARDS, buff, 4)){
		fprintf(stderr, "SB %u %u %u\n", stride, row, column);
		return 1;
	}
	return 0;
}

int main(int argc, const char* argv[]){
	unsigned sum = 0;
	unsigned offset = 0;
	__auto_type d = (uint8_t*)ARR;
	unsigned crow = 0;
	while(true){
		char buffer[DIM + 1];
		int len = read(0, buffer, sizeof(buffer));
		if(len <= 0)
			break;
		memcpy(ARR[crow], buffer, DIM);
		++crow;
	}

	for(unsigned r = 0; r < DIM; ++r){
		__auto_type row = ARR[r];
		for(unsigned c = 0; c <= DIM - 4; ++c){
			if(!strncmp(FORWARDS, row + c, 4))
				++sum;
			if(!strncmp(BACKWARDS, row + c, 4))
				++sum;
		}
	}

	for(unsigned r = 0; r <= DIM - 4; ++r){
		for(unsigned c = 0; c < DIM; ++c){
			if(c >= 3)
				sum += search(c, r, DIM - 1);

			if(c <= DIM - 4)
				sum += search(c, r, DIM + 1);

			sum += search(c, r, DIM);
		}
	}

	printf("SUM %u\n", sum);

	return 0;
}
