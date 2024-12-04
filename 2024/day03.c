#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>

enum {
	 ST_M
	,ST_U
	,ST_L
	,ST_LP
	,ST_NUM

	,ST_O
	,ST_N
	,ST_AP
	,ST_T
	,ST_DLP
	,ST_DRP
};

unsigned sum = 0;
unsigned st = ST_M;
unsigned numlen = 0;

char dt = 'o';

char numbuff[16];

int domul(unsigned numlen, const char* str){
	if('o' != dt)
		return 0;

	unsigned cidx = numlen;
	for(unsigned i = 0; i < numlen; ++i){
		if(',' == str[i]){
			cidx = i + 1;
			break;
		}
	}

	if(cidx >= numlen)
		return -1;

	int num1 = -1;
	int num2 = -1;
	sscanf(str, "%u", &num1);
	if(4 == cidx && num1 < 100)
		return -2;
	if(3 == cidx && num1 < 10)
		return -3;
	if(2 == cidx && num1 < 0)
		return -3;
	if(1 == cidx)
		return -4;

	sscanf(str + cidx, "%u", &num2);
	unsigned diff = numlen - cidx;

	if(3 == diff && num2 < 100)
		return -5;
	if(2 == diff && num2 < 10)
		return -6;
	if(1 == diff && num2 < 0)
		return -7;
	if(0 == diff)
		return -8;

	return num1 * num2;
}

void process(char c){
	switch(st){
		case ST_M:
			numlen = 0;
			if('m' == c){
				st = ST_U;
			}
			else if('d' == c){
				st = ST_O;
			}
			break;

		case ST_U:
			if('u' == c){
				st = ST_L;
			}
			else{
				st = ST_M;
				process(c);
			}
			break;


		case ST_L:
			if('l' == c){
				st = ST_LP;
			}
			else{
				st = ST_M;
				process(c);
			}
			break;

		case ST_LP:
			if('(' == c){
				st = ST_NUM;
			}
			else{
				st = ST_M;
				process(c);
			}
			break;

		case ST_NUM:
			if('m' == c){
				st = ST_U;
				numlen = 0;
				break;
			}
			else if('d' == c){
				st = ST_O;
				break;
			}


			if(')' == c){
				numbuff[numlen] = '\0';
				int s = domul(numlen, numbuff);
				if(s > 0){
					sum += s;
				}
				else{
					fprintf(stderr, "ERR %s %i\n", numbuff ,s);
				}
				st = ST_M;
				numlen = 0;
			}
			if(7 == numlen){
				st = ST_M;
				numlen = 0;
			}
			else{
				numbuff[numlen] = c;
				++numlen;
			}
			break;

		case ST_O:
			if('o' == c){
				st = ST_N;
			}
			else{
				st = ST_M;
				process(c);
			}
			break;

		case ST_N:
			if('n' == c){
				dt = 'n';
				st = ST_AP;
			}
			else if('(' == c){
				dt = 'o';
				st = ST_DRP;
			}
			else{
				st = ST_M;
				process(c);
			}
			break;

		case ST_AP:
			if('\'' == c){
				st = ST_T;
			}
			else{
				st = ST_M;
				process(c);
			}
			break;

		case ST_T:
			if('t' == c){
				st = ST_DLP;
			}
			else{
				st = ST_M;
				process(c);
			}
			break;

		case ST_DLP:
			if('(' == c){
				st = ST_DRP;
			}
			else{
				st = ST_M;
				process(c);
			}
			break;

		case ST_DRP:
			if(')' == c){
				st = ST_M;
			}
			else{
				st = ST_M;
				process(c);
			}
			break;

		default:
			break;
	}

}

int main(int argc, const char* argv[]){
	while(true){
		char buffer[1024];
		int len = read(0, buffer, sizeof(buffer));
		if(len <= 0)
			break;
		for(unsigned i = 0; i < len; ++i){
			char c = buffer[i];
			process(c);
		}
	}

	printf("SUM %u\n", sum);

	return 0;
}
