#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <iomanip>
#include <climits>
#include "exhautive_search.h"
#include "naive_bound.h"
// #include "nearest_neighbor_heuristic.h"
#include "hungarian_alg.h"

#define INF (INT_MAX - 100000)

// #ifndef RESET
// // Khai báo công tắc màu toàn cục
// extern bool USE_COLOR; 

// // Sửa lại đống define màu thành thế này:
// #define RESET   (USE_COLOR ? "\033[0m"  : "")
// #define RED     (USE_COLOR ? "\033[31m" : "")
// #define GREEN   (USE_COLOR ? "\033[32m" : "")
// #define YELLOW  (USE_COLOR ? "\033[33m" : "")
// #define BLUE    (USE_COLOR ? "\033[34m" : "")
// #define CYAN    (USE_COLOR ? "\033[36m" : "")
// #define MAGENTA (USE_COLOR ? "\033[35m" : "")
// #endif

using namespace std;
using namespace std::chrono;

bool USE_COLOR = true;

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
    cout << "  Instructed by: Dr. Nguyen Van Hieu\n";
    cout << "  Developed by : Tran Duc Thang & Nguyen Gia Lam\n";
    cout << "======================================================\n\n";
}

void printInitialMatrix(const vector<vector<int>>& costMatrix, int n) {
    cout << YELLOW << "\n=== INITIAL COST MATRIX ===" << RESET << "\n";
    cout << "    " << setw(6) << "";
    for (int j = 0; j < n; j++) cout << BLUE << setw(5) << j << RESET;
    cout << "\n";
    for (int i = 0; i < n; i++) {
        cout << "    " << BLUE << setw(4) << i << " |" << RESET;
        for (int j = 0; j < n; j++) {
            if (costMatrix[i][j] >= INF / 2) cout << setw(5) << "INF";
            else cout << setw(5) << costMatrix[i][j];
        }
        cout << "\n";
    }
    cout << "======================================================\n\n";
}

int main() {
    printHeader();
    
    int algoChoice, inputChoice, illuChoice, outChoice;
    vector<vector<int>> costMatrix;
    int n = 0;
    
    bool isRunning = true;
    bool hasInput = false;

    while (isRunning) {
        cout << CYAN << "\nSelect Algorithm:\n" << RESET;
        cout << "  1) Exhaustive Search\n";
        cout << "  2) Naive Bound\n";
        cout << "  3) Nearest Neighbor Heuristic (NNH)\n";
        cout << "  4) Hungarian Algorithm\n";
        cout << "  5) Hungarian + NNH\n";
        cout << "Choice: "; cin >> algoChoice;
        
        if (!hasInput) {
            cout << CYAN << "\nSelect Input Method:\n" << RESET;
            cout << "  1) Keyboard\n";
            cout << "  2) File\n";
            cout << "Choice: "; cin >> inputChoice;

            if (inputChoice == 2) {
                cin.ignore();
                string filename;
                cout << YELLOW << "Enter the file path: " << RESET;
                getline(cin, filename);
                ifstream fin(filename);
                if (!fin) {
                    cout << RED << "Error: Cannot open " << filename << " (make sure the file exists)" << RESET << "\n";
                    continue; // Lỗi thì vòng lại từ đầu, không sập chương trình
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
                cout << CYAN << "\nEnter number of cities: " << RESET; cin >> n;
                costMatrix.assign(n, vector<int>(n));
                cout << "Enter cost matrix (use 0 for diagonals):\n";
                for (int i = 0; i < n; i++) {
                    for (int j = 0; j < n; j++) {
                        cin >> costMatrix[i][j];
                        if (i == j) costMatrix[i][j] = INF;
                    }
                }
            }
            hasInput = true;
        }

        cout << CYAN << "\nSimulate Execution?\n"<< RESET << "  1) Yes\n  2) No\nChoice: "; 
        cin >> illuChoice;
        bool doIllu = (illuChoice == 1);

        cout << CYAN<<"\nOutput to output.txt?\n"<< RESET << "  1) Yes\n  2) No\nChoice: "; 
        cin >> outChoice;

        // Output routing
        streambuf *coutbuf = cout.rdbuf(); 
        ofstream out;
        if (outChoice == 1) {
            USE_COLOR = false;
            out.open("output.txt");
            cout.rdbuf(out.rdbuf());
            printHeader(); 
        }

        // In ma trận ra sau khi đã setup file stream để lưu được vào output.txt nếu cần
        printInitialMatrix(costMatrix, n);

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
            cout.rdbuf(coutbuf); // Trả lại stdout cho terminal
            USE_COLOR = true;    // Khôi phục lại cờ màu cho các vòng lặp sau
            out.close();         // Dọn dẹp file stream
            cout << GREEN << "\nDone! Check output.txt for the trace." << RESET << "\n";
        }

        // --- MENU ĐIỀU HƯỚNG ---
        int nextAction;
        cout << CYAN << "\nWHAT DO YOU WANT TO DO NEXT?\n" << RESET;
        cout << "  1) Try another algorithm (keep current matrix)\n";
        cout << "  2) Load a new input\n";
        cout << "  3) Exit program\n";
        cout << "Choice: "; cin >> nextAction;

        if (nextAction == 3) {
            isRunning = false;
            cout << "Exiting... Bye!\n";
        } else if (nextAction == 2) {
            hasInput = false; // Xóa cờ để vòng lặp sau bắt nhập lại
        } 
        // Nếu chọn 1, hasInput vẫn là true, vòng lặp sau sẽ nhảy qua block nhập dữ liệu.
    }

    return 0;
}