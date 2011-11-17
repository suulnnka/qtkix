#include "bit.h"

uint8 bit_count(uint64 x){
#if (defined(SSE4_2) && defined(AMD64))

	__asm__("popcntq %1,%0" : "=r"(x) : "r"(x));
	return (uint8)x;

	#else

	#define MASK1_uint8 0x5555555555555555ull
	#define MASK2_uint8 0x3333333333333333ull
	#define MASK4_uint8 0x0F0F0F0F0F0F0F0Full
	x = x - ( ( x >> 1 ) & MASK1_uint8 );
	x = ( x & MASK2_uint8 ) + ( ( x >> 2 ) & MASK2_uint8 ) ;
	x = (x+(x>>4)) & MASK4_uint8;
	x = x * 0x0101010101010101ULL;
	return (uint8)(x>>56);

	#endif
}

uint8 bit_to_x(uint64 x){
	#ifdef AMD64

	__asm__("bsfq %1,%0":"=r"(x):"r"(x));
	return (uint8)x;

	#else

	//使用欧拉回路算法生成的数组以及0x07EF3AE369961512
	const uint8 euler_table[] = {
		63, 0,47, 1,56,48,27, 2,
		60,57,49,41,37,28,16, 3,
		61,54,58,35,52,50,42,21,
		44,38,32,29,23,17,11, 4,
		62,46,55,26,59,40,36,15,
		53,34,51,20,43,31,22,10,
		45,25,39,14,33,19,30, 9,
		24,13,18, 8,12, 7, 6, 5
	};
	return euler_table[ ( x * 0x07EF3AE369961512ull ) >> 58 ];

	#endif
}
uint64 get_some_move(uint64 my,uint64 mask,uint8 dir){
	//TODO 可汇编优化
	uint64 flip;
	flip = (( my << dir ) | ( my >> dir )) & mask;
	flip |= (( flip << dir ) | (flip >> dir )) & mask;
	flip |= (( flip << dir ) | (flip >> dir )) & mask;
	flip |= (( flip << dir ) | (flip >> dir )) & mask;
	flip |= (( flip << dir ) | (flip >> dir )) & mask;
	flip |= (( flip << dir ) | (flip >> dir )) & mask;
	return ( flip << dir ) | ( flip >> dir );
}
uint64 get_move(uint64 my,uint64 opp){
	return (get_some_move(my , opp & 0x007E7E7E7E7E7E00ull , 9 )
		| get_some_move(my , opp & 0x007E7E7E7E7E7E00ull , 7 )
		| get_some_move(my , opp & 0x7E7E7E7E7E7E7E7Eull , 1 )
		| get_some_move(my , opp & 0x00FFFFFFFFFFFF00ull , 8 ))
		& ( ~( my | opp) ) ;
}
