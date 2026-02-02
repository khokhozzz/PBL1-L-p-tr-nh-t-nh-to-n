#include <bits/stdc++.h>

using namespace std;
class Solution {
    int SumMin = 0;
    int Res = 9999999;
    vector <int> MinRow;
    int Len;
    vector<bool> Check;
    void Sol(int id, int CurrDes, int CurrSum, vector<vector<int>>& cost){
        if (id == Len - 1){
            CurrSum += cost[CurrDes][0];
            Res = min(CurrSum, Res);
            return;
        }
        int Tmp;
        SumMin -= MinRow[CurrDes];
        for (int i = 1; i < Len; i++){
            if (Check[i]) continue;
            // if (id == 1 and CurrDes == 4){
            //     cout << "1";
            // }
            Tmp = CurrSum + cost[CurrDes][i];
            if (Tmp + SumMin > Res) continue; ///
            Check[i] = true;
            Sol(id + 1, i, Tmp, cost);
            Check[i] = false;
        }
        SumMin += MinRow[CurrDes];
    }
  public:
    int tsp(vector<vector<int>>& cost) {
        Len = cost.size();
        MinRow = vector <int> (Len, 9999999);
        Check = vector<bool> (Len, false);
        int Min;
        for (int i = 0; i < Len; i++){
            Min = 99999999;
            for(int j = 0; j < Len; j++){
                if (j != i){
                    MinRow[i] = min(MinRow[i], cost[i][j]);
                }
            }
            SumMin += MinRow[i];
        }
        Sol(0, 0, 0, cost);
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
    }
    Solution sol;
    int res = sol.tsp(cost);
    cout << res <<endl;
}