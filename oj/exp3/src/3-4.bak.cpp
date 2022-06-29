#include <iostream>
#include <vector>
using namespace std;
int main() {
    int n;
    cin >> n;
    vector<vector<int>> matrix;
    for (int i = 0; i < n; i++) {
        matrix.push_back({});
        matrix[i].resize(n);
        for (int j = 0; j < n; j++) {
            cin >> matrix[i][j];
        }
    }

    int maxk = 0;
    for (int i = 0; i < n; i++) {
        vector<int> col_sum(n); // vector会自动初始化所有元素为0
        for (int j = i; j < n; j++) {
            for (int col = 0; col < n; col++) {
                // 计算第i~j行的列和
                col_sum[col] += matrix[j][col];
            }
            int k = j-i+1;
            for (int col = 0; col < n; col++) {
                int count = 0;
                while (col_sum[col] == k) {
                    // 判断是否有k x k全部为1的方阵
                    count++;
                    col++;
                    if (count == k) {
                        if (k > maxk)
                            maxk = k;
                        break;
                    }
                }
                if (count == k) {
                    break;
                }
            }
        }
    }
    cout << maxk << endl;
}