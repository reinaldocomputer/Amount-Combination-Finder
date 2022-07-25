#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <sstream>
#include <map>
#include <set>
#include <vector>
#include <math.h>
#define lli long long int
#define MAP_TYPE map <lli, vector< vector<Transaction> >  >
using namespace std;

class Transaction{
    public:
    string id;
    lli value;
    lli irf;

    Transaction(string id, lli value, lli irf)
    {
        this->id = id;
        this->value = value;
        this->irf = irf;
    }
    const bool operator < (const Transaction &b) const {
        return this->id < b.id;
    }

    bool operator != (const Transaction &b) const {
        return this->id != b.id;
    }    
};

class Solution {
    public:
    vector<Transaction> S;

    Solution(vector<Transaction> S){
        sort(S.begin(), S.end());
        this->S = S;
    }

    int size(){
        return this->S.size();
    }

    bool operator == (const Solution &b){
        if(this->S.size() != b.S.size()) return false;
        for(int i=0; i < b.S.size(); i++){
            if(S[i] != b.S[i]) return false;
        }
        return true;
    }

    bool operator < (const Solution &b) const {
        string a_id = "";
        string b_id = "";

        for(int i=0; i < this->S.size(); i++){
            a_id += this->S[i].id;
        }

        for(int i=0; i < b.S.size(); i++){
            b_id += this->S[i].id;
        }
        return a_id < b_id;
    }

    void printS() const{
        cout << "(";
        for(int i=0; i < this->S.size(); i++){
            if(i!=0) cout << ",";
            cout << this->S[i].id;
        }
        cout << ")" << endl;
    }
};

const int decimal_fix = 100;

vector <Transaction> transactions;

string inputFile = "", outputFile = "";

vector <vector <Transaction> > solutions;
MAP_TYPE mapa;

void printTransactions(){
    cout << "=== PRINT TRANSACTIONS==" << endl;
    for(int i=0; i < transactions.size();i++){
        cout << "id: " << transactions[i].id << endl;
        cout << "value: "  << transactions[i].value << endl;
        cout << "irf: " << transactions[i].irf << endl;
        // printf("value: %lf\n", float(transactions[i].value)/float(decimal_fix));
        // printf("irf: %lf\n", float(transactions[i].irf)/float(decimal_fix));
    }
    cout << "=== END TRANSACTIONS==" << endl;
}


void getInput(){
    ifstream fin;
    fin.open(inputFile);
    string line;
    while(getline(fin, line)){
        stringstream ss(line);

        string id, aux_value, aux_irf;
        double value = 0, irf = 0;

        getline(ss, id, ',');
        getline(ss, aux_value, ',');
        getline(ss, aux_irf, ',');
        
        value = stod(aux_value);
        irf = stod(aux_irf);

        value = value * decimal_fix;
        irf = irf * decimal_fix;
        lli lli_value = trunc(value);
        lli lli_irf = trunc(irf);

        transactions.push_back(Transaction(id, lli_value, lli_irf));
    }
}


void printSolutions(vector< vector<Transaction> > S){
    lli sum_total = 0;
    lli irf_total = 0;
    if(S.empty()){
        cout << "There is no solution." << endl;
        return ;
    }
    cout << "\"IDs\", \"SUM_TOTAL_AMOUNT\", \"SUM_IRF_AMOUNT\"" << endl;

    for(int i=0; i < S.size(); i++){
        cout << "\"(";
        for(int j=0; j < S[i].size(); j++){
            if(j!=0) cout << ",";
            cout << S[i][j].id;
            sum_total += S[i][j].value;
            irf_total += S[i][j].irf;
        }
        cout << ")\",";
        cout << "\"" << float(sum_total)/decimal_fix << "\",";
        cout << "\"" << float(irf_total)/decimal_fix << "\"" << endl;
    }
}

void printStack(vector <Transaction> S){
    lli sum_total = 0;
    lli irf_total = 0;
    cout << endl << "A solution found." << endl;
    cout << "(";
    for(int i=0; i < S.size(); i++){
        if(i!=0) cout << ",";
        cout << S[i].id;
        sum_total += S[i].value;
        irf_total += S[i].irf;

    }
    cout << ") ";
    cout << "SUM_ALL: " << float(sum_total)/decimal_fix;
    cout << " IRF_ALL: " << float(irf_total)/decimal_fix << endl;
}

void algorithm2( lli SUM_TOTAL_AMOUNT, int totalElements)
{
    for(int i=1; i <= SUM_TOTAL_AMOUNT; i++){
        for(int j=0; j < transactions.size(); j++){
            lli dif = i - transactions[j].value;

            if(dif < 0) continue;
            //  Element found
            if(dif == 0){
                vector <Transaction> new_solution;
                new_solution.push_back(transactions[j]);
                mapa[i].push_back(new_solution);
            }
        
            // Finding another solution
            if(not mapa[dif].empty()){
                for(int k=0; k < mapa[dif].size(); k++){
                    if(mapa[dif][k].size() < totalElements){
                        vector <Transaction> new_solution = vector<Transaction>(mapa[dif][k].begin(), mapa[dif][k].end());
                        new_solution.push_back(transactions[j]);
                        mapa[i].push_back(new_solution);
                    }
                }
            }
        }
    }
}

vector< vector <Transaction> > getSolutions(lli amountTarget, int nElements){
    vector <vector <Transaction> > output;
    set <Solution> Ss;

    if(mapa.find(amountTarget) != mapa.end()){
        for(int i=0; i < mapa[amountTarget].size();i++){
            vector<Transaction> current = mapa[amountTarget][i];
            if(current.size() == nElements){
                Ss.insert(Solution(current));
            }
        }
    }

    set <Solution> :: iterator it = Ss.begin();

    for(;it != Ss.end(); it++){
        output.push_back(it->S);
    }

    return output;
}

int main(int argc, char *argv[]){
    inputFile = string(argv[1]);
    outputFile = string(argv[2]);
    string amountTargetS = string(argv[3]);
    string nElementsS = string(argv[4]);
    string irfTargetS = string(argv[4]);
    double amountTarget = 0.0;
    double irfTarget = 0.0;
    int nElements = 0;
    
    if(argc != 5){
        cout << "Error: Provide input, output, key value and quantity of elements." << endl;
        cout << argc << " arguments." << endl;
        return -1;
    }

    try {    
        amountTarget = stod(amountTargetS);
        irfTarget = stod(irfTargetS);
        nElements = stoi(nElementsS);
    } catch(...) {
        cout << "The sequence of elements is wrong." << endl;
        cout << "A float and integer are expected." << endl;  
    }

    getInput();

    vector <Transaction> empty;
    lli amountTargetT = trunc(amountTarget*decimal_fix);
 
    generateCombinations(amountTargetT, nElements);
    printSolutions(getSolutions(amountTargetT, nElements));

    return 0;
}