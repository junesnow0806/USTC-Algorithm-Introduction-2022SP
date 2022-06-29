#include <iostream>
#include <vector>
using namespace std;
int main() {
    int n;
    cin >> n;
    vector<vector<int>> matrix;
    matrix.push_back({});
    for (int i = 1; i < n+1; i++) {
        matrix.push_back({});
        matrix[i].resize(n+1);
        for (int j = 1; j < n+1; j++) {
            cin >> matrix[i][j];
        }
    }

    vector<vector<int>> S(n+1);
    for (int i = 0; i <= n; i++) {
        S[i].resize(n+1);
    }
    for (int i = 0; i <= n; i++) {
        S[i][0] = 0;
    }
    for (int j = 0; j <= n; j++) {
        S[0][j] = 0;
    }
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            S[i][j] = S[i-1][j] + S[i][j-1] - S[i-1][j-1] + matrix[i][j];
        }
    }
    // 以(i, j)元素为右下角的最大子方阵不超过min(i,j)维
    if (S[n][n] == 0) {
        cout << 0 << endl;
        return 0;
    }
    int max_k = 1;
    for (int i = 1; i <= n; i++) {
        if (i <= max_k)
            continue;
        for (int j = 1; j <= n; j++) {
            if (j <= max_k)
                continue;
            // 考察是否有 > max_k 的全为1的子方阵
            int ij_min = (i > j) ? j : i;
            int low = max_k+1, high = ij_min;
            // 使用二分法枚举k
            while (low <= high) {
                // 考察以(i, j)为右下角的k阶子方阵
                int k = (low+high)/2;
                int prej = j-k;
                int prei = i-k;
                int S1 = S[i][j] - S[i][prej] - S[prei][j] + S[prei][prej];
                if (S1 == k*k) {
                    max_k = k;
                    low = k+1;
                } else {
                    high = k-1;
                }
            }
        }
    }

    cout << max_k << endl;
}