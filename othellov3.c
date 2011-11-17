#include "ai_data.h"
#include "kix/search.h"

int AI(int deep,int *bb,int c,Ai_Data *data){
	unsigned long long my = 0ull;
	unsigned long long opp = 0ull;
	int x,y;
	unsigned long long i = 1ull;
	for( x = 1 ; x <= 8 ; x ++ )
	for( y = 1 ; y <= 8 ; y ++ ){
		if( bb[x*10+y] == c )
			my |= i ;
		if( bb[x*10+y] == -c )
			opp |= i ;
		i = i << 1;
	}
	char v;
	unsigned long long n;
	int m = search(deep,my,opp,&v,&n);

	data->value = v;
	data->node = n;

	x = m/8 + 1;
	y = m%8 + 1;

	return x*10+y;

}
