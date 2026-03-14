#include <iostream>
#include <vector>
#include "nearest_neighbor_heuristic.h"

#define INF (INT_MAX - 100000)

using namespace std;

class hungarian_alg : public NNH{
protected: 
    vector<int> u, v;
private:
    void __solveDfs(int dstOrder, int currDst, int currCost, vector<vector<int>> &cost, int size){
        if (dstOrder == size - 1){
            upperbound = min(upperbound, currCost + cost[currDst][0] - u[currDst] - v[0]);
            return;
        }
        for (int dst = 0; dst < size; dst++){
            if (isVisited[dst]) continue;
            if (currCost + cost[currDst][dst] - u[currDst] - v[dst] >= upperbound) continue;
            isVisited[dst] = true;
            __solveDfs(dstOrder + 1, dst, currCost + cost[currDst][dst] - u[currDst] - v[dst], cost, size);
            isVisited[dst] = false;
        }
        
    }

public:
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

                //Vì chỉ cần biết có người nào tham gia vào chuỗi để tính u[]
                //Cho nên ta thay vì tạo mới một mảng đánh 'true' đối với ai tham gia vào chuỗi
                //Thì ta có thể tận dụng isAssigned[] để lưu người từng làm công việc này thay vì chỉ lưu trạng thái công việc
                //Vì số bit của 1 biến bool == int nên là sẽ không có sự gia tăng về bộ nhớ, ngược lại còn tiết kiệm hơn vì đỡ được 1 mảng.
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
    

    int solveTsp_hungarian_alg(vector<vector<int>> &cost, bool usingNNH){
        len = cost.size();
        isVisited = vector<bool>(len);
        u = vector<int> (len, 0);
        v = vector<int> (len, 0);
        upperbound = (usingNNH ? solveUpperboundNNH(cost, len) : INF);
        int lowerbound = solveHungarian_algorithm(cost, len, u, v);
        __solveDfs(0, 0, lowerbound, cost, len);
        return upperbound;
    }

};