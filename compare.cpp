#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include <algorithm>

using namespace std;

int main() {
    // this script compares two files
    ifstream inp1("pi.txt"); 
    ifstream inp2("pi2.txt");

    string str1;
    if (inp1) {
        ostringstream ss;
        ss << inp1.rdbuf(); 
        str1 = ss.str();
    }

    string str2;
    if (inp2) {
        ostringstream ss;
        ss << inp2.rdbuf(); 
        str2 = ss.str();
    }

    if (str1.size() != str2.size()) {
        cout << "Inputs have different sizes" << endl;
        return 0;
    }

    for (int i = 0; i < str1.size(); i++) {
        if (str1[i] != str2[i]) {
            cout << "Inputs differ at index " << i + 1 << endl;
            return 0;
        }
    }

    cout << "Inputs are the same" << endl;
    return 0;

}