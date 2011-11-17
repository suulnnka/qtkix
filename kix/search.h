#ifndef SEARCH_H
#define SEARCH_H

#include "base.h"

uint64 node;
uint8 pass[64];

int search(uint8 deep,uint64 my,uint64 opp,int8 *v,uint64 *n);

#endif
