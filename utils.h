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
 * �����������.
 */
int randPrime(int n);

/**
 * ��ȡһ�������.
 */
int randExponent(int phi, int n);

/**
 * ����ģ��Ԫ��.
 */
int inverse(e, phi);

/**
 * ����һ���������Կ.
// */
//int GetKey(int* e, int* d, int* T);




