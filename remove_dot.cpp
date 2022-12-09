#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include <algorithm>

using namespace std;

int main() {
    // this script removes all dots from the input file
    ifstream inp("pi.txt"); 
    string str;
    if (inp) {
        ostringstream ss;
        ss << inp.rdbuf(); 
        str = ss.str();
    }

    str.erase(std::remove(str.begin(), str.end(), '.'), str.end());

    inp.close();

    ofstream out("pi.txt", ofstream::trunc);
    out << str;

    out.close();
}