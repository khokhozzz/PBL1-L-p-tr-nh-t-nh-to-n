#include <bits/stdc++.h>
#include <chrono>

#define INF (INT_MAX - 10000)

using namespace std;
using namespace std::chrono;

vector<int> u; vector<int> v;
int upperBound = INF;
int sum;
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

#include <iostream>
#include <vector>

using namespace std;

void findAndPrintSubtours2(const vector<int>& next_node, int size) {
    vector<bool> visited(size, false);
    vector<vector<int>> subtours;

    // Duyệt qua từng đỉnh để gom rác... à nhầm, gom chu trình
    for (int i = 0; i < size; i++) {
        if (!visited[i]) {
            vector<int> cycle;
            int curr = i;
            
            // Lần mò theo đường đi cho đến khi đụng đỉnh đã lướt qua
            while (!visited[curr]) {
                visited[curr] = true;
                cycle.push_back(curr);
                curr = next_node[curr];
            }
            subtours.push_back(cycle);
        }
    }

    // Xuất kết quả
    cout << "Tong so chu trinh con: " << subtours.size() << "\n";
    for (int i = 0; i < subtours.size(); i++) {
        cout << "Chu trinh " << i + 1 << ": ";
        for (int node : subtours[i]) {
            cout << node << " -> ";
        }
        cout << subtours[i][0] << " (Done)\n"; // Vòng lại điểm đầu tiên
    }
}

void findAndPrintSubtours(const vector<int>& match, int size) {
    // Tạo mảng chỉ đường: đỉnh i sẽ đi tới đỉnh next_node[i]
    vector<int> next_node(size, -1);
    
    // Ánh xạ lại mảng match của bạn. 
    // Theo code của bạn: match[j + 1] = i tức là i được gán cho j.
    for (int j = 0; j < size; j++) {
        if (match[j + 1] != -1) {
            next_node[match[j + 1]] = j; 
        }
    }

    vector<bool> visited(size, false);
    vector<vector<int>> subtours;

    // Duyệt qua từng đỉnh để gom rác... à nhầm, gom chu trình
    for (int i = 0; i < size; i++) {
        if (!visited[i]) {
            vector<int> cycle;
            int curr = i;
            
            // Lần mò theo đường đi cho đến khi đụng đỉnh đã lướt qua
            while (!visited[curr]) {
                visited[curr] = true;
                cycle.push_back(curr);
                curr = next_node[curr];
            }
            subtours.push_back(cycle);
        }
    }

    // Xuất kết quả
    cout << "Tong so chu trinh con: " << subtours.size() << "\n";
    for (int i = 0; i < subtours.size(); i++) {
        cout << "Chu trinh " << i + 1 << ": ";
        for (int node : subtours[i]) {
            cout << node << " -> ";
        }
        cout << subtours[i][0] << " (Done)\n"; // Vòng lại điểm đầu tiên
    }
}


int solveHungarian_algorithm(vector<vector<int>> &cost, int size){
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
    // findAndPrintSubtours(match, size);
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

int countCycles(const vector<int>& assign) {
    int n = assign.size();
    vector<bool> visited(n, false);
    int cycles = 0;

    for (int i = 0; i < n; ++i) {
        if (!visited[i]) {
            cycles++;
            int current = i;
            // Duyệt theo các cạnh để hết 1 chu trình
            while (!visited[current]) {
                visited[current] = true;
                current = assign[current];
            }
        }
    }
    return cycles;
}

void findMinCycleAssignment(int row, int n, const vector<vector<int>>& reducedMatrix, 
                            vector<int>& currentAssign, vector<bool>& colUsed, 
                            int& minCycles, vector<int>& bestAssign) {
    if (row == n) {
        int currentCycles = countCycles(currentAssign);
        if (currentCycles < minCycles) {
            minCycles = currentCycles;
            bestAssign = currentAssign;
        }
        return;
    }

    for (int col = 0; col < n; ++col) {
        // Chỉ chọn những ô có giá trị 0 và cột chưa được sử dụng
        if (!colUsed[col] && reducedMatrix[row][col] == 0) {
            colUsed[col] = true;
            currentAssign[row] = col;
            
            findMinCycleAssignment(row + 1, n, reducedMatrix, currentAssign, colUsed, minCycles, bestAssign);
            
            // Trả lại trạng thái cũ
            colUsed[col] = false;
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
        sum = solveHungarian_algorithm(cost, size);
        // cout << "Can duoi: " << sum << endl;
        // cout << "Ket qua: " << upperBound << endl;
        // cout << "Chenh lech: tang " << (1.0 - (double)sum/(double)upperBound) * 100 << "%\n";
        isVisited = vector<bool> (size, false);
        isVisited[0] = true;
        dfs(0, 0, sum, cost, size);
        return upperBound;
    }
};


void mtrxReduction(vector<vector<int>> &cost, int size){
    for (int i = 0; i < size; i++){
        for (int j = 0; j < size; j++){
            cost[i][j] -= u[i] + v[j];
        }
    }
}

void mtrxPrint(vector<vector<int>> &cost, int size){
    for (int i = 0; i < size; i++){
        for (int j = 0; j < size; j++){
            if (cost[i][j] >= INF/2) cout << setw(3) << "INF ";
            else
            cout << setw(3) << cost[i][j] << ' ';
        }
        cout << endl;
    }
}

int main(){
    freopen("experiments/gentest/output.txt", "r", stdin);
    int n; cin >> n;
    vector<vector<int>> cost(n, vector<int>(n, 0));
    for (int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            cin >> cost[i][j];
        }
        cost[i][i] = INF;
    }
    duration<double, milli> duration;
    auto start = high_resolution_clock::now();
    for (int i = 1; i<= 1; i++){
        Solution sol;
        int res = sol.tsp(cost);
        cout << res <<endl;
        auto tmp = high_resolution_clock::now();
        duration = tmp - start;
        if (duration.count() > 33000) break;
    }
    auto end = high_resolution_clock::now();
    duration = end - start;
    cout << "Can duoi: " << sum << endl;
    cout << "Ket qua: " << upperBound << endl;
    cout << "Chenh lech: tang " << (1.0 - (double)sum/(double)upperBound) * 100 << "%\n";
    cout << "Thoi gian thuc thi: " << duration.count() / 3 << " ms\n";
    mtrxReduction(cost, n);
    vector<int> currentAssign(n, -1);
    vector<int> bestAssign(n, -1);
    vector<bool> colUsed(n, false);
    int minCycles = n + 1;
    findMinCycleAssignment(0, n, cost, currentAssign, colUsed, minCycles, bestAssign);
    cout << "So chu trinh con it nhat: " << minCycles << "\n";
    findAndPrintSubtours2(bestAssign, n);
    // mtrxPrint(cost, n);
}