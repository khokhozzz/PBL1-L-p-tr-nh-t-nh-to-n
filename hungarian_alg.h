#pragma once
#include <iostream>
#include <vector>
#include <iomanip>
#include "nearest_neighbor_heuristic.h"

#define INF (INT_MAX - 100000)

using namespace std;

class hungarian_alg : public NNH{
protected: 
    vector<int> u, v;
private:
    void __solveDfs(int dstOrder, int currDst, int currCost, vector<vector<int>> &cost, int size, vector<int>& path){
        if (dstOrder == size - 1){
            int finalCost = currCost + cost[currDst][0] - u[currDst] - v[0];
            if (finalCost < upperbound) {
                upperbound = finalCost;
                bestPath = path;
                bestPath.push_back(0);
            }
            return;
        }
        for (int dst = 1; dst < size; dst++){
            if (isVisited[dst]) continue;
            if (currCost + cost[currDst][dst] - u[currDst] - v[dst] >= upperbound) continue;
            isVisited[dst] = true;
            path.push_back(dst);
            
            __solveDfs(dstOrder + 1, dst, currCost + cost[currDst][dst] - u[currDst] - v[dst], cost, size, path);
            
            path.pop_back();
            isVisited[dst] = false;
        }
    }

    void __illuDfs(int dstOrder, int currDst, int currCost, vector<vector<int>> &cost, int size, vector<int>& path){
        if (dstOrder == size - 1){
            int finalCost = currCost + cost[currDst][0] - u[currDst] - v[0];
            cout << CYAN << "[-] Step (Hungarian DFS): " << RESET << "Path = [";
            for (int i = 0; i < path.size(); i++) cout << path[i] << (i < path.size() - 1 ? " -> " : "");
            cout << "]";
            cout << " Current Cost = " << YELLOW << currCost << RESET << "\n";
            cout << "    Cost to city " << YELLOW << "0" << RESET << ": " << cost[currDst][0] << endl; 
            if (finalCost < upperbound) {
                cout << GREEN << "    -> NEW BEST PATH! Adjusted Cost: " << finalCost << RESET << "\n";
                upperbound = finalCost;
                bestPath = path;
                bestPath.push_back(0);
            } else {
                cout << "    -> Path complete. Adjusted Cost " << finalCost << " (not better)\n";
            }
            return;
        }

        for (int dst = 1; dst < size; dst++){
            if (isVisited[dst]) continue; 

            cout << CYAN << "\n[-] Step (Hungarian DFS): " << RESET << "Path = [";
            for (int i = 0; i < path.size(); i++) cout << path[i] << (i < path.size() - 1 ? " -> " : "");
            cout << "], Adj Cost = " << YELLOW << currCost << RESET << ", Bound = " << MAGENTA << (upperbound == INF ? "INF" : to_string(upperbound)) << RESET << "\n";

            cout << "    Standing at city " << currDst << " checking city " << dst << ", adj row: [";
            for (int i = 0; i < size; i++) {
                if (cost[currDst][i] >= INF / 2 || isVisited[i] || (i == 0 && dstOrder != size - 1)) cout << "INF";
                else cout << cost[currDst][i] - u[currDst] - v[i];
                if (i < size - 1) cout << ", ";
            }
            cout << "]\n";

            int adjCost = cost[currDst][dst] - u[currDst] - v[dst];
            if (currCost + adjCost >= upperbound) {
                cout << RED << "    -> PRUNED at city " << dst << "! (Cost " << currCost + adjCost << " >= Bound " << upperbound << ")" << RESET << "\n";
                continue;
            }
            
            cout << "    -> Exploring city " << YELLOW << dst << RESET << " with adj cost " << adjCost << "\n";
            isVisited[dst] = true;
            path.push_back(dst);
            
            __illuDfs(dstOrder + 1, dst, currCost + adjCost, cost, size, path);
            
            path.pop_back();
            isVisited[dst] = false;
        }
    }

