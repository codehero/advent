#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

#define DIM 140

char ARR[DIM][DIM];

static const char FORWARDS[3] = "MAS";
static const char BACKWARDS[3] = "SAM";

unsigned search(unsigned column, unsigned row){
	__auto_type d = (uint8_t*)ARR;
	unsigned offset = column + row * DIM;
	char buff[4];
	buff[0] = d[offset - DIM -1];
	buff[1] = d[offset];
	buff[2] = d[offset + DIM + 1];

	unsigned s = 0;
	if(!strncmp(FORWARDS, buff, 3)){
		++s;
	}
	else if(!strncmp(BACKWARDS, buff, 3)){
		++s;
	}

	buff[0] = d[offset - DIM + 1];
	buff[1] = d[offset];
	buff[2] = d[offset + DIM - 1];

	if(!strncmp(FORWARDS, buff, 3)){
		++s;
	}
	else if(!strncmp(BACKWARDS, buff, 3)){
		++s;
	}

	return (2 == s) ? 1 : 0;
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

	for(unsigned r = 1; r <= DIM - 2; ++r){
		for(unsigned c = 1; c <= DIM - 2; ++c){
			sum += search(c, r);
		}
	}

	printf("SUM %u\n", sum);

	return 0;
}
