#pragma once

#include "utils.h"

/**
 * Computes a^b mod c，计算a^b mod c
 */
int modpow(long long a, long long b, int c) {
    int res = 1;
    while (b > 0) {
        /* Need long multiplication else this will overflow...
         必须使用长乘法，否则这将溢出*/
        if (b & 1) {
            res = (res * a) % c;
        }
        b = b >> 1;
        a = (a * a) % c; /* Same deal here */
    }
    return res;
}

/**
 * Computes the Jacobi symbol, (a, n)，计算Jacobi符号（a,n）
 */
int jacobi(int a, int n) {
    int twos, temp;
    int mult = 1;
    while (a > 1 && a != n) {
        a = a % n;
        if (a <= 1 || a == n) break;
        twos = 0;
        while (a % 2 == 0 && ++twos) a /= 2; /* Factor out multiples of 2 ,减去2的倍数*/
        if (twos > 0 && twos % 2 == 1) mult *= (n % 8 == 1 || n % 8 == 7) * 2 - 1;
        if (a <= 1 || a == n) break;
        if (n % 4 != 1 && a % 4 != 1) mult *= -1; /* Coefficient for flipping，翻转系数 */
        temp = a;
        a = n;
        n = temp;
    }
    if (a == 0) return 0;
    else if (a == 1) return mult;
    else return 0; /* a == n => gcd(a, n) != 1 */
}

int gcd(int a, int b) {
    int temp;
    while (b != 0) 
    {
        temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

int solovayPrime(int a, int n) 
{
    int x = jacobi(a, n);
    if (x == -1) x = n - 1;
    return x != 0 && modpow(a, (n - 1) / 2, n) == x;
}


int probablePrime(int n, int k) 
{
    if (n == 2) return 1;
    else if (n % 2 == 0 || n == 1) return 0;
    while (k-- > 0) 
    {
        if (!solovayPrime(rand() % (n - 2) + 2, n)) return 0;
    }
    return 1; 
}

int randPrime(int n) {
    int prime = rand() % n;
    n += n % 2; /* n needs to be even so modulo wrapping preserves oddness */
    prime += 1 - prime % 2;
    while (1) {
        if (probablePrime(prime, ACCURACY)) return prime;
        prime = (prime + 2) % n;
    }
}

int randExponent(int phi, int n) {
    int e = rand() % n;
    while (1) {
        if (gcd(e, phi) == 1) return e;
        e = (e + 1) % n;
        if (e <= 2) e = 3;
    }
}

int inverse(int n, int modulus) {
    int a = n, b = modulus;
    int x = 0, y = 1, x0 = 1, y0 = 0, q, temp;
    while (b != 0) {
        q = a / b;
        temp = a % b;
        a = b;
        b = temp;
        temp = x; x = x0 - q * x; x0 = temp;
        temp = y; y = y0 - q * y; y0 = temp;
    }
    if (x0 < 0) x0 += modulus;
    return x0;
}


