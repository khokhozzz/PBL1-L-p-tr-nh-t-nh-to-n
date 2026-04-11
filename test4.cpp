#include <bits/stdc++.h>
#define INF (INT_MAX - 10000) // Chống tràn số
using namespace std;


vector<int> u; vector<int> v;
int upperBound;
vector<int> isVisited;
int sum;


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


int solveHungarian_algorithm(vector<vector<int>> &cost, int size, vector<int> &u, vector<int> &v){
    vector<int> match (size + 1, -1);
    vector<int> way (size + 1, 0);
    vector<int> slack (size, INF);
    vector<int> isAssigned (size, -1);

    int res = 0;
    for (int row = 0; row < size; row ++){
        int jobSeeker = row;
        int prevMinCostJob = 0;
        match[0] = jobSeeker;
        while (jobSeeker != -1){
            int minCost = INF;
            int minCostJob = 0;
            for (int job = 0 ; job < size; job++){
                if (isAssigned[job] > -1) continue;
                if (cost[jobSeeker][job] - u[jobSeeker] - v[job] < slack[job]){
                    slack[job] = cost[jobSeeker][job] - u[jobSeeker] - v[job];
                    way[job + 1] = prevMinCostJob;
                }
                if (slack[job] < minCost){
                    minCost = slack[job];
                    minCostJob = job + 1;
                }
            }
            u[row] += minCost;
            res += minCost;
            for(int job = 0; job < size; job++){
                if (isAssigned[job] > -1){
                    v[job] -= minCost;
                    u[isAssigned[job]] += minCost;
                }
                else{
                    slack[job] -= minCost;
                }
            }
            prevMinCostJob = minCostJob;
            jobSeeker = isAssigned[minCostJob - 1] = match[minCostJob];
        }
        int currJob = prevMinCostJob;
        int prevJob;
        while(match[0] != -1){
            prevJob = way[currJob];
            match[currJob] = match[prevJob];
            match[prevJob] = -1;
            currJob = prevJob;
        }
        fill(isAssigned.begin(), isAssigned.end(), -1);
        fill(slack.begin(), slack.end(), INF);
    }
    return res;
}

void transfer(vector<bool> &isVisited, vector<int> &isAssigned, int size){
    for (int i = 0; i < size; i++){
        if (isVisited[i] == true) isAssigned[i] = -2;
        else isAssigned[i] = -1;
    }
}

int Hungarian_Node(vector<vector<int>> &cost, int size, vector<int> &isVisted){
    vector<int> match (size + 1, -1);
    vector<int> way (size + 1, 0);
    vector<int> slack (size, INF);
    vector<int> isAssigned (size, -1);
    vector<int> u (size, 0);
    vector<int> v (size, 0);
    int res = sum;
    for (int row = 0; row < size; row ++){
        // if (isVisited[row]) continue;
        int jobSeeker = row;
        int prevMinCostJob = 0;
        match[0] = jobSeeker;
        while (jobSeeker != -1){
            int minCost = INF;
            int minCostJob = 0;
            if (isVisited[jobSeeker] != -1){
                int job = isVisited[jobSeeker];
                if (isAssigned[job] == -1){
                    slack[job] = cost[jobSeeker][job];
                    way[job + 1] = 0;
                    minCost = slack[job];
                    minCostJob = job + 1;
                }
            }
            else{
            for (int job = 0 ; job < size; job++){
                if (isAssigned[job] > -1) continue;
                if (isVisited[job] != -1){
                    continue;
                }
                if (cost[jobSeeker][job] - u[jobSeeker] - v[job] < slack[job]){
                    slack[job] = cost[jobSeeker][job] - u[jobSeeker] - v[job];
                    way[job + 1] = prevMinCostJob;
                }
                if (slack[job] < minCost){
                    minCost = slack[job];
                    minCostJob = job + 1;
                }
            }}
            u[row] += minCost;
            res += minCost;
            for(int job = 0; job < size; job++){
                if (isAssigned[job] > -1){
                    v[job] -= minCost;
                    u[isAssigned[job]] += minCost;
                }
                else{
                    slack[job] -= minCost;
                }
            }
            prevMinCostJob = minCostJob;
            jobSeeker = isAssigned[minCostJob - 1] = match[minCostJob];
        }
        int currJob = prevMinCostJob;
        int prevJob;
        while(match[0] != -1){
            prevJob = way[currJob];
            match[currJob] = match[prevJob];
            match[prevJob] = -1;
            currJob = prevJob;
        }
        fill(isAssigned.begin(), isAssigned.end(), -1);
        fill(slack.begin(), slack.end(), INF);
    }
    return res;
}

void dfs(int dstOrder, int currDst, int currCost, vector<vector<int>> &cost, int size){
    if (dstOrder == size - 1){
        int tmpCost = currCost + cost[currDst][0] + sum;
        upperBound = min(upperBound, tmpCost);
        return;
    }
    int tmpCost2;
    for (int dst = 1; dst < size; dst++){
        if (isVisited[dst] != -1) continue;
        isVisited[dst] = dstOrder;
        int tmpCost = currCost + cost[currDst][dst];
        if (tmpCost + sum >= upperBound || (tmpCost2 = tmpCost + Hungarian_Node(cost, size, isVisited)) >= upperBound){
            isVisited[dst] = -1;
            continue;
        }
        dfs(dstOrder + 1, dst, tmpCost, cost, size);
        isVisited[dst] = -1;
    }
    
}

void mtrxReduction(vector<vector<int>>& cost, vector<int>& u, vector<int>& v, int size){
    for (int row = 0; row < size; row ++){
        for (int col = 0; col < size; col ++){
            cost[row][col] -= u[row] + v[col];
        }
    }
}

class Solution {
    
  public:
    int tsp(vector<vector<int>>& cost) {
        int size = cost.size();
        u = vector<int> (size, 0);
        v = vector<int> (size, 0);
        upperBound = NNN(cost, size);
        sum = solveHungarian_algorithm(cost, size, u, v);
        mtrxReduction(cost, u, v, size);
        isVisited = vector<int> (size, -1);
        isVisited[0] = -1;
        dfs(0, 0, 0, cost, size);
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
//gugugaga