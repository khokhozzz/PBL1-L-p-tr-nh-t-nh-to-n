#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <random>

using namespace std;

int main() {
    int n = 25;
    int k = 3; // QUYỀN LỰC LÀ ĐÂY: Thích mấy chu trình thì sửa số này
    mt19937 rng(942298); 
    
    // Khởi tạo "đại dương 100"
    vector<vector<int>> matrix(n, vector<int>(n, 100));
    for(int i = 0; i < n; ++i) matrix[i][i] = 0;

    // Trộn đỉnh
    vector<int> nodes(n);
    iota(nodes.begin(), nodes.end(), 0);
    shuffle(nodes.begin(), nodes.end(), rng);

    // Chia đều n đỉnh cho k chu trình
    vector<int> sizes(k, n / k);
    for(int i = 0; i < n % k; ++i) sizes[i]++;

    int idx = 0;

    // Bơm các chu trình giá 1 vào giữa đại dương 100
    for(int s : sizes) {
        for(int i = 0; i < s; ++i) {
            int u = nodes[idx + i];
            int v = nodes[idx + (i + 1) % s];
            matrix[u][v] = 1; 
        }
        idx += s;
    }

    // KHÔNG CẦN CẦU NỐI ĐẶC BIỆT NỮA. 
    // Thuật toán muốn nối chu trình thì tự đi mà bốc bừa các cạnh giá 100.

    // In format
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