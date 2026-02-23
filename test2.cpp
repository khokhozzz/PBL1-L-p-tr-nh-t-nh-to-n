#include <bits/stdc++.h>

using namespace std;

class Solution{
    vector <int> u; vector <int> v;
    // int SumMin = 0;
    int Res = 9999999;
    bool endFlag = false;
    vector <int> MinRow;
    int Len;
    vector<int> IsVisited;
    vector<bool> CheckRow;
    vector<bool> CheckCol;

    void printMtrx(vector<vector<int>> &cost){
        for (int i = 0; i < Len; i++)
        {
            for (int j = 0; j < Len; j++)
            {
                printf("%10d", cost[i][j] - u[i] - v[j]);
            }
            printf("\n");
        }
        cout << "______________________________________\n";
    }

    void RecalculateMtrx(vector<vector<int>> &cost){
        int diffVal = 99999999;
        for (int i = 0; i < Len; i ++){
            if (CheckRow[i]) continue;
            for (int j = 0; j < Len; j++){
                if (CheckCol[j] || !(cost[i][j] - u[i] - v[j])) continue;
                diffVal = min(diffVal, cost[i][j] - u[i] - v[j]);
            }
        }
        for (int i = 0; i < Len; i++){
            if (!CheckRow[i]) u[i] += diffVal;
            if (!CheckCol[i]) v[i] -= diffVal;
        }
    }

    void dfs(int id, int currDes, vector<vector<int>>& cost){
        if (id == Len){

        }
        if (id + 1 == Len){
            if (cost[currDes][0]- u[currDes] - v[0] == 0){
                endFlag = true;
            }
            return;
        }

        bool zeroFlag = false;
        for (int i = 0; i < Len; i++){
            if (cost[currDes][i] - u[currDes] - v[i] != 0) continue;
            if (IsVisited[i]){
                CheckCol[i] = true;
                zeroFlag = true;
                continue;
            }
            IsVisited[i] = true;
            dfs(id + 1, i, cost);
            IsVisited[i] = false;
            CheckRow[currDes] = true;
        }
        if (zeroFlag) CheckRow[currDes] = false;
    }
    

  public:
    int tsp(vector<vector<int>>& cost) {
        Len = cost.size();
        u = vector <int> (Len, 0);
        v = vector <int> (Len, 9999999);
        IsVisited = vector<int> (Len, false);
        CheckRow = vector<bool> (Len, false);
        CheckCol = vector<bool> (Len, false);
        // int Min;
        for (int j = 0; j < Len; j++){
            // Min = 99999999;
            for(int i = 0; i < Len; i++){
                if (j != i){
                    v[j] = min(v[j], cost[i][j]);
                }
            }

        }
        IsVisited[0] = true;
        while (!endFlag)
        {
            IsVisited = vector<int> (Len, false);
            CheckRow = vector<bool> (Len, false);
            CheckCol = vector<bool> (Len, false);
            printMtrx(cost);
            dfs(0, 0, cost);
            RecalculateMtrx(cost);
        }
        
        return Res;
    }
};


int main(){
    int n; cin >> n;
    vector<vector<int>> cost(n, vector<int>(n, 0));
    for (int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            cin >> cost[i][j];
        }
        cost[i][i] = 99999999;
    }
    Solution sol;
    int res = sol.tsp(cost);
    cout << res <<endl;
}