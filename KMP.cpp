#include <iostream>
#include <bits/stdc++.h>
#include <fstream>
#include <sstream>

using namespace std;


void LPS(string pi, vector<int>& Lps) {
    
    Lps[0] = 0;
    int len = 0;
    int i = 1;
    
    while (i < pi.length()) 
    {
        if (pi[i] == pi[len]) 
        {
            len++;
            Lps[i] = len;
            i++;
            continue;
        }
        else 
        {
            if (len == 0) 
            {
                Lps[i] = 0;
                i++;
                continue;
            }
            else 
            {
                len = Lps[len - 1];
                continue;
            }
        }
    }
}

int KMP(string pattern, string pi) {
    
    bool isInFile = false;
    unsigned long long n = pi.length();
    unsigned long long m = pattern.length();
    vector<int>Lps(m);

    LPS(pattern, Lps); // This function constructs the Lps array.

    int i = 0;
    int j = 0;
    int index;
    
    while (i < n) {
        if (pattern[j] == pi[i]) 
        { 
            i++; 
            j++; 
        } // If there is a match continue.
        if (j == m) 
        {
            cout << pattern << ',';
            cout << i - m << endl;    // if j==m it is confirmed that we have found the pattern and we output the index
            // and update j as Lps of last matched character.
            j = Lps[j - 1];
            index = i - m;
            break;
        }
        else if (i < n && pattern[j] != pi[i]) 
        {  // If there is a mismatch
            if (j == 0) 
            {
                i++;
            }         // if j becomes 0 then simply increment the index i
            else 
            {
                j = Lps[j - 1];  //Update j as Lps of last matched character
            }
        }
    }
    return index;
}

string loadFile(const string& path) {
    
    ifstream file;
    
    file.open(path);
    
    if (file.fail()) 
    {
        cout << "File open failed" << endl;
    }

    stringstream buffer;
    
    buffer << file.rdbuf();
    
    string file_contents;
    
    file_contents = buffer.str();

    file.close();

    return file_contents;
}

int main()
{
    string pi = loadFile("C:/Private/Algs/PiGenerator/PiGenerator/pi.txt");
    string pattern = "1569817";
    int index = KMP(pattern, pi);
    ofstream out("output.txt");
    out << pattern << ',' << index;
    out.close();
    return 0;
}
