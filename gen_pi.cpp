#include <cmath>
#include <iostream>
#include <fstream>
#include <gmpxx.h>

using namespace std;

struct BinarySplit {
    mpz_class P, Q, T;
};

struct Arguments {
    int piDigits;
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

int multiply_bitewise(int a, int b)
{
    int n1 = abs(a), n2 = abs(b), result = 0;
    bool neg = false;
    if (min(a, b) < 0 && max(a, b) >= 0) neg = true;
    while (n2 > 0)
    {
        if (n2 & 1 == 1) result += n1;
        n2 >>= 1; n1 <<= 1;
    }
    if (neg) return (~(result)+1);
    else return result;
}

int Add(int x, int y)
{
    while (y != 0)
    {
        unsigned carry = x & y;
        x = x ^ y;
        y = carry << 1;
    }
    return x;
}
// Compute BinarySplit (by Binary Splitting Algorithm)
// ref: https://en.wikipedia.org/wiki/Binary_splitting#:~:text=In%20mathematics%2C%20binary%20splitting%20is,hypergeometric%20series%20at%20rational%20points.
BinarySplit Chudnovsky::computeWithBinarySplit(int n1, int n2) {
    int m;
    BinarySplit result;



    if (n1 + 1 == n2) {
        int t = multiply_bitewise(6, n2);
        result.P = ((n2 >> 1) - 1);
        result.P *= (t - 1);
        result.P *= (t - 5);
        result.Q = C3_24 * n2 * n2 * n2;
        result.T = (A + B * n2) * result.P;
        if ((n2 & 1) == 1) {
            result.T = -result.T;
        }
    }
    else {
        m = (n1 + n2) / 2;
        BinarySplit reccBinarySplit1 = computeWithBinarySplit(n1, m);
        BinarySplit reccBinarySplit2 = computeWithBinarySplit(m, n2);
        result.P = reccBinarySplit1.P * reccBinarySplit2.P;
        result.Q = reccBinarySplit1.Q * reccBinarySplit2.Q;
        result.T = reccBinarySplit1.T * reccBinarySplit2.Q + reccBinarySplit1.P * reccBinarySplit2.T;
    }



    return result;
}



void Chudnovsky::getPiExpansion() {
    cout << "PI computation for " << DIGITS << " digits" << endl;

   // Timestamp for start of computation
    start = clock();

    // PI computation starts here
    BinarySplit split = computeWithBinarySplit(0, N);
    mpf_class pi(0, PRECISION);
    pi = D * sqrt((mpf_class)E) * split.Q;
    pi /= (A * split.Q + split.T);

       // Timestamp for end of computation
        end = clock();
    cout << "Computation took " << (double)(end - start) / CLOCKS_PER_SEC << " seconds." << endl;

  // Outputs to file
    ofstream ofs("pi.txt");
    ofs.precision(DIGITS + 1);
    ofs << pi << endl;
}

Arguments parseArguments(int argc, char* argv[]) {
    Arguments args;
    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], "-d") == 0 && i + 1 < argc) {
            args.piDigits = atoi(argv[i + 1]);
        }
    }
    return args;
}



int main(int argc, char* argv[]) {
    try {
        Arguments args = parseArguments(argc, argv);
        
        Chudnovsky chudnovsky = Chudnovsky(args.piDigits);
        
        // Computes pi and saves the result to text file
        chudnovsky.getPiExpansion();
    }
    catch (...) {
        cout << "Unexpected error occured" << endl;
        return -1;
    }
    
    return 0;
}
