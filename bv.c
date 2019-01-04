/* Copyright (c) 2018, wang wenzhi */

/* The implementation of thread-safe (TODO) bit vector, which support opeartions
 * like: bvCreate, bvDestroy, bvAppend, bvSet, bvGet, bvRemove, bvClear
 *
 * For benchmark data, please check bvBench.c
 */


#include <assert.h>
#include <stdio.h>         /* printf */
#include <stdlib.h>
#include "bv.h"


// Get the index of target bit within byte from bv index
#define BITS_PER_BYTE           8
#define BV_BIT_INDEX(index)     (index % BITS_PER_BYTE)

// Get the index of byte containing target bit from bv index
#define BV_BYTE_INDEX(index)    (index / BITS_PER_BYTE)

// Set/clear bit
#define BV_BIT_MASK(bit)        (uint8_t)(0x01<<bit)
#define BV_SET_BIT(byte, bit)   byte |= BV_BIT_MASK(bit)
#define BV_CLEAR_BIT(byte, bit) byte &= ~BV_BIT_MASK(bit)

// Get the value of sepecific bit
#define BV_BIT_VAL(byte, bit)   (uint8_t)((byte >> bit) & 0x01)


struct BitVector {
    uint64_t capacity;
    uint64_t length;
    uint64_t bufSize;
    uint8_t buf[0];
};


#ifdef DEBUG
#define Debug(fmt, ...)  printf("Debug " fmt, __VA_ARGS__)
#else
#define Debug(fmt, ...)
#endif


/************************************************************
 * -- bvCreate
 *
 *     Create a BitVector object 
 *
 * Return:
 *     A pointer to BitVector object
 *
 * Side effects:
 *     It's caller's duty to release the returned oject
 *  
 ************************************************************/

BitVector *bvCreate(uint64_t capacity) // IN
{
    assert(capacity > 0);

    uint64_t bufSize = BV_BYTE_INDEX(capacity-1) + 1;
    BitVector *bv = malloc(sizeof(BitVector) + sizeof(uint8_t) * bufSize);
    if (bv== NULL) {
        return NULL;
    }

    bv->length = 0; 
    bv->capacity = capacity;
    bv->bufSize = BV_BYTE_INDEX(bv->capacity-1) + 1;

    return bv;
}


/************************************************************
 * -- bvDestroy
 *
 *     Destroy BitVector object and release its memory.
 *
 * Return:
 *     0 on success, -1 on failure
 *
 ************************************************************/

int bvDestroy(BitVector *bv)  // IN/OUT
{
    if (bv == NULL) {
        Debug("%s: invalid parameter\n", __FUNCTION__);
        return 0;
    }

    // Make sure bv is not in use.
    if (bv->length > 0) { 
        Debug("%s: non-empty BitVector\n", __FUNCTION__);
        return -1;
    }

    free(bv);
    return 0;
}


/*******************************************************************
 * -- bvLength
 *
 *     Get the number of bits stored in the BitVector
 *
 * Return:
 *     The length of the BitVector
 *
 *******************************************************************/

uint64_t bvLength(const BitVector *bv) // IN
{
    assert(bv != NULL);
    return bv->length;
}


/*******************************************************************
 * -- bvCapacity
 *
 *     Get the number of bits the BitVector can host at most
 *
 * Return:
 *     The capacity of the BitVector
 *
 *******************************************************************/

uint64_t bvCapacity(const BitVector *bv) // IN
{
    assert(bv != NULL);
    return bv->capacity;
}


/************************************************************
 * -- bvSet
 *
 *     Set the 'index'th bit of BitVector to 1 or 0
 *
 * Return:
 *     0 on success, -1 on failure
 *
 ************************************************************/

