#include <iostream>
#include <vector>
using namespace std;

void merge(vector<long long> &A, long long left, long long right, vector<long long> &origin,
           vector<long long> &left_bigger_num,
           vector<long long> &right_smaller_num, long long &tuple_count) {
    long long len = right - left + 1;
    if (len <= 1)
        return;
    long long mid = (left + right) / 2;

    merge(A, left, mid, origin, left_bigger_num, right_smaller_num, tuple_count);
    merge(A, mid + 1, right, origin, left_bigger_num, right_smaller_num, tuple_count);

    vector<long long> origin_bak(len);

    vector<long long> B(len);  // merge排序的临时数组
    long long count_ij_k = 0;
    long long count_i_jk = 0;
    long long count_i_jk_todel = 0;
    long long bi = 0;
    long long li = left;
    long long lstart = left;
    long long lend = mid;
    long long ri = mid + 1;
    long long rstart = mid + 1;
    long long rend = right;

    while (bi < len) {
        if (li <= lend && A[li] < A[ri]) {
            // B[bi]处要放置A[li]
            B[bi] = A[li];
            origin_bak[bi] = origin[li];

            long long origin_t = origin[li];

            // 对于元素A[li], 左半子数组中原位置在A[li]后面且大于A[li]的元素个数 * 右半子数组中小于A[li]的元素个数
            count_ij_k += (lend - origin_t - right_smaller_num[origin_t])  // 左半子数组中, 在原数组中位置在A[li]右边且大于A[li]的元素个数
                          * (bi - (li - lstart));                          // 右半子数组中小于A[li]的元素个数

            right_smaller_num[origin_t] += bi - (li - lstart);  // 更新原数组中在A[li]右边且小于A[li]的元素个数
            li++;
            bi++;
        } else if (ri <= rend && A[ri] < A[li]) {
            // B[bi]处要放置A[ri]
            B[bi] = A[ri];
            origin_bak[bi] = origin[ri];

            long long origin_t = origin[ri];

            // 对于元素A[ri], 右半子数组中原位置在A[ri]前面且大于A[ri]的元素个数 * 左半子数组中小于A[ri]的元素个数
            count_i_jk_todel += left_bigger_num[origin_t] * (bi - (ri - rstart));

            // 左半子数组中原位置在A[ri]前面且小于A[ri]的元素个数 * 右半子数组中原位置为A[ri]后面且小于A[ri]的元素个数
            count_i_jk += (bi - (ri - rstart)) * right_smaller_num[origin_t];

            left_bigger_num[origin_t] += ri - left - bi;  // 更新原数组中在A[ri]左边且大于A[li]的元素个数

            ri++;
            bi++;
        } else if (li > lend) {
            // 左半子数组已经放完了, 只剩右半子数组了
            if (ri > rend) {
                break;
            } else {
                // B[bi]处要放置A[ri]
                B[bi] = A[ri];
                origin_bak[bi] = origin[ri];

                long long origin_t = origin[ri];

                // 对于元素A[ri], 右半子数组中原位置在A[ri]前面且大于A[ri]的元素个数 * 左半子数组中小于A[ri]的元素个数
                count_i_jk_todel += left_bigger_num[origin_t] * (bi - (ri - rstart));

                // 左半子数组中原位置在A[ri]前面且小于A[ri]的元素个数 * 右半子数组中原位置为A[ri]后面且小于A[ri]的元素个数
                count_i_jk += (bi - (ri - rstart)) * right_smaller_num[origin_t];

                left_bigger_num[origin_t] += ri - left - bi;  // 更新原数组中在A[ri]左边且大于A[li]的元素个数

                ri++;
                bi++;
            }
        } else if (ri > rend) {
            // 右半子数组已经放完了, 只剩左半子数组了
            if (li > lend) {
                break;
            } else {
                // B[bi]处要放置A[li]
                B[bi] = A[li];
                origin_bak[bi] = origin[li];

                long long origin_t = origin[li];

                // 对于元素A[li], 左半子数组中原位置在A[li]后面且大于A[li]的元素个数 * 右半子数组中小于A[li]的元素个数
                count_ij_k += (lend - origin_t - right_smaller_num[origin_t])  // 左半子数组中, 在原数组中位置在A[li]右边且大于A[li]的元素个数
                              * (bi - (li - lstart));                          // 右半子数组中小于A[li]的元素个数

                right_smaller_num[origin_t] += bi - (li - lstart);  // 更新原数组中在A[li]右边且小于A[li]的元素个数
                li++;
                bi++;
            }
        }
    }
    tuple_count += count_ij_k + count_i_jk - count_i_jk_todel;
    for (bi = 0; bi < len; bi++) {
        A[bi + left] = B[bi];
        origin[bi + left] = origin_bak[bi];
    }
}

int main() {
    long long n;
    cin >> n;
    vector<long long> A(n + 1);
    for (long long i = 1; i <= n; i++) {
        cin >> A[i];
    }

    vector<long long> origin(n + 1);  // 记录每个元素在原数组中的位置
    for (long long i = 1; i <= n; i++) {
        origin[i] = i;
    }

    // 对于每个元素e, 记录在原数组中在e左边且比e大的元素个数
    vector<long long> left_bigger_num(n + 1);
    // 对于每个元素e, 记录在原数组中在e右边且比e小的元素个数
    vector<long long> right_smaller_num(n + 1);

    long long tuple_count = 0;  // 最后三元组的个数可能会非常多

    merge(A, 1, n, origin, left_bigger_num, right_smaller_num, tuple_count);
    cout << tuple_count << endl;
}