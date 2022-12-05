#include <cmath>
#include <iostream>
#include <fstream>
#include <gmpxx.h>

using namespace std;

struct PQT {
    mpz_class P, Q, T;
};

class Chudnovsky {
    mpz_class A, B, C, D, E, C3_24;     // Multi precision integers
    int DIGITS, PRECISION, N;            
    double DIGITS_PER_TERM;        
    clock_t start, end;                     // Timestamps
    PQT computePQT(int n1, int n2);     // Computer PQT (by BSA)

    public:
        Chudnovsky();                   
        void getPiExpansion();            
};

Chudnovsky::Chudnovsky() {
    // Constants from Chudnovsky algorithm (ref: https://en.wikipedia.org/wiki/Chudnovsky_algorithm)
    DIGITS = 100;
    A = 13591409;
    B = 545140134;
    C = 640320;
    D = 426880;
    E = 10005;
    DIGITS_PER_TERM = 14.1816474627254776555;  // = log(53360^3) / log(10)
    C3_24 = C * C * C / 24;
    N = DIGITS / DIGITS_PER_TERM;
    PRECISION = DIGITS * log2(10);
}


// Compute PQT (by Binary Splitting Algorithm)
PQT Chudnovsky::computePQT(int n1, int n2) {
    int m;
    PQT result;

    if (n1 + 1 == n2) {
        result.P  = (2 * n2 - 1);
        result.P *= (6 * n2 - 1);
        result.P *= (6 * n2 - 5);
        result.Q  = C3_24 * n2 * n2 * n2;
        result.T  = (A + B * n2) * result.P;
        if ((n2 & 1) == 1) {
            result.T = - result.T;
        }
    } else {
        m = (n1 + n2) / 2;
        PQT reccPQT1 = computePQT(n1, m);
        PQT reccPQT2 = computePQT(m, n2);
        result.P = reccPQT1.P * reccPQT2.P;
        result.Q = reccPQT1.Q * reccPQT2.Q;
        result.T = reccPQT1.T * reccPQT2.Q + reccPQT1.P * reccPQT2.T;
    }

    return result;
}

void Chudnovsky::getPiExpansion() {
    cout << "PI computation for " << DIGITS << " digits" << endl;

    // Timestamp for start of computation
    start = clock();

    // PI computation starts here
    PQT PQT = computePQT(0, N);
    mpf_class pi(0, PRECISION);
    pi  = D * sqrt((mpf_class)E) * PQT.Q;
    pi /= (A * PQT.Q + PQT.T);

    // Timestamp for end of computation
    end = clock();
    cout << "Computation took "
         << (double)(end - start) / CLOCKS_PER_SEC
         << " seconds." << endl;

    // Outputs to file
    ofstream ofs ("pi.txt");
    ofs.precision(DIGITS + 1);
    ofs << pi << endl;
}

int main() {
    Chudnovsky chudnovsky;
    
    try {    
        // Computes pi and saves the result to text file
        chudnovsky.getPiExpansion();
    }
    catch (...) {
        cout << "Unexpected error occured" << endl;
        return -1;
    }

    return 0;
}