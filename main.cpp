#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <iomanip>
#include "exhautive_search.h"
#include "naive_bound.h"
// #include "nearest_neighbor_heuristic.h"
#include "hungarian_alg.h"

#define INF (INT_MAX - 100000)

using namespace std;
using namespace std::chrono;

// Function pointer wrappers
typedef int (*AlgoFunc)(vector<vector<int>>&, bool);

int runExhaustive(vector<vector<int>>& cost, bool illu) {
    exhautive_search solver;
    return illu ? solver.illuTsp_exhautiveSearch(cost) : solver.solveTsp_exhautiveSearch(cost);
}

int runNaive(vector<vector<int>>& cost, bool illu) {
    naive_bound solver;
    return illu ? solver.illuTsp_naive_bound(cost) : solver.solveTsp_naive_bound(cost);
}

int runNNH(vector<vector<int>>& cost, bool illu) {
    NNH solver;
    return illu ? solver.illuTsp_NNH(cost) : solver.solveTsp_NNH(cost);
}

int runHungarianOnly(vector<vector<int>>& cost, bool illu) {
    hungarian_alg solver;
    return illu ? solver.illuTsp_hungarian_alg(cost, false) : solver.solveTsp_hungarian_alg(cost, false);
}

int runHungarianNNH(vector<vector<int>>& cost, bool illu) {
    hungarian_alg solver;
    return illu ? solver.illuTsp_hungarian_alg(cost, true) : solver.solveTsp_hungarian_alg(cost, true);
}

void printHeader() {
    cout << "\n======================================================\n";
    cout << CYAN << "  PROJECT: TRAVELING SALESPERSON PROBLEM (TSP)" << RESET << "\n";
    cout << "  Instructed by: Dr. Nguyen Van A\n";
    cout << "  Developed by : Tran Duc Thang & Nguyen Gia Lam\n";
    cout << "======================================================\n\n";
}

int main() {
    printHeader();
    
    int algoChoice, inputChoice, tcChoice, illuChoice, outChoice;
    
    cout << "Select Algorithm:\n";
    cout << "  1) Exhaustive Search\n";
    cout << "  2) Naive Bound\n";
    cout << "  3) Nearest Neighbor Heuristic (NNH)\n";
    cout << "  4) Hungarian Algorithm\n";
    cout << "  5) Hungarian + NNH\n";
    cout << "Choice: "; cin >> algoChoice;
    
    cout << "\nSelect Input Method:\n";
    cout << "  1) Keyboard\n";
    cout << "  2) File\n";
    cout << "Choice: "; cin >> inputChoice;

    vector<vector<int>> costMatrix;
    int n;

    if (inputChoice == 2) {
        cout << "\nSelect Testcase (0-5):\n";
        cout << "  0 (3 cities), 1 (5 cities), 2 (10 cities)\n";
        cout << "  3 (15 cities), 4 (20 cities), 5 (23 cities)\n";
        cout << "Choice: "; cin >> tcChoice;
        
        string filename = "testcase/input" + to_string(tcChoice) + ".txt";
        ifstream fin(filename);
        if (!fin) {
            cout << RED << "Error: Cannot open " << filename << " (make sure the 'testcase' folder exists)" << RESET << "\n";
            return 1;
        }
        fin >> n;
        costMatrix.assign(n, vector<int>(n));
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                fin >> costMatrix[i][j];
                if (i == j) costMatrix[i][j] = INF; // Force diagonal to INF
            }
        }
        fin.close();
    } else {
        cout << "\nEnter number of cities: "; cin >> n;
        costMatrix.assign(n, vector<int>(n));
        cout << "Enter cost matrix (use 0 for diagonals):\n";
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                cin >> costMatrix[i][j];
                if (i == j) costMatrix[i][j] = INF;
            }
        }
    }

    cout << "\nSimulate Execution?\n  1) Yes\n  2) No\nChoice: "; 
    cin >> illuChoice;
    bool doIllu = (illuChoice == 1);

    cout << "\nOutput to output.txt?\n  1) Yes\n  2) No\nChoice: "; 
    cin >> outChoice;

    // Output routing
    streambuf *coutbuf = cout.rdbuf(); 
    ofstream out;
    if (outChoice == 1) {
        out.open("output.txt");
        cout.rdbuf(out.rdbuf());
        printHeader(); 
    }

    AlgoFunc algos[] = {runExhaustive, runNaive, runNNH, runHungarianOnly, runHungarianNNH};
    if(algoChoice < 1 || algoChoice > 5) algoChoice = 1;

    auto start = high_resolution_clock::now();
    
    int result = algos[algoChoice - 1](costMatrix, doIllu);
    
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);

    cout << "\n======================================================\n";
    cout << GREEN << "FINAL RESULT: " << result << RESET << "\n";
    cout << YELLOW << "EXECUTION TIME: " << duration.count() << " ms" << RESET << "\n";
    cout << "======================================================\n";

    if (outChoice == 1) {
        cout.rdbuf(coutbuf); // Reset back to terminal
        cout << GREEN << "\nDone! Check output.txt for the trace." << RESET << "\n";
    }

    return 0;
}