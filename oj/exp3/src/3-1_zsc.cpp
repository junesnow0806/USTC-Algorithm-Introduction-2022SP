#include <stdio.h>
#define MAX_N 1000000
long long int a[MAX_N];
long long int right_smaller_count[MAX_N];
long long int left_bigger_count[MAX_N];
long long int orig_index[MAX_N];
long long int count = 0;

void f(long long int left, long long int right) // right not included
{
    if (right - left <= 1)
        return;
    long long int mid = (left + right) / 2;
    f(left, mid);
    f(mid, right);
    long long int len = right - left;
    long long int tmp[len];
    long long int new_id[len];
    for (long long int i = 0; i < len; i++)
        new_id[i] = orig_index[i];
    long long int ileft = left;
    long long int iright = mid;
    long long int itmp = 0;
    long long int count_ij_k = 0;
    long long int count_i_jk_all = 0;
    long long int count_i_jk_no = 0;
    while (itmp < len)
    {
        if (iright >= right || (ileft < mid && a[ileft] < a[iright]))
        {
            tmp[itmp] = a[ileft];
            new_id[itmp] = orig_index[ileft];

            count_ij_k += ((mid - 1 - orig_index[ileft] - right_smaller_count[orig_index[ileft]]) *
                           (itmp - (ileft - left)));

            right_smaller_count[orig_index[ileft]] += itmp - (ileft - left);

            ileft++;
        }
        else if (iright < right)
        {
            tmp[itmp] = a[iright];
            new_id[itmp] = orig_index[iright];

            count_i_jk_no += (left_bigger_count[orig_index[iright]]) *
                             (itmp - (iright - mid));
            count_i_jk_all += (right_smaller_count[orig_index[iright]]) *
                              (itmp - (iright - mid));

            left_bigger_count[orig_index[iright]] += (iright - left) - itmp;
            iright++;
        }
        itmp++;
    }
    count += count_i_jk_all - count_i_jk_no + count_ij_k;
    for (long long int i = 0; i < len; i++)
    {
        a[left + i] = tmp[i];
        orig_index[left + i] = new_id[i];
    }
}
long long int main()
{
    long long int n;
    scanf("%lld", &n);
    for (long long int i = 0; i < n; i++)
    {
        scanf("%lld", &a[i]);
        orig_index[i] = i;
    }
    f(0, n);
    printf("%lld\n",count);
    return 0;
}
