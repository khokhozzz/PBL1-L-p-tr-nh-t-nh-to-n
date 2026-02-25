#include <bits/stdc++.h>
#define INF INT_MAX
using namespace std;


vector<int> u; vector<int> v;
int sum_u, sum_v;
int upperBound;
vector<bool> isVisited;


void printMtrx(vector<int> &cost, int Len){
    for (int j = 0; j < Len; j++)
    {
        printf("%5d", cost[j]);
    }
    cout << "\n______________________________________\n";
}

//Nearest Neighbor Heuristic
int NNN(vector<vector<int>> &cost, int size){
    vector<int> prevDes(size + 1, 0);
    vector<bool> isVisited(size, false);
    int upperBound = INF;
    for(int startDes = 0; startDes < size; startDes++){
        int currDes = startDes;
        int nextDes = startDes;
        int sumCost = 0;
        int cntDes = 0;
        isVisited[startDes] = true;
        while(cntDes < size - 1){
            int minCost = INF;
            for (int i = 0; i < size; i++){
                if (isVisited[i]) continue;
                if (cost[currDes][i] < minCost) nextDes = i;
                minCost = min(cost[currDes][i], minCost); 
            }
            sumCost += minCost;
            isVisited[nextDes] = true;
            prevDes[cntDes] = currDes;
            currDes = nextDes;
            cntDes++;
        }
        prevDes[size - 1] = currDes;
        prevDes[size] = startDes;
        sumCost += cost[currDes][startDes];
        upperBound = min(upperBound, sumCost);
        // cout << sumCost<<endl;
        // printMtrx(prevDes, size + 1);
        fill(isVisited.begin(), isVisited.end(), false);
    }
    return upperBound;
}

void vectorCopy(vector<int> original, vector<int> copy, int len){
    for(int id = 0; id < len; id++){
        copy[id] = original[id];
    }
}

int two_opt(vector<int> &prevDes, vector<vector<int>> &cost, int size){
    vector<int> copy(size);
    int upperBound = INF;
    

}

void matrxReduction(vector<vector<int>> &cost, int size){
    for (int row = 0; row < size; row ++){
        for (int col = 0; col < size; col ++){
            u[row] = min(u[row], cost[row][col]); 
        }
        sum_u += u[row];
    }
    for (int col = 0; col < size; col++){
        for(int row = 0; row < size; row++){
            v[col] = min(v[col], cost[row][col] - u[row]);
        }
        sum_v += v[col];
    }
}

void dfs(int dstOrder, int currDst, int currCost, vector<vector<int>> &cost, int size){
    if (dstOrder == size - 1){
        int tmpCost = currCost + cost[currDst][0] - u[currDst] - v[0];
        upperBound = min(upperBound, tmpCost);

    }
    for (int dst = 0; dst < size; dst++){
        if (isVisited[dst]) continue;
        int tmpCost = currCost + cost[currDst][dst] - u[currDst] - v[dst];
        if (tmpCost >= upperBound) continue;
        isVisited[dst] = true;
        dfs(dstOrder + 1, dst, tmpCost, cost, size);
        isVisited[dst] = false;
    }
    
}

class Solution {
    
  public:
    int tsp(vector<vector<int>>& cost) {
        int size = cost.size();
        u = vector<int> (size, INF);
        v = vector<int> (size, INF);
        upperBound = NNN(cost, size);
        matrxReduction(cost, size);
        isVisited = vector<bool> (size, false);
        isVisited[0] = true;
        dfs(0, 0, sum_u + sum_v, cost, size);
        return upperBound;
    }
};


int main(){
    int n; cin >> n;
    vector<vector<int>> cost(n, vector<int>(n, 0));
    for (int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            cin >> cost[i][j];
        }
        cost[i][i] = INF;
    }
    Solution sol;
    int res = sol.tsp(cost);
    cout << res <<endl;
}