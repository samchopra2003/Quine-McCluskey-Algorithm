#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <set>
#include <bits/stdc++.h>
#include <algorithm>
using namespace std;

int NBITS = 4; // default size is 4 (not const as it can change depending on user input)

// function prototypes
string cvtToBinary(int);
vector<string> reduceMinterms(vector<string>);
bool differByOneBit(string, string);
string replaceBit(string, string);
void primeImplicantChart(vector<string>);
vector<string> getAllMinterms(string);
int cvtToDecimal(string);
vector<string> EssPrimeImp(vector<string>, vector<string>);
vector<string> cvtToVars(vector<string>);

int main() {
    vector<string> vec = {"0x01", "01x1", "011x", "x00x", "x0x0", "xx10"};
    primeImplicantChart(vec);

    return 0;
}
// function definitions
string cvtToBinary(int n) { // binary converter function (8 bits)
    string bin;
    for (int i=0; i < NBITS; i++) { // will give us required 0s
        bin += "0";
    }
    int i = 0; // counter (allows us to edit the 16-bit binary no.)
    while (n > 0) {
        bin[NBITS-(1+i)] = (n % 2 == 0) ? '0' : '1'; // ternary operator
        i++;
        n /= 2;
    }
    return bin;
}

vector<string> reduceMinterms(vector<string> vec) {
    vector<string> result;
    set<string> uniqueTerms;
    vector<int> checked(vec.size(), 0); // will keep track of what has been checked (1) in coming loop

    for (int i=0; i < vec.size(); i++) { // two loops to compare two terms at a time
        for (int j=0; j < vec.size(); j++) {
            if (i == j) // skip over
                continue;
            if (differByOneBit(vec[i], vec[j])) {
                // cout << endl << vec[i] << " and " << vec[j] << " differ by one bit" << endl;
                checked[i] = 1;
                checked[j] = 1;
                string m = replaceBit(vec[i], vec[j]);
                // result.push_back(m);
                uniqueTerms.insert(m);
            }
        }
        if (checked[i] == 0) { // this means not checked
            // result.push_back(vec[i]);
            uniqueTerms.insert(vec[i]);
        }
    }
    // all unique minterms due to set
    result.assign(uniqueTerms.begin(), uniqueTerms.end());

    return result;
}

bool differByOneBit(string a, string b) { // tell whether minterm differs by one bit
    int diff = 0; // counts the difference in bits
    for (int i=0; i < a.length(); i++) {
        if (a[i] != b[i])
            diff++;
        if (diff == 2) {
            return false;
        }
    }
    return true;
}

string replaceBit(string a, string b) {
    string res;
    for (int i=0; i < a.length(); i++) {
        if (a[i] != b[i]) {
            a[i] = 'x'; // x denotes don't care situation
            break;
        }
    }
    res = a;
    return res;
}

