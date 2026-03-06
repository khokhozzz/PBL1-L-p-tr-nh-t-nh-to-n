#include <bits/stdc++.h>
#define INF (INT_MAX - 10000)
using namespace std;


vector<int> u; vector<int> v;
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

int hungarianAlgorithm(vector<vector<int>> &cost, int size){
    vector<int> match (size + 1, -1); //match[i] = j: việc i được giao cho người j
    vector<int> way (size + 1, 0); //way[i]: trước khi Ji được phân công thì nó đã thay thế cho J gì
    vector<int> slack (size, INF); //chứa min của việc j trong chuỗi đang xét
    vector<bool> isAssigned (size, false);
    vector<bool> didAssigned (size, false);
    int res = 0;
    for(int id = 0; id < size; id++){
        int jobSeeker = id;
        bool isProcessing = true;
        int prevMinCostJob = 0;
        int tmpSlack;
        match[0] = jobSeeker;
        while(isProcessing){
            // cout << "bug1\n";
            int minCost = INF;
            int minCostJob = 0;
            for (int job = 0; job < size; job++){
                if (isAssigned[job]) continue;
                tmpSlack = cost[jobSeeker][job] - u[jobSeeker] - v[job];
                if (tmpSlack < slack[job]){
                    slack[job] = tmpSlack;
                    way[job + 1] = prevMinCostJob;
                }
                if (slack[job] < minCost){
                    minCost = slack[job];
                    minCostJob = job + 1;
                }
            }
            didAssigned[jobSeeker] = true;
            for(int job = 0; job < size; job++){
                if (isAssigned[job]){
                    v[job] -= minCost;
                    res -= minCost;
                }
                else{
                    slack[job] -= minCost;
                }
                if (didAssigned[job]){
                    u[job] += minCost;
                    res += minCost;
                }
            }
            if (match[minCostJob] != -1){
                isProcessing = true;
                jobSeeker = match[minCostJob];
                isAssigned[minCostJob - 1] = true;
            }
            else{
                // match[minCostJob] = jobSeeker;
                isProcessing = false; 
            }
            prevMinCostJob = minCostJob;
        }
        int prevJob = prevMinCostJob;
        int currJob;
        int tmpJob;
        while(match[0] != -1){
            currJob = way[prevJob];
            match[prevJob] = match[currJob];
            match[currJob] = -1;
            prevJob = currJob;
        }
        fill(isAssigned.begin(), isAssigned.end(), false);
        fill(didAssigned.begin(), didAssigned.end(), false);
        fill(slack.begin(), slack.end(), INF);
        fill(way.begin(), way.end(), 0);
        
    }
    return res;
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
        u = vector<int> (size, 0);
        v = vector<int> (size, 0);
        upperBound = NNN(cost, size);
        int sum = hungarianAlgorithm(cost, size);
        isVisited = vector<bool> (size, false);
        isVisited[0] = true;
        dfs(0, 0, sum, cost, size);
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