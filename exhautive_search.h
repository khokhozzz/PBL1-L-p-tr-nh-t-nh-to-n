#include <iostream>
#include <vector>

#define INF (INT_MAX - 100000)

using namespace std;

int res;
int len;
vector<bool> isVisited;

void solveDfs(int cntDst, int currDst, int currSum, vector<vector<int>>& cost){
    if (cntDst == len - 1){
        res = min(currSum + cost[currDst][0], res);
        return;
    }
    for (int dst = 1; dst < len; dst ++){
        if (isVisited[dst]) continue;
        isVisited[dst] = true;
        solveDfs(cntDst + 1, dst, currSum + cost[currDst][dst], cost);
        isVisited[dst] = false;
    }
}

int solveTsp_exhautiveSearch(vector<vector<int>>& cost){
    len = cost.size();
    res = INF;
    isVisited = vector<bool>(len, false);
    solveDfs(0, 0, 0, cost);
    return res;
}

/*
Chỉ gọi hàm tsp, không sử dụng bất cứ một hàm hay một biến gì khác từ bên đây
*/