#pragma once
#include <iostream>
#include <vector>
#include <iomanip>
#include <climits>

#define INF (INT_MAX - 100000)

// #ifndef RESET
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define CYAN    "\033[36m"
#define MAGENTA "\033[35m"
// #endif

using namespace std;

class NNH {
private:
    void __solveDfs(int cntDst, int currDst, int currSum, vector<vector<int>>& cost, vector<int>& path){
        if (cntDst == len - 1){
            if (currSum + cost[currDst][0] < upperbound) {
                upperbound = currSum + cost[currDst][0];
                bestPath = path;
                bestPath.push_back(0);
            }
            return;
        }
        for (int dst = 1; dst < len; dst ++){
            if (currSum + cost[currDst][dst] >= upperbound) continue;
            if (isVisited[dst]) continue;
            isVisited[dst] = true;
            path.push_back(dst);
            
            __solveDfs(cntDst + 1, dst, currSum + cost[currDst][dst], cost, path);
            
            path.pop_back();
            isVisited[dst] = false;
        }
    }

    void __illuDfs(int cntDst, int currDst, int currSum, vector<vector<int>>& cost, vector<int>& path) {
        if (cntDst == len - 1) {
            int finalCost = currSum + cost[currDst][0];
            cout << CYAN << "[-] Step (NNH Bound DFS): " << RESET << "Path = [";
            for (int i = 0; i < path.size(); i++) cout << path[i] << (i < path.size() - 1 ? " -> " : "");
            cout << "]\n";
            if (finalCost < upperbound) {
                cout << GREEN << "    -> NEW BEST PATH! Cost: " << finalCost << " (beats bound " << upperbound << ")" << RESET << "\n";
                upperbound = finalCost;
                bestPath = path;
                bestPath.push_back(0);
            } else {
                cout << "    -> Path complete. Cost " << finalCost << " (did not beat bound)\n";
            }
            return;
        }

        for (int dst = 1; dst < len; dst++) {
            if (isVisited[dst]) continue; 

            cout << CYAN << "\n[-] Step (NNH Bound DFS): " << RESET << "Path = [";
            for (int i = 0; i < path.size(); i++) cout << path[i] << (i < path.size() - 1 ? " -> " : "");
            cout << "], Curr Cost = " << YELLOW << currSum << RESET << ", NNH Bound = " << MAGENTA << upperbound << RESET << "\n";

            cout << "    Standing at city " << currDst << " checking city " << dst << ", row array: [";
            for (int i = 0; i < len; i++) {
                if (cost[currDst][i] >= INF / 2 || isVisited[i] || i == 0) cout << "INF";
                else cout << cost[currDst][i];
                if (i < len - 1) cout << ", ";
            }
            cout << "]\n";

            if (currSum + cost[currDst][dst] >= upperbound) {
                cout << RED << "    -> PRUNED at city " << dst << "! (Cost " << currSum + cost[currDst][dst] << " >= NNH Bound " << upperbound << ")" << RESET << "\n";
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

protected:
    int upperbound;
    int len;
    vector<bool> isVisited;
    vector<int> bestPath; // Protected để thằng cu Hungarian xài chung

public:    
    int solveUpperboundNNH(vector<vector<int>> &cost, int size){
        int upperbound = INF;
        isVisited = vector<bool> (size, false);
        for(int startDst = 0; startDst < size; startDst++){
            int currDst = startDst;
            int nextDst = startDst;
            int sumCost = 0;
            int cntDst = 0;
            isVisited[startDst] = true;
            vector<int> localPath = {startDst};

            while (cntDst < size - 1)
            {
                int minCost = INF;
                for(int dst = 0; dst < size; dst++){
                    if (isVisited[dst]) continue;
                    if (cost[currDst][dst] < minCost){
                        nextDst = dst;
                        minCost = cost[currDst][dst];
                    }
                }
                sumCost += minCost;
                isVisited[nextDst] = true;
                currDst = nextDst;
                localPath.push_back(currDst);
                cntDst++;
            }
            localPath.push_back(startDst); // Khép vòng
            if (sumCost + cost[currDst][startDst] < upperbound) {
                upperbound = sumCost + cost[currDst][startDst];
                bestPath = localPath; // Lưu mảng xịn nhất của phần Heuristic
            }
            fill(isVisited.begin(), isVisited.end(), false);
        }
        return upperbound;
    }

    int illuUpperboundNNH(vector<vector<int>> &cost, int size){
        cout << BLUE << "\n--- CALCULATING NNH UPPERBOUND ---" << RESET << "\n";
        int local_upperbound = INF;
        vector<bool> local_isVisited(size, false);
        for(int startDst = 0; startDst < size; startDst++){
            cout << CYAN << "\n>>> Starting point: City " << GREEN << startDst << RESET << "\n";
            int currDst = startDst;
            int nextDst = startDst;
            int sumCost = 0;
            int cntDst = 0;
            local_isVisited[startDst] = true;
            vector<int> greedyPath;
            greedyPath.push_back(startDst);

            while (cntDst < size - 1)
            {
                int minCost = INF;
                for(int dst = 0; dst < size; dst++){
                    if (local_isVisited[dst]) continue;
                    if (cost[currDst][dst] < minCost){
                        nextDst = dst;
                        minCost = cost[currDst][dst];
                    }
                }
                
                cout << "  At city " << currDst << ", row: [";
                for (int dst = 0; dst < size; dst++) {
                    if (cost[currDst][dst] >= INF / 2 || local_isVisited[dst]) cout << "INF";
                    else if (dst == nextDst) cout << YELLOW << cost[currDst][dst] << RESET;
                    else cout << cost[currDst][dst];
                    if (dst < size - 1) cout << ", ";
                }
                cout << "] -> Picked: " << nextDst << " (Cost: " << minCost << ")\n";

                sumCost += minCost;
                local_isVisited[nextDst] = true;
                currDst = nextDst;
                greedyPath.push_back(currDst);
                cntDst++;
            }
            int cycleCost = sumCost + cost[currDst][startDst];
            greedyPath.push_back(startDst);
            cout << "  Cycle complete. Path: [";
            for(int i=0; i<greedyPath.size(); i++) cout << greedyPath[i] << (i<greedyPath.size()-1?" -> ":"");
            cout << "]\n";
            cout << "  Cost: " << sumCost << " + " << cost[currDst][startDst] << " (return) = " << cycleCost << "\n";

            if (cycleCost < local_upperbound) {
                local_upperbound = cycleCost;
                bestPath = greedyPath; // Lưu vết
            }
            fill(local_isVisited.begin(), local_isVisited.end(), false);
        }
        cout << MAGENTA << "=> Final NNH Upperbound: " << local_upperbound << RESET << "\n";
        return local_upperbound;
    }

    int solveTsp_NNH(vector<vector<int>> &cost){
        len = cost.size();
        bestPath.clear();
        upperbound = solveUpperboundNNH(cost, len);
        isVisited = vector<bool>(len, false);
        vector<int> path = {0};
        
        __solveDfs(0, 0, 0, cost, path);
        
        cout << GREEN << "\n=> OPTIMAL PATH: [";
        for(int i=0; i<bestPath.size(); i++) cout << bestPath[i] << (i<bestPath.size()-1?" -> ":"");
        cout << "]" << RESET << "\n";
        
        return upperbound;
    }

    int illuTsp_NNH(vector<vector<int>> &cost){
        len = cost.size();
        bestPath.clear();
        cout << YELLOW << "\n=== STARTING NNH HEURISTIC SEARCH ===" << RESET << "\n";
        upperbound = illuUpperboundNNH(cost, len);
        isVisited = vector<bool>(len, false);
        vector<int> path;
        path.push_back(0);
        
        cout << YELLOW << "\n=== STARTING DFS WITH NNH BOUND ===" << RESET << "\n";
        __illuDfs(0, 0, 0, cost, path);
        
        cout << GREEN << "\n=> FINAL OPTIMAL PATH: [";
        for(int i=0; i<bestPath.size(); i++) cout << bestPath[i] << (i<bestPath.size()-1?" -> ":"");
        cout << "]" << RESET << "\n";
        cout << GREEN << "=> FINAL MIN COST (NNH): " << upperbound << RESET << "\n";
        
        return upperbound;
    }
};