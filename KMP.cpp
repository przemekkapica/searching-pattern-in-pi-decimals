#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;

void LPS(string pi, vector<int>& lps) {
    lps[0] = 0;
    int len = 0;
    int i = 1;
    
    while (i < pi.length()) {
        if (pi[i] == pi[len]) {
            len++;
            lps[i] = len;
            i++;
            continue;
        }
        else {
            if (len == 0) {
                lps[i] = 0;
                i++;
                continue;
            }
            else {
                len = lps[len - 1];
                continue;
            }
        }
    }
}

int KMP(string pattern, string pi) {
    bool isInFile = false;
    unsigned long long n = pi.length();
    unsigned long long m = pattern.length();
    vector<int>lps(m);

    LPS(pattern, lps); // This function constructs the Lps array.

    int i = 0;
    int j = 0;
    int index;
    
    while (i < n) {
        if (pattern[j] == pi[i]) { 
            i++; 
            j++; 
        } // If there is a match continue.
        if (j == m) {
            cout << pattern << ',';
            cout << i - m << endl;    // if j==m it is confirmed that we have found the pattern and we output the index
            // and update j as Lps of last matched character.
            j = lps[j - 1];
            index = i - m;
            break;
        }
        else if (i < n && pattern[j] != pi[i]) {  // If there is a mismatch
            if (j == 0) {
                i++;
            }         // if j becomes 0 then simply increment the index i
            else {
                j = lps[j - 1];  // Update j as Lps of last matched character
            }
        }
    }
    return index;
}

string loadFile(const string& path) {
    ifstream file;
    
    file.open(path);
    
    if (file.fail()) {
        cout << "File open failed" << endl;
    }

    stringstream buffer;
    
    buffer << file.rdbuf();
    
    string file_contents;
    
    file_contents = buffer.str();

    file.close();

    return file_contents;
}

struct Arguments {
    string inputFilePath;
    string pattern;
};

Arguments parseArguments(int argc, char *argv[]) {
    Arguments args;
    for (int i = 0; i < argc; i++) {   
        if (strcmp(argv[i], "-i") == 0 && i + 1 < argc) {
            args.inputFilePath = argv[i + 1];
        } 
        else if (strcmp(argv[i], "-p") == 0 && i + 1 < argc) {
            args.pattern = argv[i + 1];
        }
    }
    return args;
}

int main(int argc, char *argv[]) {
    try {
        Arguments args = parseArguments(argc, argv);
        
        string pi = loadFile("C:/Private/Algs/PiGenerator/PiGenerator/pi.txt");
        // string pi = loadFile(args.inputFilePath);

        string pattern = "1569817";
        // string pattern = args.pattern;

        int index = KMP(pattern, pi);

        ofstream out("output.txt");
        out << pattern << ',' << index;
        out.close();
    }
    catch (...) {
        cout << "Unexpected error occured" << endl;
        return -1;
    }
    return 0;
}