void primeImplicantChart(vector<string> vec) {
    vector<string> primeImplicants;
    vector<string> strMinTerms, hold, holdy; 
    vector<int> cover; // includes all minterms that need to be covered
    set<string> set0;
    for (int i=0; i < vec.size(); i++) {
        holdy = getAllMinterms(vec[i]);
        for (int k=0; k < holdy.size(); k++) {
            hold.push_back(holdy[k]);
        } 
        for (int j=0; j < holdy.size(); j++) {
            set0.insert(holdy[j]);
        } 
    }
    strMinTerms.assign(set0.begin(), set0.end());
    for (int i=0; i < strMinTerms.size(); i++) {
        cover.push_back(cvtToDecimal(strMinTerms[i]));
    } 
    // now sort according to no. of xs in descending order
    /*
    vector<string> sortedMinTerms;
    int cou, maxInd, maxX, i;
    const int vecSize = vec.size();
    vector<string>::iterator it; // helps remove appropriate element
    while (sortedMinTerms.size() != vecSize) {
        maxInd = 0;
        maxX = 0; // counts max no. of x's in a term
        for (i=0; i < vec.size(); i++) {
            cou = count(vec[i].begin(),vec[i].end(),'x');
            if (cou > maxX) {
                maxX = cou;
                maxInd = i;
            }
        }
        sortedMinTerms.push_back(vec[maxInd]);
        it = vec.begin() + maxInd;
        vec.erase(it); // delete maxInd term
    }
    */
    // find all essential prime implicants
    vector<string> essentialPrimes = EssPrimeImp(vec, hold), copyVec = vec;
    vector<string>::iterator it0;
    vector<int>::iterator rem0;
    vector<string> temp0;
    vector<int> tempDec0;
    
    int i, l, x, c;
    for (int j=0; j < essentialPrimes.size(); j++) { 
        for (i=0; i < copyVec.size(); i++) {
            if (essentialPrimes[j] == copyVec[i]) {
                temp0 = getAllMinterms(essentialPrimes[j]);
                for (x=0; x < temp0.size(); x++) {
                    tempDec0.push_back(cvtToDecimal(temp0[x]));
                }
                for (x=0; x < tempDec0.size(); x++) {
                    for (c=0; c < cover.size(); c++) {
                        if (tempDec0[x] == cover[c]) {
                            rem0 = cover.begin() + c;
                            cover.erase(rem0); // removes this element from the cover
                            break;
                        }
                    }
                }
                it0 = copyVec.begin() + i;
                copyVec.erase(it0);
                temp0.empty();
                tempDec0.empty();
            }
        }
        primeImplicants.push_back(essentialPrimes[j]);
    }

    // use brute force method to find minimum cover now lol
    string arr[copyVec.size()];
    for (int z=0; z < copyVec.size(); z++) {
        arr[z] = copyVec[z];
    }
    sort(arr, arr + copyVec.size()); 
    vector<string> implicantCombs, implicantCover;
    vector<string> temp;
    vector<int> copyCover = cover;
    cout << "cover:" << copyCover[0] << copyCover[1] << endl;
    vector<int> tempDec;
    vector<int>::iterator rem;
    bool partOfCover; // flag to denote whether minterm is part of cover
    int minCoverSize = 1000; // random large value, could even set to infinity
    vector<string> minImplicantCover;

    do {
        for (int z=0; z < copyVec.size(); z++) { // will get all combos of prime Implicants remaining
            implicantCombs.push_back(arr[z]);
            // cout << implicantCombs[z] << " ";
        }
        // cout << endl;
        cout << endl << "cover elements: " << cover[0] << cover[1] << endl;
        while (!cover.empty()) { // stop when cover is empty
            for (l=0; l < implicantCombs.size(); l++) {
                temp = getAllMinterms(implicantCombs[l]);
                partOfCover = false;
                cout << "minterms of " << implicantCombs[l] << endl;
                for (x=0; x < temp.size(); x++) {
                    tempDec.push_back(cvtToDecimal(temp[x]));
                    cout << tempDec[x] << " ";
                }
                cout << endl;
                for (x=0; x < tempDec.size(); x++) {
                    for (c=0; c < cover.size(); c++) {
                        if (tempDec[x] == cover[c]) {
                            rem = cover.begin() + c;
                            cover.erase(rem); // removes this element from the cover
                            partOfCover = true;
                            break;
                        }
                    }
                }
                temp.clear();
                tempDec.clear();

                if (partOfCover) {
                    // primeImplicants.push_back(copyVec[l]);
                    implicantCover.push_back(implicantCombs[l]);
                }
                if (cover.empty()) {
                    // break;
                }
            }
            if (cover.empty()) {
                // break;
            }
        }
        cout << "Implicant cover:" << endl;
        for (int z=0; z < implicantCover.size(); z++) {
            cout << implicantCover[z] << " ";
        }
        cout << endl;

        implicantCombs.clear(); 
        cover = copyCover;
        if (implicantCover.size() < minCoverSize) { // to get minimum cover from all combos
            minCoverSize = implicantCover.size();
            minImplicantCover.clear();
            minImplicantCover = implicantCover; // set to implicant cover
        }
        implicantCover.clear();
        
    } while (next_permutation(arr, arr + copyVec.size()));

    // testing
    cout << endl << endl << "Mincover:" << endl;
    for (int z=0; z < minImplicantCover.size(); z++) {
        cout << minImplicantCover[z] << endl;
    }
        
    //return primeImplicants;
}

vector<string> getAllMinterms(string a) {
    vector<string> vals;
    vector<string> minTerms;
    int i = 0, x_counter = 0; // counter
    while (a.find('x') < a.length()) { // basically if x is in a
        if (i < a.length()) { // only iterate if i less than the length of a
            if (a[i] == 'x') {
                a[i] = '1';
                minTerms.push_back(a);
                a[i] = '0';
                minTerms.push_back(a);
                x_counter++;
            }
            i++;
        }
    }
    // some terms will still hv x (we need to now iterate over the minTerms vector)
     vector<string> temp;
    if (x_counter >= 2) { // recursive call
        for (int c=0; c < minTerms.size(); c++) {
            if (minTerms[c].find('x') < minTerms[c].length()) {
                temp = getAllMinterms(minTerms[c]);
                for (int k=0; k < temp.size(); k++) {
                    vals.push_back(temp[k]);
                }
            }
        }
    }
    else {
        vals = minTerms;
    }
    return vals;
}

int cvtToDecimal(string a) {
    int temp = stoi(a);
    int dec_value = 0;
    int base = 1;
    
    while (temp) {
        int last_digit = temp % 10;
        temp = temp / 10;
        dec_value += last_digit * base;
        base = base * 2;
    }
 
    return dec_value;
}

vector<string> EssPrimeImp(vector<string> minTerms, vector<string> hold) {
    vector<string> combos, ess; 
    int cou;
    for (int i=0; i < minTerms.size(); i++) {
        combos = getAllMinterms(minTerms[i]);
        for (int j=0; j < combos.size(); j++) {
            cou = count(hold.begin(), hold.end(), combos[j]);
            if (cou == 1) { // this means essential prime implicant
                ess.push_back(minTerms[i]);
                break;
            }
        }
    }
    return ess;
}  

vector<string> cvtToVars(vector<string> vec) {
    string varArr[16] = {"a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p"};
    string varsNeeded[NBITS];
    vector<string> out;
    for (int i=0; i < NBITS; i++) { // will use the vars required 
        varsNeeded[i] = varArr[i];
    }
    int varCounter = 0; // iterates through varsNeeded
    string literal = "", term = "";
    for (int i=0; i < vec.size(); i++) { // iterates through vector
        for (int j=0; j < vec[i].length(); j++) { // iterates through the string element
            if (vec[i][j] == 'x') {
                varCounter++;
                continue;
            }
            else if (vec[i][j] == '0') {
                literal += varsNeeded[varCounter];
                literal += "'";
            }   
            else {
                literal += varsNeeded[varCounter];
            }  
            // cout << literal; 
            term += literal;
            literal = ""; // reset
            varCounter++;
        }
        //cout << endl << term << endl;
        out.push_back(term);
        term = "";
        varCounter = 0; // reset
    }
    return out;
}
