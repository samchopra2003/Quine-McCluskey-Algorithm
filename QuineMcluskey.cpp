#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <set>
#include <bits/stdc++.h>
using namespace std;

int NBITS = 4; // default size is 4 (not const as it can change depending on user input)

// function prototypes
string cvtToBinary(int);
vector<string> reduceMinterms(vector<string>);
bool differByOneBit(string, string);
string replaceBit(string, string);
vector<string> primeImplicantChart(vector<string>);
vector<string> getAllMinterms(string);
int cvtToDecimal(string);
vector<string> EssPrimeImp(vector<string>, vector<string>);
vector<string> cvtToVars(vector<string>);

int main() {
    // step 0- read input from file
    
    ifstream Infile;
    Infile.open("minterms.txt");
    string line;
    getline(Infile, line); // first input line is size of no. of minterms
    const int SIZE = stoi(line.substr(7)); 
    getline(Infile, line); // second line will give NBITS
    const int NBITS = stoi(line.substr(8)); // controls bits
    int minTermArray[SIZE];

    int i_count = 0;
    while (getline(Infile, line)) { // will get all remaining lines
        minTermArray[i_count] = stoi(line);
        // cout << minTermArray[i_count];
        i_count++;
    }

    
    // step 1 - convert minterms to binary
    vector<string> binaryMinTerms;
    for (int m=0; m < SIZE; m++) {
        string bin = cvtToBinary(minTermArray[m]);
        binaryMinTerms.push_back(bin);
        // cout << binaryMinTerms[m] << endl;
    }

    // step 2/3 - start comparing minterms
    vector<string> reducedMinterms = reduceMinterms(binaryMinTerms);
    vector<string> copyMinTerms = reducedMinterms;
    // more iterations need to be done if differeByOneBit is true
    while (true) {
        bool simplify = 0; // currently set to false
        for (int i=0; i < reducedMinterms.size(); i++) {
            for (int j=0; j < reducedMinterms.size(); j++) {
                if (i == j) // skip over
                    continue; 
                if (differByOneBit(reducedMinterms[i], reducedMinterms[j])) {
                    simplify = true;
                    break;
                }
                if (simplify) 
                    break;
            }
        }
        if (simplify){
            reducedMinterms.clear();
            reducedMinterms = reduceMinterms(copyMinTerms);
            copyMinTerms.clear();
            copyMinTerms = reducedMinterms;
        }
        // while true break condition (if simplify false)
        else
            break;
    } /*
    for (int i=0; i < reducedMinterms.size(); i++) {
        cout << reducedMinterms[i] << endl;
    } */

    // step 4 - prime implicant chart
    vector<string> output = primeImplicantChart(reducedMinterms);

    // step 5- convert to variables now
    vector<string> vars = cvtToVars(output);

    // print result
    cout << endl << "We are running the Quine McCluskey Algorithm made by Samarth Chopra" << endl;
    cout << "Logic minimization of minterms(";
    for (int i=0; i < SIZE; i++) {
        if (i == (SIZE - 1)) {
            cout << minTermArray[i];
        }
        else
            cout << minTermArray[i] << ", ";
    } 
    cout << "):" << endl << "f = ";

    for (int i=0; i < vars.size(); i++) {
        if (i == (vars.size() - 1)) {
            cout << vars[i] << endl << endl;
        }
        else
            cout << vars[i] << " + ";
    } 

    Infile.close();
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

vector<string> primeImplicantChart(vector<string> vec) {
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
    // find all essential prime implicants
    vector<string> essentialPrimes = EssPrimeImp(sortedMinTerms, hold);
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

    // now remove elements from the cover until no elements are left
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
    return primeImplicants;
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
