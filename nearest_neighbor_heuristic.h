#include <iostream>
#include <vector>

#define INF (INT_MAX - 100000)

using namespace std;

class NNH {
private:
    int upperbound;
    int len;
    vector<bool> isVisited;

    void __solveDfs(int cntDst, int currDst, int currSum, vector<vector<int>>& cost){
        if (cntDst == len - 1){
            upperbound = min(currSum + cost[currDst][0], upperbound);
            return;
        }
        for (int dst = 1; dst < len; dst ++){
            if (currSum + cost[currDst][dst] >= upperbound) continue;
            if (isVisited[dst]) continue;
            isVisited[dst] = true;
            __solveDfs(cntDst + 1, dst, currSum + cost[currDst][dst], cost);
            isVisited[dst] = false;
        }
    }
public:    
    int upperboundNNH(vector<vector<int>> &cost, int size){
        int upperbound = INF;
        isVisited = vector<bool> (size, false);
        for(int startDst = 0; startDst < size; startDst++){
            int currDst = startDst;
            int nextDst = startDst;
            int sumCost = 0;
            int cntDst = 0;
            isVisited[startDst] = true;
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
                cntDst++;
            }
            upperbound = min(upperbound, sumCost + cost[currDst][startDst]);
            fill(isVisited.begin(), isVisited.end(), false);
        }
        return upperbound;
    }

    int solveTsp_NNH(vector<vector<int>> &cost){
        len = cost.size();
        upperbound = upperboundNNH(cost, len);
        isVisited = vector<bool>(len, false);
        __solveDfs(0, 0, 0, cost);
        return upperbound;
    }
};