    void printMatrixUV(const vector<vector<int>>& cost, int size, const vector<int>& u, const vector<int>& v) {
        cout << "\n    " << setw(6) << "";
        for(int j=0; j<size; j++) cout << BLUE << setw(5) << j << RESET;
        cout << "  | " << MAGENTA << "u" << RESET << "\n";
        
        for(int i=0; i<size; i++) {
            cout << "    " << BLUE << setw(4) << i << " |" << RESET;
            for(int j=0; j<size; j++) {
                if(cost[i][j] >= INF/2) cout << setw(5) << "INF";
                else cout << setw(5) << cost[i][j];
            }
            cout << "  | " << MAGENTA << u[i] << RESET << "\n";
        }
        cout << "    " << setw(6) << "----";
        for(int j=0; j<size; j++) cout << "-----";
        cout << "\n    " << MAGENTA << setw(4) << "v" << " |" << RESET;
        for(int j=0; j<size; j++) cout << MAGENTA << setw(5) << v[j] << RESET;
        cout << "\n\n";
    }

public:
    int solveHungarian_algorithm(vector<vector<int>> &cost, int size, vector<int> &u, vector<int> &v){
        /*
        Lưu ý: mảng match và way chơi index từ 1 đến n. 
        index = 0 dùng để lưu giá trị tạm,
        tức là thay vì tạo hai biến mới và cho hai mảng này dùng index từ 0 đến n - 1 
        thì ta dùng id = 0 làm biến tạm và index từ 1 đến n để dễ quản lý vòng lặp

        Về cơ bản, đối với mảng match và way sẽ không tồn tại dst0/job0, mà nó là dst1/job1.
        Nên khi lưu trữ kết quả vào mảng match và way thì ta thường cộng 1 vào index,
        vì vốn dĩ bài toán xử lí từ index 0, có mỗi hai cái mảng này là dùng index 1
        */
        vector<int> match (size + 1, -1);//match[i]: việc i do người match[i] làm
        vector<int> way (size + 1, 0);//way[i]: trước khi được phân làm việc i thì người match[way[i]] đang làm việc way[i]
        //Có một tính chất của mảng way và match: các phần tử way[i] bằng nhau thì chỉ vào đúng một người duy nhất
        //người đó là match[way[i]]. Cần phải hiểu là việc có nhiều phần tử way[i] bằng nhau, tức là người đó có xác xuất
        //được phân một trong các việc i mới
        /*
        Ví dụ: (dùng index 1 với job, người thì vẫn dùng 0)
            Way:   [0, 0, 0, 2, 2, 4]
            Match: [3, 2, 0, -1, 1, -1]
            - Người số match[way[i]] = 0 trước khi được làm việc i = 3 hoặc 4 thì đang làm việc way[i] = 2
            - Người số 1 trước khi được làm việc 5 thì đang làm việc 4
        */
        vector<int> slack (size, INF);
        vector<int> isAssigned (size, -1);

        int res = 0;
        for (int row = 0; row < size; row ++){
            int jobSeeker = row;
            int prevMinCostJob = 0;
            match[0] = jobSeeker;
            //Lý do lấy jobSeeker == -1 là điểm dừng: nếu match[minCostJob] == -1 thì tức là chỗ đó không có tranh chấp
            //n job, n người làm thì kiểu gì cũng lòi ra job chưa có người làm, chỉ là lâu hay mau
            while (jobSeeker != -1){
                int minCost = INF;
                int minCostJob = 0;
                for (int job = 0 ; job < size; job++){
                    if (isAssigned[job] > -1) continue;
                    if (cost[jobSeeker][job] - u[jobSeeker] - v[job] < slack[job]){
                        slack[job] = cost[jobSeeker][job] - u[jobSeeker] - v[job];
                        //Lý do gán way[job + 1] = prevMinCostJob: 1. Vì đây là việc bị xung đột
                        //2. Trước khi đổi sang việc job + 1, match[way[job + 1]] đã làm việc gì, và việc đó chính là prevMinCostJob
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
                /*
                minCostJob là job mà ta lựa chọn để phân việc cho jobSeeker
                - Ta thấy rằng tại vị trí nào đó mà match[minCostJob] = -1, tức là việc đó chưa được phân cho ai.
                Vậy thì ta sẽ cho jobSeeker làm việc đó luôn, kết thúc vòng lặp (lý do vì sao điều kiện dừng là JobSeeker == -1)
                - Nếu việc đó đã có người làm, tức là match[minCostJob] != -1, về cơ bản thì ta phải cần
                "tìm việc cho thằng jobSeeker hiện tại (sau khi được gắn với match[minCostJob]) và những jobSeeker trước đó"
                Nhưng thực tế lại không cần làm như vậy,
                ta chỉ cần xem thằng jobSeeker hiện tại có những việc nào chi phí rẻ hơn những thằng trước đó hay không
                Mảng slack đã được sinh ra để làm việc này.
                - Lý do mà ta bỏ qua những ô isAssigned[job] != -1: Vì ta biết chắc rằng job này sẽ có người nhận,
                chỉ đơn giản là người nhận đang tranh chấp lẫn nhau thôi
                */
            }
            //Lý do lấy mốc currJob ở prevMinCostJob/MinCostJob:
            //jobSeeker = isAssigned[minCostJob - 1] = match[minCostJob]; như ta có thể thấy, đây là vị trí chắc chắn có -1.
            //Nhờ đó mà ta có thể mang cái giá trị này về match[0]
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

    int illuHungarian_algorithm(vector<vector<int>> &cost, int size, vector<int> &u, vector<int> &v){
        vector<int> match (size + 1, -1);
        vector<int> way (size + 1, 0);
        vector<int> slack (size, INF);
        vector<int> isAssigned (size, -1);

        int res = 0;
        cout << YELLOW << "\n=== STARTING HUNGARIAN ALGORITHM LOWERBOUND ===" << RESET << "\n";
        
        for (int row = 0; row < size; row ++){
            cout << "\n" << CYAN << ">>> Process Dst (Row): " << row << RESET << "\n";
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

                cout << MAGENTA << "  - City pulled in: " << jobSeeker << RESET << "\n";
                cout << "  - Slack array (before sub): [";
                for (int j = 0; j < size; j++) {
                    if (isAssigned[j] > -1 || slack[j] >= INF/2) cout << "INF";
                    else cout << slack[j];
                    if (j < size - 1) cout << ", ";
                }
                cout << "]\n";
                cout << "  - Found minCost = " << GREEN << minCost << RESET << " at Target City = " << GREEN << minCostJob - 1 << RESET << "\n";

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

                cout << "  - Slack array (after sub):  [";
                for (int j = 0; j < size; j++) {
                    if (isAssigned[j] > -1 || slack[j] >= INF/2) cout << "INF";
                    else cout << slack[j];
                    if (j < size - 1) cout << ", ";
                }
                cout << "]\n";

                prevMinCostJob = minCostJob;
                jobSeeker = isAssigned[minCostJob - 1] = match[minCostJob];
                
                cout << "  - Active assigned cities (isAssigned): [";
                for (int j=0; j<size; j++) cout << (isAssigned[j]>-1 ? to_string(isAssigned[j]) : "-") << (j<size-1?", ":"");
                cout << "]\n";
            }
            
            cout << "\n  [Augmenting Path - Match Swap]\n";
            cout << "  Initial match: [";
            for(int i=0; i<=size; i++) cout << match[i] << (i<size?", ":"");
            cout << "]\n";
            
            int currJob = prevMinCostJob;
            int prevJob;
            while(match[0] != -1){
                prevJob = way[currJob];
                
                cout << "    Swapping... currDst=" << currJob << ", prevDst=" << prevJob << "\n";
                cout << "    Way:   [";
                for(int i=0; i<=size; i++) {
                    if (i == currJob) cout << YELLOW << way[i] << RESET;
                    else cout << way[i];
                    if(i<size) cout << ", ";
                } cout << "]\n";
                
                cout << "    Match: [";
                for(int i=0; i<=size; i++) {
                    if (i == currJob || i == prevJob) cout << GREEN << match[i] << RESET;
                    else cout << match[i];
                    if(i<size) cout << ", ";
                } cout << "]\n";

                match[currJob] = match[prevJob];
                match[prevJob] = -1;
                currJob = prevJob;
            }
            cout << "  Final match for this step: [";
            for(int i=0; i<=size; i++) cout << match[i] << (i<size?", ":"");
            cout << "]\n";

            printMatrixUV(cost, size, u, v);

            fill(isAssigned.begin(), isAssigned.end(), -1);
            fill(slack.begin(), slack.end(), INF);
        }
        cout << GREEN << "=> Lowerbound (sum of minCosts): " << res << RESET << "\n";

        cout << YELLOW << "\n=== FINAL ADJUSTED MATRIX (Cost - u - v) ===" << RESET << "\n";
        cout << "    " << setw(6) << "";
        for(int j=0; j<size; j++) cout << BLUE << setw(5) << j << RESET;
        cout << "\n";
        for(int i=0; i<size; i++) {
            cout << "    " << BLUE << setw(4) << i << " |" << RESET;
            for(int j=0; j<size; j++) {
                if(cost[i][j] >= INF/2) cout << setw(5) << "INF";
                else cout << setw(5) << cost[i][j] - u[i] - v[j];
            }
            cout << "\n";
        }
        cout << "\n";

        return res;
    }
    
    int solveTsp_hungarian_alg(vector<vector<int>> &cost, bool usingNNH){
        len = cost.size();
        bestPath.clear();
        isVisited = vector<bool>(len);
        u = vector<int> (len, 0);
        v = vector<int> (len, 0);
        upperbound = (usingNNH ? solveUpperboundNNH(cost, len) : INF);
        int lowerbound = solveHungarian_algorithm(cost, len, u, v);
        
        vector<int> path = {0};
        __solveDfs(0, 0, lowerbound, cost, len, path);
        
        cout << GREEN << "\n=> OPTIMAL PATH: [";
        for(int i=0; i<bestPath.size(); i++) cout << bestPath[i] << (i<bestPath.size()-1?" -> ":"");
        cout << "]" << RESET << "\n";
        
        return upperbound;
    }

    int illuTsp_hungarian_alg(vector<vector<int>> &cost, bool usingNNH){
        len = cost.size();
        bestPath.clear();
        isVisited = vector<bool>(len, false);
        u = vector<int> (len, 0);
        v = vector<int> (len, 0);
        
        if(usingNNH) {
            cout << YELLOW << "\n[HUNGARIAN + NNH MODE]" << RESET << "\n";
            upperbound = illuUpperboundNNH(cost, len);
        } else {
            cout << YELLOW << "\n[HUNGARIAN ONLY MODE]" << RESET << "\n";
            upperbound = INF;
        }

        int lowerbound = illuHungarian_algorithm(cost, len, u, v);
        
        vector<int> path;
        path.push_back(0);
        cout << YELLOW << "\n=== STARTING DFS WITH HUNGARIAN COST ADJUSTMENTS ===" << RESET << "\n";
        __illuDfs(0, 0, lowerbound, cost, len, path);
        
        cout << GREEN << "\n=> FINAL OPTIMAL PATH: [";
        for(int i=0; i<bestPath.size(); i++) cout << bestPath[i] << (i<bestPath.size()-1?" -> ":"");
        cout << "]" << RESET << "\n";
        cout << GREEN << "=> FINAL MIN COST (Hungarian" << (usingNNH?"+NNH":"") << "): " << upperbound << RESET << "\n";
        
        return upperbound;
    }
};