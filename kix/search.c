#include "base.h"
#include "midsearch.h"
#include "endsearch.h"
#include "search.h"
#include "bit.h"
#include "flip.h"
#include "eval.h"
#include <string.h>

int search(uint8 deep,uint64 my,uint64 opp,int8 *v,uint64 *n){

	int m ;

	memset(pass,0,sizeof(pass));

	node = 0ull;
	int space = bit_count( ~(my|opp) );
	if(space <= deep )
		m = end_search(space,my,opp,v);
	else
		m = mid_search(deep,my,opp,v);

	(*n) = node;

	return m;
}
