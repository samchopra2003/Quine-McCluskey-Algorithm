// Online C++ compiler to run C++ program online
#include <iostream>
#include <bits/stdc++.h>
#include <vector>
#include <string>
#include <set>
using namespace std;

int NBITS = 4;

string cvtToBinary(int);
void primeImplicantChart(vector<string>);
vector<string> getAllMinterms(string);
int cvtToDecimal(string);
vector<string> EssPrimeImp(vector<string>, vector<string>);
void cvtToVars(vector<string> vec);

int main() {
    vector<string> vec = {"0x01", "01x1", "011x", "x00x", "x0x0", "xx10"};
    primeImplicantChart(vec);
    //cvtToVars(vec);
    

    return 0;
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
    cout << "hold";
    for (int i=0; i < hold.size(); i++) {
        cout << hold[i] << endl;
    }
    // cout << endl << endl;
    strMinTerms.assign(set0.begin(), set0.end());
    for (int i=0; i < strMinTerms.size(); i++) {
        cover.push_back(cvtToDecimal(strMinTerms[i]));
    } 
    /*
    for (int i=0; i < hold.size(); i++) {
        cout << hold[i] << endl;
    } */
    cout << endl << endl << endl;
    // now sort according to no. of xs in descending order
    vector<string> sortedMinTerms;
    int cou, maxInd, maxX, i;
    const int vecSize = vec.size();
    vector<string>::iterator it; // helps remove appropriate element
    while (sortedMinTerms.size() != vecSize) {
        maxInd = 0;
        maxX = 0; // counts max no. of x's in a term
        cout << endl;
        for (i=0; i < vec.size(); i++) {
            cou = count(vec[i].begin(),vec[i].end(),'x');
            // cout << vec[i] << " : " << cou << endl;
            if (cou > maxX) {
                maxX = cou;
                maxInd = i;
            }
        }
        // cout << vec[maxInd] << endl;
        sortedMinTerms.push_back(vec[maxInd]);
        it = vec.begin() + maxInd;
        vec.erase(it); // delete maxInd term
        // cout << vec[maxInd] << endl;
        /*
        for (int i=0; i < vec.size(); i++) {
            cout << vec[i] << endl;
        }  
        cout << endl;
        */
    }
    
    cout << endl << endl << "sortedminterms" << endl; 
    for (int l=0; l < sortedMinTerms.size(); l++) { 
        cout << sortedMinTerms[l] << endl;  
    }
    cout << endl << endl;

    // find all essential prime implicants
    vector<string> essentialPrimes = EssPrimeImp(sortedMinTerms, hold);
    for (int l=0; l < essentialPrimes.size(); l++) { 
        cout << essentialPrimes[l] << endl;
    }
    cout << endl << endl;

    vector<string>::iterator it0;
    vector<int>::iterator rem0;
    vector<string> temp0;
    vector<int> tempDec0;
    int l, x, c;
    for (int j=0; j < essentialPrimes.size(); j++) {
        for (i=0; i < sortedMinTerms.size(); i++) {
            if (essentialPrimes[j] == sortedMinTerms[i]) {
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
                it0 = sortedMinTerms.begin() + i;
                sortedMinTerms.erase(it0);
                temp0.empty();
                tempDec0.empty();
            }
        }
        primeImplicants.push_back(essentialPrimes[j]);
    }
    cout << endl << "remaining minterms: " << endl;
    for (l=0; l < sortedMinTerms.size(); l++) { 
        cout << sortedMinTerms[l] << endl;
    }

    // remove elements from the cover until no elements are left (with prime implicants)
    vector<string> temp;
    vector<int> tempDec;
    vector<int>::iterator rem;
    bool partOfCover; // flag to denote whether minterm is part of cover
    while (!cover.empty()) { // stop when cover is empty
        for (l=0; l < sortedMinTerms.size(); l++) {
            temp = getAllMinterms(sortedMinTerms[l]);
            partOfCover = false;
            for (x=0; x < temp.size(); x++) {
                tempDec.push_back(cvtToDecimal(temp[x]));
            }
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
            temp.empty();
            tempDec.empty();

            if (partOfCover) {
                primeImplicants.push_back(sortedMinTerms[l]);
            }
        }

    } 
    cout << endl << "prime implicants: " << endl;
     for (l=0; l < primeImplicants.size(); l++) { 
        cout << primeImplicants[l] << endl;
    } 

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
    cout << endl << "1001 count: " << count(hold.begin(), hold.end(), "1001") << endl << endl;
    return ess;
}  

void cvtToVars(vector<string> vec) {
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

}