#include <iostream>
#include <vector>

#define INF (INT_MAX - 100000)

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
public:
    int solveTsp_naive_bound(vector<vector<int>>& cost){
        len = cost.size();
        res = INF;
        isVisited = vector<bool>(len, false);
        __solveDfs(0, 0, 0, cost);
        return res;
    }
};

/*
Chỉ gọi hàm tsp, không sử dụng bất cứ một hàm hay một biến gì khác từ bên đây
*/