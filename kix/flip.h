#ifndef FLIP_H
#define FLIP_H

#include "base.h"
uint64 flip_slow(uint64,uint64,uint64);
uint64 flip_dir(uint64 my,uint64 opp,uint64 m,uint8 dir);
unsigned long long (*flip[66])(const unsigned long long, const unsigned long long);

#endif
