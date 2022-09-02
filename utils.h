#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>


#define ACCURACY 5
#define SINGLE_MAX 500
#define EXPONENT_MAX 500
#define BUF_SIZE 1024


int gcd(int a, int b);

int probablePrime(int n, int k);

int solovayPrime(int a, int n);

int jacobi(int a, int n);

int modpow(long long a, long long b, int c);


/**
 * 随机生成素数.
 */
int randPrime(int n);

/**
 * 获取一个随机数.
 */
int randExponent(int phi, int n);

/**
 * 计算模反元素.
 */
int inverse(e, phi);

/**
 * 产生一对随机的密钥.
// */
//int GetKey(int* e, int* d, int* T);




