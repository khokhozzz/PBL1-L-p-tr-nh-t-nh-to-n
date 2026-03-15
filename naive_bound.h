#include <iostream>
#include <vector>
#include <iomanip>
#include <climits>

#define INF (INT_MAX - 100000)

#ifndef RESET
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define CYAN    "\033[36m"
#endif

using namespace std;

class naive_bound {
private:
    int res;
    int len;
    vector<bool> isVisited;

    void __solveDfs(int cntDst, int currDst, int currSum, vector<vector<int>>& cost){
        if (cntDst == len - 1){
            res = min(currSum + cost[currDst][0], res);
            return;
        }
        for (int dst = 1; dst < len; dst ++){
            if (currSum + cost[currDst][dst] >= res) continue;
            if (isVisited[dst]) continue;
            isVisited[dst] = true;
            __solveDfs(cntDst + 1, dst, currSum + cost[currDst][dst], cost);
            isVisited[dst] = false;
        }
    }

    void __illuDfs(int cntDst, int currDst, int currSum, vector<vector<int>>& cost, vector<int>& path) {
        if (cntDst == len - 1) {
            int finalCost = currSum + cost[currDst][0];
            cout << CYAN << "[-] Step (Naive Bound): " << RESET << "Path = [";
            for (int i = 0; i < path.size(); i++) cout << path[i] << (i < path.size() - 1 ? " -> " : "");
            cout << "]\n";
            if (finalCost < res) {
                cout << GREEN << "    -> NEW BEST PATH FOUND! Cost: " << finalCost << RESET << "\n";
                res = finalCost;
            } else {
                cout << "    -> Path completed but cost " << finalCost << " >= bound " << res << "\n";
            }
            return;
        }

        for (int dst = 1; dst < len; dst++) {
            if (isVisited[dst]) continue; // Chặn trước cho nhẹ nợ

            cout << CYAN << "\n[-] Step (Naive Bound): " << RESET << "Path = [";
            for (int i = 0; i < path.size(); i++) cout << path[i] << (i < path.size() - 1 ? " -> " : "");
            cout << "], Current Cost = " << YELLOW << currSum << RESET << ", Bound = " << RED << (res == INF ? "INF" : to_string(res)) << RESET << "\n";

            cout << "    Standing at city " << currDst << " checking city " << dst << ", row array: [";
            for (int i = 0; i < len; i++) {
                if (cost[currDst][i] >= INF / 2 || isVisited[i] || i == 0) cout << "INF";
                else cout << cost[currDst][i];
                if (i < len - 1) cout << ", ";
            }
            cout << "]\n";

            if (currSum + cost[currDst][dst] >= res) {
                cout << RED << "    -> PRUNED at city " << dst << "! (Cost " << currSum + cost[currDst][dst] << " >= Bound " << res << ")" << RESET << "\n";
                continue;
            }
            
            cout << "    -> Exploring city " << YELLOW << dst << RESET << " with cost " << cost[currDst][dst] << "\n";
            isVisited[dst] = true;
            path.push_back(dst);
            
            __illuDfs(cntDst + 1, dst, currSum + cost[currDst][dst], cost, path);
            
            path.pop_back();
            isVisited[dst] = false;
        }
    }

public:
    int solveTsp_naive_bound(vector<vector<int>>& cost){
        len = cost.size();
        res = INF;
        isVisited = vector<bool>(len, false);
        __solveDfs(0, 0, 0, cost);
        return res;
    }

    int illuTsp_naive_bound(vector<vector<int>>& cost){
        len = cost.size();
        res = INF;
        isVisited = vector<bool>(len, false);
        vector<int> path;
        path.push_back(0);

        cout << YELLOW << "\n=== STARTING NAIVE BOUND SEARCH ===" << RESET << "\n";
        __illuDfs(0, 0, 0, cost, path);
        cout << GREEN << "\n=> FINAL MIN COST (Naive Bound): " << res << RESET << "\n";
        return res;
    }
};