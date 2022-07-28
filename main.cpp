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
#define MAP_TYPE2 map <lli, vector< vector<Transaction> >  >
#define MAP_TYPE1 map <lli, map<int, vector <vector <Transaction> > > >
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
    lli sum_irf;
    lli sum_total;

    Solution(vector<Transaction> S){
        this->S = S;
        this->sum_irf = 0;
        this->sum_total = 0;
        for(int i=0; i < S.size(); i++){
            sum_irf += S[i].irf;
            sum_total += S[i].irf;
        }
        sort(this->S.begin(), this->S.end());
    }

    int size(){
        return this->S.size();
    }

    bool operator == (const Solution &b){
        string a_id = "";
        string b_id = "";

        for(int i=0; i < this->S.size(); i++){
            a_id += this->S[i].id;
        }

        for(int i=0; i < b.S.size(); i++){
            b_id += this->S[i].id;
        }
        return a_id == b_id;
    }

    bool operator < (const Solution &b) const {
        string a_id = "";
        string b_id = "";

        for(int i=0; i < this->S.size(); i++){
            a_id += this->S[i].id;
        }

        for(int i=0; i < b.S.size(); i++){
            b_id += b.S[i].id;
        }
        return a_id < b_id;
    }

    void printS(ostream &cout) const{
        cout << "\"(";
        for(int i=0; i < this->S.size(); i++){
            if(i!=0) cout << ",";
            cout << this->S[i].id;
        }
        cout << ")\"";
    }
};

// Variables and constants

const int decimal_fix = 100;
lli irfTargetT = 0;
vector <Transaction> transactions;
string inputFile = "", outputFile = "";
vector <Solution> solutions;
MAP_TYPE2 mapa_alg2;
MAP_TYPE1 mapa_alg1;

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
    fin.close();
}

bool sortbyirf(Solution a, Solution b){
    double diff_a = abs(a.sum_irf - irfTargetT);
    double diff_b = abs(b.sum_irf - irfTargetT);
    return diff_a < diff_b;
}

void outputSolutions(vector<Solution> solutions){
    sort(solutions.begin(), solutions.end(), sortbyirf);

    ofstream myfile;
    myfile.open(outputFile);

    if(solutions.empty()){
        myfile << "There is no solution." << endl;
        return ;
    }
    myfile << "\"IDs\", \"SUM_TOTAL_AMOUNT\", \"SUM_IRF_AMOUNT\"" << endl;

    for(int i=0; i < solutions.size(); i++){
        solutions[i].printS(myfile);
        myfile << "\"" << float(solutions[i].sum_total)/decimal_fix << "\",";
        myfile << "\"" << float(solutions[i].sum_irf)/decimal_fix << "\"" << endl;
    }

    myfile.close();
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

lli min(lli a, lli b){
    return a < b ? a : b;
}

lli getMin(){
    lli minValue = INT_MAX;
    for(int i=0; i < transactions.size(); i++){
        minValue = min(minValue, transactions[i].value);
    }
    return minValue;
}

void algorithm2( lli SUM_TOTAL_AMOUNT, int totalElements)
{
    int i = int(getMin());
    for(;i <= SUM_TOTAL_AMOUNT; i++){
        for(int j=0; j < transactions.size(); j++){
            lli dif = i - transactions[j].value;

            if(dif < 0) continue;
            
            if(dif == 0){ //  Element found
                vector <Transaction> new_solution;
                new_solution.push_back(transactions[j]);
                mapa_alg2[i].push_back(new_solution);
            }
            else{
                if(not mapa_alg2[dif].empty()){ // Finding another solution
                    for(int k=0; k < mapa_alg2[dif].size(); k++){
                        if(mapa_alg2[dif][k].size() <= totalElements){
                            vector <Transaction> new_solution = mapa_alg2[dif][k];
                            new_solution.push_back(transactions[j]);
                            mapa_alg2[i].push_back(new_solution);
                        }
                    }
                }
            }
        }
    }
}
void algorithm1(int totalElements, lli value, vector<Transaction> conjunto, vector<Transaction> S)
{
    // Critério de parada
    if(totalElements < 1 || value <= 0 ||
       conjunto.empty() || totalElements > conjunto.size()
    ){
        return;
    }

    int i = conjunto.size() - 1;
    conjunto.pop_back();

    S.push_back(conjunto[i]);

    Transaction tCurrent = conjunto[i];
    lli valueCurrent = conjunto[i].value;
    string idCurrent = conjunto[i].id;

    lli new_value = value - valueCurrent;

    if(new_value == 0){
        vector <Transaction> newSolution2;
        newSolution2.push_back(tCurrent);
        mapa_alg1[value][totalElements].push_back(newSolution2);
        if(totalElements == 1){
            printStack(S);
            solutions.push_back(Solution(S));
        }
    }
    else if(new_value > 0){
        if(mapa_alg1[new_value][totalElements-1].empty()){
            algorithm1(totalElements-1, new_value,conjunto, S);
        }

        for(int i=0; i < mapa_alg1[new_value][totalElements-1].size();i++){
            vector<Transaction> new_solution = mapa_alg1[new_value][totalElements-1][i];
            new_solution.push_back(tCurrent);
            mapa_alg1[value][totalElements].push_back(new_solution);
        }
        S.pop_back();
        algorithm1(totalElements, value,conjunto, S);
    }
}

vector <Solution> getSolutions(lli amountTarget, int nElements){
    vector <Solution> output;
    set <Solution> S;

    if(mapa_alg2.find(amountTarget) != mapa_alg2.end()){
        for(int i=0; i < mapa_alg2[amountTarget].size();i++){
            vector<Transaction> current = mapa_alg2[amountTarget][i];
            if(current.size() == nElements){
                S.insert(Solution(current));
            }
        }
    }

    set <Solution> :: iterator it = S.begin();

    for(;it != S.end(); it++){
        output.push_back(*it);
    }

    return output;
}

int main(int argc, char *argv[]){
    inputFile = string(argv[1]);
    outputFile = string(argv[2]);
    string amountTargetS = string(argv[3]);
    string irfTargetS = string(argv[4]);
    string nElementsS = string(argv[5]);
    string algorithm = string(argv[6]);
    double amountTarget = 0.0;
    double irfTarget = 0.0;
    int nElements = 0;
    
    if(argc != 7){
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

    // inputFile = "input2.csv";
    // outputFile = "output3.csv";
    // string algorithm = "algorithm1";
    // double amountTarget = 96.8;
    // double irfTarget = 0.9;
    // int nElements = 3;
    
    getInput();

    vector <Transaction> empty;
    lli amountTargetT = trunc(amountTarget*decimal_fix);
    irfTargetT = trunc(irfTarget*decimal_fix);
 
    if(algorithm == "algorithm1"){
        vector <Transaction> empty;
        algorithm1(nElements, amountTargetT, transactions, empty);
        outputSolutions(solutions);
    }

    if(algorithm == "algorithm2") {
        algorithm2(amountTargetT, nElements);
        outputSolutions(getSolutions(amountTargetT, nElements));
    }

    return 0;
}