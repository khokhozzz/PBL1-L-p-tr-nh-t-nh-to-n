#include <bits/stdc++.h>
#define INF INT_MAX
using namespace std;


int hungarianAlgorithm(vector<vector<int>> &cost, int size, vector<int> u, vector <int> v){
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
            /*
            tmpJob = match[prevJob];
            currJob = way[prevJob];
            match[prevJob] = match[currJob];
            preJob = currJob
            */
        }
        fill(isAssigned.begin(), isAssigned.end(), false);
        fill(didAssigned.begin(), didAssigned.end(), false);
        fill(slack.begin(), slack.end(), INF);
        fill(way.begin(), way.end(), 0);
        
    }
    return res;
}

// class Solution {
// public:
//     int assignmentProblem(int Arr[], int N) {
//         vector<int> u(N, 0);
//         vector<int> v(N, 0);
//         vector<vector<int>> C(N, vector<int>(N + 1, 0));
        
//         // Bung dữ liệu từ mảng 1 chiều sang ma trận 2 chiều
//         for (int i = 0; i < N; i++) {
//             for (int j = 0; j < N; j++) {
//                 // Ánh xạ công thức 1D sang 2D
//                 // Dùng i + 1 và j + 1 để dữ liệu thực sự bắt đầu từ ô C[1][1]
//                 C[i][j] = Arr[i * N + j];
//             }
//         }

//         // Lúc này ma trận C đã ở đúng form chuẩn. 
//         // Ném C và N vào hàm xử lý Hungarian của bạn tại đây:
//         // return solveHungarian(C, N);
        
//         return hungarianAlgorithm(C, N, u, v); 
//     }
// };

int main(){
    int n; cin >> n;
    vector<int> u(n, 0);
    vector<int> v(n, 0);
    vector<vector<int>> cost(n, vector<int>(n, 0));
    for (int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            cin >> cost[i][j];
        }
    }
    cout << hungarianAlgorithm(cost, n, u, v) << endl;
}