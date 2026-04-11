#include <bits/stdc++.h>
using namespace std;

const int INF = 1e8; // Đủ lớn để không tràn khi cộng

int N;
vector<vector<int>> origCost;
vector<vector<int>> reducedCost; // Ma trận rút gọn dùng cho DFS
int upperBound = INF;
int rootBound = 0; // Chính là biến 'sum' của bạn

vector<int> pathNext; 
vector<bool> isVisitedNode;

// =====================================================================
// Heuristic khởi đầu bằng Nearest Neighbor (Tính trên ma trận gốc)
// =====================================================================
int getNearestNeighborBound() {
    vector<bool> vis(N, false);
    int curr = 0;
    int cost = 0;
    vis[0] = true;
    for (int step = 1; step < N; ++step) {
        int next_node = -1;
        int min_edge = INF;
        for (int i = 0; i < N; ++i) {
            if (!vis[i] && origCost[curr][i] < min_edge) {
                min_edge = origCost[curr][i];
                next_node = i;
            }
        }
        if (next_node == -1) return INF;
        cost += min_edge;
        vis[next_node] = true;
        curr = next_node;
    }
    cost += origCost[curr][0];
    return cost;
}

// =====================================================================
// Hàm Hungarian Gốc: Chạy 1 lần duy nhất ở vạch xuất phát
// Nhiệm vụ: Sinh ra u, v để tạo Ma Trận Rút Gọn (Reduced Matrix)
// =====================================================================
int solveHungarianRoot(vector<int>& u_out, vector<int>& v_out, vector<int>& matchOut) {
    int n = N;
    vector<int> u(n + 1, 0), v(n + 1, 0), p(n + 1, 0), way(n + 1, 0);
    
    for (int i = 1; i <= n; ++i) {
        p[0] = i;
        int j0 = 0;
        vector<int> minv(n + 1, INF);
        vector<char> used(n + 1, false);
        
        do {
            used[j0] = true;
            int i0 = p[j0], delta = INF, j1 = 0;
            for (int j = 1; j <= n; ++j) {
                if (!used[j]) {
                    int cur = origCost[i0 - 1][j - 1] - u[i0] - v[j];
                    if (cur < minv[j]) { minv[j] = cur; way[j] = j0; }
                    if (minv[j] < delta) { delta = minv[j]; j1 = j; }
                }
            }
            for (int j = 0; j <= n; ++j) {
                if (used[j]) { u[p[j]] += delta; v[j] -= delta; } 
                else { minv[j] -= delta; }
            }
            j0 = j1;
        } while (p[j0] != 0);
        
        do {
            int j1 = way[j0]; p[j0] = p[j1]; j0 = j1;
        } while (j0 != 0);
    }
    
    matchOut.assign(n, -1);
    for (int j = 1; j <= n; ++j) matchOut[p[j] - 1] = j - 1;
    
    u_out.assign(n, 0); v_out.assign(n, 0);
    for(int i = 1; i <= n; i++) u_out[i-1] = u[i];
    for(int j = 1; j <= n; j++) v_out[j-1] = v[j];
    
    int total_cost = 0;
    for (int j = 1; j <= n; ++j) {
        int r = p[j] - 1, c = j - 1;
        if(origCost[r][c] >= INF / 2) return INF;
        total_cost += origCost[r][c];
    }
    return total_cost;
}

// =====================================================================
// Hàm Chi Phí Ảo: Đánh lừa Hungarian_Node, không tốn RAM copy ma trận
// =====================================================================
inline int getVirtualCost(int r, int c, int currDst, int numEdges) {
    // Ép duyên: Bắt buộc đi theo pathNext
    if (pathNext[r] != -1 && pathNext[r] != c) return INF;
    // Chống chu trình con: Cấm về nhà sớm
    if (numEdges < N - 1 && r == currDst && c == 0) return INF;
    // Dùng ma trận ĐÃ RÚT GỌN
    return reducedCost[r][c]; 
}

