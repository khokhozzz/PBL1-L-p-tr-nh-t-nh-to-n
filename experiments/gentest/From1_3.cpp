#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <random>

using namespace std;

int main() {
    int n = 25;
    mt19937 rng(2147); // Đổi seed tùy ý

    vector<vector<int>> matrix(n, vector<int>(n));

    // 1. Cạnh rác phải bị đẩy giá lên cao (80-200). 
    // Vì nếu rác rẻ hơn chu trình, thuật toán sẽ không thèm chui vào bẫy nữa.
    uniform_int_distribution<int> garbage_dist(80, 150); 
    for(int i = 0; i < n; ++i) {
        for(int j = 0; j < n; ++j) {
            matrix[i][j] = (i == j) ? 0 : garbage_dist(rng);
        }
    }

    vector<int> nodes(n);
    iota(nodes.begin(), nodes.end(), 0);
    shuffle(nodes.begin(), nodes.end(), rng);

    // 2. ÉP GIÁ TRỊ RẢI ĐỀU CHO CHU TRÌNH ẢO
    vector<int> cost_pool;
    int num_subtour_edges = 25; // Tổng số cạnh của 3 chu trình (8+8+9)
    for(int i = 0; i < num_subtour_edges; ++i) {
        // Trải đều 25 mốc từ 10 đến 100
        int val = 10 + i * (100 - 10) / (num_subtour_edges - 1);
        
        // Nêm nếm thêm chút +- 3 để số nhìn tự nhiên, không bị chẵn quá
        val += (rng() % 7) - 3; 
        cost_pool.push_back(max(1, val)); 
    }
    // Trộn cái kho này lên
    shuffle(cost_pool.begin(), cost_pool.end(), rng);

    vector<int> sizes = {8, 8, 9};
    vector<int> ends, starts; 
    int idx = 0, pool_idx = 0;

    // 3. Rải đều mìn vào 3 chu trình
    for(int s : sizes) {
        starts.push_back(nodes[idx]);
        for(int i = 0; i < s; ++i) {
            int u = nodes[idx + i];
            int v = nodes[idx + (i + 1) % s];
            matrix[u][v] = cost_pool[pool_idx++]; // Bốc từ kho ra xài
        }
        ends.push_back(nodes[idx + s - 1]);
        idx += s;
    }

    // Cầu nối: Cho giá lơ lửng ở giữa (50-70)
    uniform_int_distribution<int> bridge_dist(50, 70);
    for(int i = 0; i < 3; ++i) {
        int u = ends[i];
        int v = starts[(i + 1) % 3];
        matrix[u][v] = bridge_dist(rng); 
    }

    // In format căn lề cho dễ nhìn
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