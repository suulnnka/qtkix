#ifndef ENDSEARCH_H
#define ENDSEARCH_H

#include "base.h"

uint8 end_search(uint8 deep,uint64 my,uint64 opp,int8 *v);
int8 end_pvs(uint8 deep,uint64 my,uint64 opp,int8 alpha,int8 beta);
int8 end_alpha_beta(uint8 deep,uint64 my,uint64 opp,int8 alpha,int8 beta);

#endif
