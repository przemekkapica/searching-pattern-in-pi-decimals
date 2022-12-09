#include <cmath>
#include <iostream>
#include <fstream>
#include <gmpxx.h>

using namespace std;

struct BinarySplit {
    mpz_class D, H, L;
};

class Chudnovsky {
    mpz_class A, B, C, D, E, C3_24; // Multi precision integers
    int DIGITS, PRECISION, N;            
    double DIGITS_PER_TERM;        
    clock_t start, end; // Timestamps
    BinarySplit computeWithBinarySplit(int n1, int n2); // Computes using Binary Split Algorithm

    public:
        Chudnovsky(int digits);                   
        void getPiExpansion();            
};

Chudnovsky::Chudnovsky(int digits) {
    // Constants from Chudnovsky algorithm (ref: https://en.wikipedia.org/wiki/Chudnovsky_algorithm)
    DIGITS = digits;
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


// Compute BinarySplit (by Binary Splitting Algorithm)
// ref: https://en.wikipedia.org/wiki/Binary_splitting#:~:text=In%20mathematics%2C%20binary%20splitting%20is,hypergeometric%20series%20at%20rational%20points.
BinarySplit Chudnovsky::computeBinarySplit(int n1, int n2) {
    int m;
    BinarySplit result;

    if (n1 + 1 == n2) {
        result.D  = (2 * n2 - 1);
        result.D *= (6 * n2 - 1);
        result.D *= (6 * n2 - 5);
        result.H  = C3_24 * n2 * n2 * n2;
        result.L  = (A + B * n2) * result.D;
        if ((n2 & 1) == 1) {
            result.L = - result.L;
        }
    } else {
        m = (n1 + n2) / 2;
        BinarySplit reccBinarySplit1 = computeBinarySplit(n1, m);
        BinarySplit reccBinarySplit2 = computeBinarySplit(m, n2);
        result.D = reccBinarySplit1.D * reccBinarySplit2.D;
        result.H = reccBinarySplit1.H * reccBinarySplit2.H;
        result.L = reccBinarySplit1.L * reccBinarySplit2.H + reccBinarySplit1.D * reccBinarySplit2.L;
    }

    return result;
}

void Chudnovsky::getPiExpansion() {
    cout << "PI computation for " << DIGITS << " digits" << endl;

    // Timestamp for start of computation
    start = clock();

    // PI computation starts here
    BinarySplit split = computeBinarySplit(0, N);
    mpf_class pi(0, PRECISION);
    pi = D * sqrt((mpf_class)E) * split.H;
    pi /= (A * split.H + split.L);

    // Timestamp for end of computation
    end = clock();
    cout << "Computation took " << (double)(end - start) / CLOCKS_PER_SEC << " seconds." << endl;

    // Outputs to file
    ofstream ofs ("pi.txt");
    ofs.precision(DIGITS + 1);
    ofs << pi << endl;
}

struct Arguments {
    int piDigits;
};

Arguments parseArguments(int argc, char *argv[]) {
    Arguments args;
    for (int i = 0; i < argc; i++) {   
        if (strcmp(argv[i], "-d") == 0 && i + 1 < argc) {
            args.piDigits = atoi(argv[i + 1]);
        } 
    }
    return args;
}

int main(int argc, char *argv[]) {
    try { 
        Arguments args = parseArguments(argc, argv);
        
        Chudnovsky chudnovsky;
        chudnovsky(args.piDigits);

        // Computes pi and saves the result to text file
        chudnovsky.getPiExpansion();
    }
    catch (...) {
        cout << "Unexpected error occured" << endl;
        return -1;
    }

    return 0;
}