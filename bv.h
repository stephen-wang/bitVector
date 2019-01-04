/* Copyright (c) 2018, wang wenzhi */

#ifndef _BITVECTOR_H_
#define _BITVECTOR_H_


#include <inttypes.h>


typedef struct BitVector BitVector;

BitVector *bvCreate(uint64_t capacity);
int bvDestroy(BitVector *bv);

uint64_t bvLength(const BitVector *bv);
uint64_t bvCapacity(const BitVector *bv);

int bvSet(BitVector *bv, uint64_t index, uint8_t val);
int bvGet(const BitVector *bv, uint64_t index, uint8_t *pVal);

int bvAppend(BitVector *bv, uint8_t val);
int bvRemove(BitVector *bv, uint64_t index);
int bvClear(BitVector *bv);

#endif
