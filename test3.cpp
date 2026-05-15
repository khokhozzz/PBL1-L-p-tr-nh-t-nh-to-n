#include <bits/stdc++.h>
#include "nearest_neighbor_heuristic.h"

#define INF (INT_MAX - 100000)
#define TIME_LIMIT 138000 // 2 mins + 15%
#define mtrx vector<vector<int>>

using namespace std;
using namespace std::chrono;

// ĐÃ SỬA: Thêm '&' để truyền tham chiếu
void random_mtrx(mtrx &cost, int size){
    mt19937 rng(random_device{}());
    uniform_int_distribution<> distrib(0, 500);
    for (int i = 0; i < size; i++){
        for (int j = 0; j < size; j++){
            if (i == j) cost[i][j] = INF;
            else cost[i][j] = distrib(rng);
        }
    }
}

class hungarian_alg : public NNH {
public: // Đẩy mấy cái cờ ra public để hàm main dễ truy xuất
    vector<int> u, v;
    bool timeout = false;
    high_resolution_clock::time_point startTime;
    long long final_elapsed = 0; // Biến lưu thời gian chạy chốt hạ

private:
    void __solveDfs(int dstOrder, int currDst, int currCost, vector<vector<int>> &cost, int size){
        if (timeout) return;
        auto now = high_resolution_clock::now();
        auto elapsed = duration_cast<milliseconds> (now - startTime).count();
        if (elapsed > TIME_LIMIT){
            timeout = true;
            return;
        }

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
        // ... (Giữ nguyên logic code Hungarian của ông ở đây) ...
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
    
    int solveTsp_hungarian_alg(vector<vector<int>> &cost, bool usingNNH){
        timeout = false; // Phải reset cờ này cho mỗi testcase mới
        len = cost.size();
        isVisited = vector<bool>(len);
        u = vector<int> (len, 0);
        v = vector<int> (len, 0);
        upperbound = (usingNNH ? solveUpperboundNNH(cost, len) : INF);
        int lowerbound = solveHungarian_algorithm(cost, len, u, v);
        
        startTime = high_resolution_clock::now();
        __solveDfs(0, 0, lowerbound, cost, len);
        auto now = high_resolution_clock::now();
        
        // Lưu thời gian chạy vào biến public
        final_elapsed = duration_cast<milliseconds> (now - startTime).count();
        return upperbound;
    }
};

// ================= MODULE KHẢO SÁT =================
void runBenchmark(int n, int num_tests) {
    int c_05 = 0, c_1 = 0, c_5 = 0, c_10 = 0, c_60 = 0, c_120 = 0, c_fail = 0;

    for (int i = 0; i < num_tests; i++) {
        mtrx cost(n, vector<int>(n, 0));
        random_mtrx(cost, n);

        hungarian_alg sol;
        sol.solveTsp_hungarian_alg(cost, true);

        if (sol.timeout) {
            c_fail++;
        } else {
            long long t = sol.final_elapsed;
            // Đã cộng thẳng 15% tolerance vào ngưỡng thời gian
            if (t <= 575) c_05++;
            else if (t <= 1150) c_1++;
            else if (t <= 5750) c_5++;
            else if (t <= 11500) c_10++;
            else if (t <= 69000) c_60++;
            else if (t <= 138000) c_120++;
            else c_fail++; 
        }
        // In log ra màn hình để biết nó chưa bị treo
        cout << "Running size " << n << " - Test " << i + 1 << "/" << num_tests << "...\r" << flush;
    }

    // Ghi nối (append) vào file output.txt
    ofstream out("output.txt", ios::app);
    out << "=== KET QUA KHAO SAT N = " << n << " (" << num_tests << " testcases) ===\n";
    out << "- < 0.5s  : " << c_05 << "\n";
    out << "- < 1s    : " << c_1 << "\n";
    out << "- < 5s    : " << c_5 << "\n";
    out << "- < 10s   : " << c_10 << "\n";
    out << "- < 1m    : " << c_60 << "\n";
    out << "- < 2m    : " << c_120 << "\n";
    out << "- Cut luon (Failed/Timeout): " << c_fail << "\n";
    out << "========================================\n\n";
    out.close();
    
    cout << "\nHoan thanh khao sat n = " << n << ". Da ghi vao output.txt!\n";
}

int main() {
    // Reset file output.txt trắng tinh trước khi chạy
    ofstream out("output.txt");
    out.close();

    // Setup chạy khảo sát: n = 20, 100 testcases
    runBenchmark(25, 10000); 
    
    // Nếu rảnh máy thì gỡ comment dòng dưới để bào tiếp n = 25
    // runBenchmark(25, 100); 

    return 0;
}