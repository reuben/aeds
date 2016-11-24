#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <assert.h>
#include <math.h>
#include <stdio.h>

#define MIN(a,b) (a)<(b)?(a):(b)
#define MAX(a,b) (a)>(b)?(a):(b)

typedef struct
{
    uint32_t num[10];
    uint8_t len;
} bigint;

bigint big_init(int n)
{
    bigint big;
    big.num[0] = n;
    int i;
    for (i = 1; i < 10; ++i) {
        big.num[i] = 0;
    }
    big.len = 1;
    return big;
}

bigint big_pow_2(int n)
{
    assert(n/32 < 10);
    bigint big;
    int i;
    for (i = 0; i < 10; ++i) {
        big.num[i] = 0;
    }
    big.num[n/32] = 1 << (n % 32);
    big.len = (n/32)+1;
    return big;
}

void big_print(bigint big)
{
    fprintf(stderr, "%lx", (unsigned long)big.num[big.len-1]);
    int i;
    for(i = big.len-1; i; i--) {
        fprintf(stderr, "%.8lx", (unsigned long)big.num[i-1]);
    }
    fprintf(stderr, "\n");
}

bigint big_add(bigint lhs, uint32_t rhs)
{
    uint32_t carry = rhs;
    int i;
    for (i = 0; i < lhs.len; ++i) {
        uint64_t tmp = carry + (uint64_t)lhs.num[i];
        lhs.num[i] = tmp;
        carry = tmp >> 32;
    }
    if (carry) {
        assert(lhs.len <= 10);
        lhs.num[lhs.len++] = carry;
    }
    return lhs;
}

bigint big_sub(bigint lhs, uint32_t rhs)
{
    if (lhs.num[0] < rhs) {
        int i;
        for (i = 1; i < lhs.len; ++i) {
            if (lhs.num[i] > 0) {
                lhs.num[i]--;
                if (lhs.num[i] == 0) {
                    lhs.len--;
                }
                int j;
                for (j = i-1; j > 0; --j) {
                    lhs.num[j] = 0xFFFFFFFF;
                }
                lhs.num[0] = 0xFFFFFFFF - rhs + 1;
                break;
            }
        }
    } else {
        lhs.num[0] -= rhs;
    }
    return lhs;
}

bigint big_mul(bigint lhs, double rhs)
{
    uint32_t carry = 0;
    int i;
    for (i = 0; i < lhs.len; ++i) {
        uint64_t tmp = (rhs * (uint64_t)lhs.num[i]) + carry;
        lhs.num[i] = tmp;
        carry = tmp >> 32;
    }
    if (carry) {
        assert(lhs.len <= 10);
        lhs.num[lhs.len++] = carry;
    }
    return lhs;
}

bool big_bit(bigint big, int bit)
{
    return big.num[bit/32] & (1 << (bit % 32));
}

bool big_lt(bigint lhs, bigint rhs)
{
    if (lhs.len != rhs.len) {
        return lhs.len < rhs.len;
    }
    int i;
    for (i = 0; i < lhs.len; ++i) {
        if (lhs.num[i] >= rhs.num[i]) {
            return false;
        }
    }
    return true;
}

bool big_gt(bigint lhs, bigint rhs)
{
    if (lhs.len != rhs.len) {
        return lhs.len > rhs.len;
    }
    int i;
    for (i = 0; i < lhs.len; ++i) {
        if (lhs.num[i] <= rhs.num[i]) {
            return false;
        }
    }
    return true;
}

bool big_eq(bigint lhs, bigint rhs)
{
    if (lhs.len != rhs.len) {
        return false;
    }
    int i;
    for (i = 0; i < lhs.len; ++i) {
        if (lhs.num[i] != rhs.num[i]) {
            return false;
        }
    }
    return true;
}
