/* Copyright (c) 2018, wang wenzhi */

/* Benchmark testing on bv.c:
 *     1) 100K bits of append
 *     2) 1M bits of append
 *     3) 10M bits of append
 *     4) 100M bits of append
 *     5) 10K bits of set 
 *     6) 100K bits of set 
 *     7) 1M bits of set 
 *     8) 10K bits of remove 
 *     9) 100K bits of remove 
 *     10) 1M bits of remove 
 *
 *     TODO: testing with multiple threads
 *
 *     TODO: testing with multiple threads
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "bv.h"


#define BVBENCH_RAND_BIT(i)  (i%2)


uint64_t Random_Index(uint64_t limit)
{
     return (random() * limit / RAND_MAX);
}


char *StrElapse(time_t elapse)
{
    static char tmBuf[16];
    struct tm *tmElapse = gmtime(&elapse);

    strftime(tmBuf, sizeof(tmBuf), "%T", tmElapse);
    return tmBuf;
}


void PrintResult(const char *op, uint64_t bits, time_t elapse)
{
    const char *fmt = "####BitVector Benchmark########\n"
                      "  Operation: %s\n"
                      "       Bits: %lu\n"
                      "   Duration: %s\n"
                      "###############################\n\n";
    printf(fmt, op, bits, StrElapse(elapse)); 
}


void  bvBenchmark_Append(uint64_t bits)
{
    time_t start, end;
    BitVector *bv = bvCreate(bits) ;
    assert(bv != NULL);

    time(&start);
    int rc;
    for (uint64_t i=0; i<bits; i++) {
        rc = bvAppend(bv, BVBENCH_RAND_BIT(i));
        assert(rc == 0);
    }

    time(&end);
    PrintResult("Append", bits, end-start);
    bvClear(bv);
    bvDestroy(bv);
}


void  bvBenchmark_Set(uint64_t bits)
{
    time_t start, end;
    BitVector *bv = bvCreate(bits) ;
    assert(bv != NULL);

    int rc;
    for (uint64_t i=0; i<bits; i++) {
        rc = bvAppend(bv, 0);
        assert(rc == 0);
    }

    time(&start);
    srandom(time(NULL));
    for (uint64_t i=0; i<bits; i++) {
        uint64_t index = Random_Index(bits);
        //printf("Random_Index: %lu\n", index);
        rc = bvSet(bv, index, BVBENCH_RAND_BIT(index));
        assert(rc == 0);
    }
    time(&end);
    PrintResult("Set", bits, end-start);
    //bvDump(bv);
    bvClear(bv);
    bvDestroy(bv);
}


void  bvBenchmark_Remove(uint64_t bits)
{
    time_t start, end;
    BitVector *bv = bvCreate(bits) ;
    assert(bv != NULL);

    int rc;
    for (uint64_t i=0; i<bits; i++) {
        rc = bvAppend(bv, 0);
        assert(rc == 0);
    }

    time(&start);
    srandom(time(NULL));
    for (uint64_t i=0; i<bits; i++) {
        uint64_t index = Random_Index(bvCapacity(bv));
        //printf("Random_Index: %lu\n", index);
        rc = bvSet(bv, index, BVBENCH_RAND_BIT(index));
        assert(rc == 0);
    }
    time(&end);
    PrintResult("Remove", bits, end-start);
    //bvDump(bv);
    bvClear(bv);
    bvDestroy(bv);
}


int main(int argc, char **argv)
{
    printf("\n%s started\n", argv[0]);
    bvBenchmark_Append(1000000);   // 1M
    bvBenchmark_Append(10000000);  // 10M
    bvBenchmark_Append(100000000); // 100M

    bvBenchmark_Set(1000000);   // 1M
    bvBenchmark_Set(10000000);  // 10M
    bvBenchmark_Set(100000000); // 100M

    bvBenchmark_Remove(10000);    // 10K
    bvBenchmark_Remove(100000);   // 100K
    bvBenchmark_Remove(10000000); // 1M
    printf("%s passed\n", argv[0]);
}