// =====================================================================
// Hungarian Node (O(N^3)): Lớp khiên động chạy trên Ma trận rút gọn
// =====================================================================
int solveVirtualHungarian(int currDst, int numEdges) {
    int n = N;
    vector<int> u(n + 1, 0), v(n + 1, 0), p(n + 1, 0), way(n + 1, 0);
    
    for (int i = 1; i <= n; ++i) {
        p[0] = i;
        int j0 = 0;
        vector<int> minv(n + 1, INF);
        vector<char> used(n + 1, false);
        
        do {
            used[j0] = true;
            int i0 = p[j0], delta = INF, j1 = 0;
            for (int j = 1; j <= n; ++j) {
                if (!used[j]) {
                    int actualCost = getVirtualCost(i0 - 1, j - 1, currDst, numEdges);
                    int cur = actualCost - u[i0] - v[j];
                    if (cur < minv[j]) { minv[j] = cur; way[j] = j0; }
                    if (minv[j] < delta) { delta = minv[j]; j1 = j; }
                }
            }
            for (int j = 0; j <= n; ++j) {
                if (used[j]) { u[p[j]] += delta; v[j] -= delta; } 
                else { minv[j] -= delta; }
            }
            j0 = j1;
        } while (p[j0] != 0);
        
        do {
            int j1 = way[j0]; p[j0] = p[j1]; j0 = j1;
        } while (j0 != 0);
    }
    
    int total_cost = 0;
    for (int j = 1; j <= n; ++j) {
        int r = p[j] - 1, c = j - 1;
        int actualCost = getVirtualCost(r, c, currDst, numEdges);
        if(actualCost >= INF / 2) return INF;
        total_cost += actualCost;
    }
    return total_cost;
}

// =====================================================================
// Hàm DFS: Tích hợp 2 Tầng Cận Dưới (Cascading Bounds)
// =====================================================================
// =====================================================================
// Hàm DFS: Tích hợp 2 Tầng Cận Dưới (Cascading Bounds)
// =====================================================================
void dfs(int depth, int currDst, int currCostReduced) {
    // 🛡️ LỚP KHIÊN TĨNH O(1): Nếu chi phí rút gọn + bound gốc đã quá đắt -> Cắt
    if (currCostReduced + rootBound >= upperBound) return;

    if (depth == N - 1) {
        int finalCost = currCostReduced + rootBound + reducedCost[currDst][0];
        upperBound = min(upperBound, finalCost);
        return;
    }

    for (int dst = 1; dst < N; ++dst) {
        if (isVisitedNode[dst]) continue;
        
        int nextCostBase = currCostReduced + reducedCost[currDst][dst];
        
        // 🛡️ LỚP KHIÊN TĨNH O(1) (Cắt trước khi rẽ nhánh)
        if (nextCostBase + rootBound >= upperBound) continue;
        
        // Bắt đầu ép duyên
        isVisitedNode[dst] = true;
        pathNext[currDst] = dst; 
        
        // 🛡️ LỚP KHIÊN ĐỘNG O(N^3): Nhánh này lọt qua khiên tĩnh, soi kĩ bằng Hungarian
        int boundCost = 0;
        if (depth < N / 3) { // Tùy chỉnh con số này (ví dụ: N/3 hoặc N/4)
            boundCost = solveVirtualHungarian(dst, depth + 1);
        }
        
        // SỬA BUG ĐẾM TRÙNG Ở ĐÂY: 
        // boundCost ĐÃ TÍNH LUÔN CẢ nextCostBase (vì đường đi bị ép trong ma trận)
        // Nên chỉ cần lấy rootBound + boundCost
        if (boundCost < INF / 2 && rootBound + boundCost < upperBound) {
            dfs(depth + 1, dst, nextCostBase);
        }
        
        // Backtrack
        isVisitedNode[dst] = false;
        pathNext[currDst] = -1;
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    if (!(cin >> N)) return 0;

    origCost.assign(N, vector<int>(N));
    for (int i = 0; i < N; ++i) {
        for(int j = 0; j < N; ++j) {
            cin >> origCost[i][j];
            if (i == j) origCost[i][j] = INF; // Chống tự kỷ
        }
    }

    upperBound = getNearestNeighborBound();

    // 1. Chạy Hungarian Gốc
    vector<int> u_root, v_root, initialMatch;
    rootBound = solveHungarianRoot(u_root, v_root, initialMatch);

    if (rootBound >= INF) {
        cout << "Khong co duong di!\n";
        return 0;
    }

    // Check nhân phẩm: Có đúng 1 chu trình không?
    int countNodes = 0, curr = 0;
    while(true) {
        curr = initialMatch[curr]; countNodes++;
        if (curr == 0 || countNodes > N) break;
    }
    if (countNodes == N) {
        cout << rootBound << "\n";
        return 0;
    }

    // 2. Chuẩn bị Ma trận rút gọn (Reduced Matrix)
    reducedCost.assign(N, vector<int>(N, 0));
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (origCost[i][j] < INF / 2) {
                reducedCost[i][j] = origCost[i][j] - u_root[i] - v_root[j];
            } else {
                reducedCost[i][j] = INF;
            }
        }
    }

    // 3. Khởi động DFS với ma trận rút gọn
    pathNext.assign(N, -1);
    isVisitedNode.assign(N, false);
    isVisitedNode[0] = true;
    
    dfs(0, 0, 0);

    cout << upperBound << "\n";
    return 0;
}