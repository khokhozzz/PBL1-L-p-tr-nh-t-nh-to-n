#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <random>

using namespace std;

int main() {
    int n = 25;
    mt19937 rng(2353); // Thay đổi seed lô tô ở đây
    
    // THÍCH BAO NHIÊU CHU TRÌNH THÌ SỬA Ở ĐÂY (Nên thử 4, 5, 6)
    int num_subtours = 6; 

    vector<vector<int>> matrix(n, vector<int>(n));

    // 1. Phủ mìn rác ngẫu nhiên (Giá chát để làm nản lòng chiến sĩ)
    uniform_int_distribution<int> garbage_dist(80, 130); 
    for(int i = 0; i < n; ++i) {
        for(int j = 0; j < n; ++j) {
            matrix[i][j] = (i == j) ? 0 : garbage_dist(rng);
        }
    }

    // Trộn 25 đỉnh
    vector<int> nodes(n);
    iota(nodes.begin(), nodes.end(), 0);
    shuffle(nodes.begin(), nodes.end(), rng);

    // 2. Chia chác số lượng đỉnh cho các chu trình
    vector<int> sizes(num_subtours, n / num_subtours);
    for(int i = 0; i < n % num_subtours; ++i) {
        sizes[i]++; // Thằng nào lẻ thì cộng thêm 1 đỉnh
    }

    // 3. Chuẩn bị kho giá trị rải đều cho các cạnh trong chu trình ảo
    vector<int> cost_pool;
    int num_edges = n; // Mỗi đỉnh có 1 cạnh đi ra trong chu trình
    for(int i = 0; i < num_edges; ++i) {
        // Rải đều từ 5 đến 90, cộng thêm tí nhiễu
        int val = 5 + i * (90 - 5) / (num_edges - 1) + (rng() % 10 - 5);
        cost_pool.push_back(max(1, val)); 
    }
    shuffle(cost_pool.begin(), cost_pool.end(), rng);

    vector<int> ends, starts; 
    int idx = 0, pool_idx = 0;

    // 4. Lắp ráp các chu trình
    for(int s : sizes) {
        starts.push_back(nodes[idx]);
        for(int i = 0; i < s; ++i) {
            int u = nodes[idx + i];
            int v = nodes[idx + (i + 1) % s];
            matrix[u][v] = cost_pool[pool_idx++]; 
        }
        ends.push_back(nodes[idx + s - 1]);
        idx += s;
    }

    // 5. Nối cầu giữa các chu trình (Tạo thành 1 vòng tròn lớn bao ngoài)
    uniform_int_distribution<int> bridge_dist(50, 80);
    for(int i = 0; i < num_subtours; ++i) {
        int u = ends[i];
        int v = starts[(i + 1) % num_subtours];
        matrix[u][v] = bridge_dist(rng); 
    }

    // In format căn lề đẹp đẽ
    cout << n << "\n";
    for(int i = 0; i < n; ++i) {
        for(int j = 0; j < n; ++j) {
            if (matrix[i][j] < 10) cout << "  ";
            else if (matrix[i][j] < 100) cout << " ";
            cout << matrix[i][j] << " ";
        }
        cout << "\n";
    }
    return 0;
}