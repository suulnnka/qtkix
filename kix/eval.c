#include "eval.h"
#include "bit.h"

int16 eval(uint64 my,uint64 opp){
	int value[]={
		40, -8, 4, 3, 3, 4, -8,40,
		-8,-10, 2, 1, 1, 2,-10,-8,
		 4,  2, 3, 1, 1, 3,  2, 4,
		 3,  1, 1, 2, 2, 1,  1, 3,
		 3,  1, 1, 2, 2, 1,  1, 3,
		 4,  2, 3, 1, 1, 3,  2, 4,
		-8,-10, 2, 1, 1, 2,-10,-8,
		40, -8, 4, 3, 3, 4, -8,40};
	int16 r = 0;
	uint8 i=0;
	uint64 m=1ull;
	for(;i<64;i++,m=m<<1){
		if(my&m)
			r+=value[i];
		else if(opp&m)
			r-=value[i];
	}
	return r;
}
