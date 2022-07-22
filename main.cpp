#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <sstream>
#include <map>
#include <vector>
#include <math.h>
#define lli long long int
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
};

const int decimal_fix = 100000;

vector <Transaction> transactions;

string inputFile = "", outputFile = "";

vector <vector <Transaction> > solutions;
map <lli, map<int, vector <vector <string> > > > mapa;

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

vector < vector<string> > generateCombinations(int totalElements, lli value, vector<Transaction> conjunto, vector<Transaction> S)
{
    // Critério de parada
    if(totalElements < 1 || value <= 0 ||
       conjunto.empty() || totalElements > conjunto.size()
    ){
        vector< vector<string> > empty_vector;
        return empty_vector;
    }
    
    for(int i=0; i < conjunto.size(); i++){
        S.push_back(conjunto[i]);
        lli valueCurrent = conjunto[i].value;
        string idCurrent = conjunto[i].id;

        //Removendo elemento do conjunto
        conjunto.erase(conjunto.begin()+i);
        lli new_value = value - valueCurrent;

        if(new_value == 0){
            vector <string> newSolution2;
            newSolution2.push_back(idCurrent);
            mapa[value][totalElements].push_back(newSolution2);
            if(totalElements == 1){
                // printStack(S);
                solutions.push_back(S);
            }
        } 
        else {
            vector < vector <string> > previous_solution;
            if(mapa[new_value][totalElements-1].empty()){
                previous_solution = generateCombinations(totalElements-1, new_value, conjunto, S);
            } else {
                previous_solution = mapa[new_value][totalElements-1];
            }
            
            if(not mapa[new_value][totalElements-1].empty()){
                for(int j=0; j < mapa[new_value][totalElements-1].size(); j++){
                    vector <string> aux_vector = mapa[new_value][totalElements-1][j];
                    aux_vector.push_back(idCurrent);
                    mapa[value][totalElements].push_back(aux_vector);
                }
            }
        }
        S.pop_back();
    }
    return mapa[value][totalElements];
}

int main(int argc, char *argv[]){
    inputFile = string(argv[1]);
    outputFile = string(argv[2]);
    string keyValueS = string(argv[3]);
    string nElementsS = string(argv[4]);
    double keyValue = 0.0;
    int nElements = 0;
    
    if(argc != 5){
        cout << "Error: Provide input, output, key value and quantity of elements." << endl;
        cout << argc << " arguments." << endl;
        return -1;
    }

    try {    
        keyValue = stod(keyValueS);
        nElements = stoi(nElementsS);
    } catch(...) {
        cout << "The sequence of elements is wrong." << endl;
        cout << "A float and integer are expected." << endl;  
    }

    getInput();
    // printTransactions();

    vector <Transaction> empty;
    lli keyValueT = trunc(keyValue*decimal_fix);
    vector < vector<string> > S = generateCombinations(nElements, keyValueT, transactions, empty);
    printSolutions(solutions);

    return 0;
}