int bvSet(BitVector *bv,  // IN/OUT
          uint64_t index, // IN
          uint8_t val)    // IN
{
    if (bv == NULL || index >= bv->length) {
        Debug("%s: invalid paramter\n", __FUNCTION__);
        return -1;
    }

    uint8_t bitIndex = BV_BIT_INDEX(index);
    uint64_t byteIndex = BV_BYTE_INDEX(index);

    assert(bitIndex < BITS_PER_BYTE && byteIndex < bv->bufSize);
    //Debug("%s: index %lu, value %u\n", __FUNCTION__, index, val);
    if (val == 1) {
        BV_SET_BIT(bv->buf[byteIndex], bitIndex);
    } else {
        BV_CLEAR_BIT(bv->buf[byteIndex], bitIndex);
    }

    /*
    uint8_t newVal; 
    bvGet(bv, index, &newVal);
    Debug("%s: index %lu, value %u, real value: %u\n", __FUNCTION__, index, val, newVal);
    */

    return 0;
}


/*******************************************************************
 * -- bvGet
 *
 *     Get the value of target bit
 *
 * Return:
 *     0 on success, -1 on failure
 *
 *******************************************************************/

uint8_t bvGet(const BitVector *bv, // IN
              uint64_t index,      // IN
              uint8_t *pVal)       // OUT
{
    if (bv==NULL || pVal == NULL || index >= bv->length) {
        Debug("%s: invalid parameter\n", __FUNCTION__);
        return -1;
    }

    uint8_t bitIndex = BV_BIT_INDEX(index);
    uint64_t byteIndex = BV_BYTE_INDEX(index);

    assert(bitIndex < BITS_PER_BYTE && byteIndex < bv->bufSize);
    *pVal = BV_BIT_VAL(bv->buf[byteIndex], bitIndex);
    return 0;
}


/*******************************************************************
 * -- bvAppend
 *  
 *    Append a data (1 or 0) to the end of the bit vector.
 *
 * Return:
 *     0 on success, -1 on failure
 *
 *******************************************************************/

int bvAppend(BitVector *bv, // IN/OUT
             uint8_t val)   // IN
{
    if (bv == NULL || (val != 0 && val != 1)) {
        Debug("%s: invalid parameter\n", __FUNCTION__);
        return -1;
    }

    if (bv->length == bv->capacity) {
        Debug("%s: bit vector is full\n", __FUNCTION__);
        return -1;
    }

    bv->length++;
    int rc = bvSet(bv, bv->length-1, val);
    if (rc != 0) {
        bv->length--;
    }
    return rc;
}


/*******************************************************************
 * -- bvRemove
 *  
 *     Remove one bit from vector.
 *
 * Return:
 *     0 on success, -1 on failure
 *
 * Note:
 *     This operation may take a long time if vector is large
 *
 *******************************************************************/

int bvRemove(BitVector *bv, uint64_t index) {
    if (bv == NULL || index >= bv->length) {
        Debug("%s: invalid parameter\n", __FUNCTION__);
        return -1;
    }

    /* All bits after the target bit move a step forward. */
    for (int i=index+1; i<bv->length; i++) {
        uint64_t byteIndex = BV_BYTE_INDEX(i);
        bvSet(bv, i-1, BV_BIT_VAL(bv->buf[byteIndex], i));
    }

    bv->length--;
    return 0;
}


/*******************************************************************
 * -- bvClear
 *  
 *    Clean up a bit vector.  
 *
 * Return:
 *     0 on success, -1 on failure
 *
 *******************************************************************/

int bvClear(BitVector *bv) // IN/OUT
{
    if (bv == NULL) {
        Debug("%s: invalid parameter\n", __FUNCTION__);
        return -1;
    }

    bv->length = 0;
    return 0;
}


/*******************************************************************
 * -- bvPrint
 *  
 *    Output bit vector
 *
 * Return:
 *     None
 *
 *******************************************************************/

void bvDump(const BitVector *bv) // IN
{
    printf("Bitvector length: %lu, capacity: %lu, bits:\n", bv->length, bv->capacity);
    if (bv == NULL) {
        printf("(emtpy)\n");
        return;
    }

    uint8_t val;
    for(uint64_t i=0; i < bv->length; i++) {
        bvGet(bv, i, &val);
        printf("%u", val);
    }
    printf("\n");
}
