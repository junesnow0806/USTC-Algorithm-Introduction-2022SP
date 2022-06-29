/* 给定一个数组, 求出其中最长递增子序列的长度
 * 算法思想:
 * 用一个数组final_of_leni记录不同长度的递增子序列中最有"潜力"子序列的最后一个元素
 * 初始化final_of_leni[i] = -1;
 * 考察a[i]元素时, 二分查找第一个不小于a[i]的final_of_leni[j], 有可能全部都小于a[i]
 * 将a[i]作为final_of_leni[j+1]对应子序列的最后一个元素, 得到一个新的长度为j的递增子序列newj
 * newj的最后一个元素是a[i], 比final_of_leni[j]小, 因此newj比原来的长度为j的子序列更有可能得到"延长"
 * 遍历完a[i]后取max_len = argmax{final_of_len[i]!=-1}
 */

#include <iostream>
#include <vector>
using namespace std;

int LIS(vector<int> &a) {
    int max_len = 0;
    int n = a.size();
    vector<int> final_of_leni(n + 1, -1);  // final_of_leni[i]记录长度为i的递增子序列的最后一个元素
    for (int k = 0; k < n; k++) {
        if (k == 0) {
            final_of_leni[1] = a[k];
            max_len = 1;
            continue;
        }
        int insert_pos = 0;
        int low = 1, high = max_len;
        // 二分法查找第一个不小于a[k]的final_of_leni[j]
        while (low <= high) {
            if (final_of_leni[high] < a[k]) {
                insert_pos = high + 1;
                break;
            }
            int middle = (low + high) / 2;
            if (final_of_leni[middle] >= a[k] && (middle == 1 || final_of_leni[middle - 1] < a[k])) {
                insert_pos = middle;
                break;
            } else if (final_of_leni[middle] >= a[k]) {
                high = middle - 1;
            } else {
                low = middle + 1;
            }
        }
        final_of_leni[insert_pos] = a[k];
        if (insert_pos > max_len) {
            max_len = insert_pos;
        }
    }
    return max_len;
}

int main() {
    int n;
    cin >> n;
    vector<int> a(n);
    for (int i = 0; i < n; i++) {
        cin >> a[i];
    }
    cout << LIS(a) << endl;
}