#ifndef MIDSEARCH_H
#define MIDSEARCH_H

#include "base.h"

uint8 mid_search(uint8 deep,uint64 my,uint64 opp,int8 *v);
int16 mid_alpha_beta(uint8 deep,uint64 my,uint64 opp,int16 alpha,int16 beta);
int16 mid_pvs(uint8 deep,uint64 my,uint64 opp,int16 alpha,int16 beta);

#endif
