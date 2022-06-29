#include <stdio.h>

#define MAX 1000000
#define INFI -1000000000
typedef struct Augmenting_a
{
    int key;
    long long int count;
}Augmenting_a;

Augmenting_a a1[MAX];
Augmenting_a a2[MAX];
Augmenting_a a3[MAX];
Augmenting_a L[MAX];
Augmenting_a R[MAX];

int temp_count[MAX];
int Low[MAX];

/*i,j在左侧，k在右侧*/
//对固定的i进行，此时count统计的是原来的子问题中，右侧大于该元素的数的数目
//Low是合并过程中右边的子问题中小于该元素的数的数目
long long int Merge_ij_k(Augmenting_a A[], int left, int mid, int right)
{
    //#a[i] < a[j] and #a[i] > a[k]
    int left_num = mid - left;
    int right_num = right - mid + 1;
    for(int i = 1; i <= left_num; i++)
    {
        L[i] = A[left + i - 1];
        //temp_count[i] = L[i].count;
    }
    for(int i = 1; i <= right_num; i++)
        R[i] = A[mid + i - 1];
    L[0].key = INFI;
    R[0].key = INFI;

    int l_index = left_num;
    int r_index = right_num;
    for(int i = right; i >= left; i--)
    {
        if(L[l_index].key > R[r_index].key)
        {
            A[i] = L[l_index];
            Low[l_index] = r_index;
            A[i].count += right_num - r_index;
            l_index--;
        }
        else
        {
            A[i] = R[r_index];
            r_index--;
        }
    }
    
    long long int sum = 0;
    for(int i = 1; i <= left_num; i++)
        sum += L[i].count * Low[i];
    return sum;
}

long long int Count_ij_k(Augmenting_a A[], int left, int right)
{
    if(left < right)
    {
        int mid = (left + right) / 2;
        long long int num1 = Count_ij_k(A, left, mid);
        long long int num2 = Count_ij_k(A, mid+1, right);
        return num1 + num2 + Merge_ij_k(A, left, mid+1, right);
    }
    else
        return 0;
}

/*i在左侧，j,k在右侧*/
//对固定的j进行，先不考虑a[i] > a[k]的限制, 此时count统计的是原来的子问题中，右侧小于该元素的数的数目
//Low是合并过程中左边的子问题中小于该元素的数的数目
long long Merge_i_jk_1(Augmenting_a A[], int left, int mid, int right)
{
    int left_num = mid - left;
    int right_num = right - mid + 1;
    for(int i = 1; i <= left_num; i++)
    {
        L[i] = A[left + i - 1];
        //temp_count[i] = L[i].count;
    }
    for(int i = 1; i <= right_num; i++)
        R[i] = A[mid + i - 1];
    L[0].key = INFI;
    R[0].key = INFI;

    int l_index = left_num;
    int r_index = right_num;
    for(int i = right; i >= left; i--)
    {
        if(L[l_index].key > R[r_index].key)
        {
            A[i] = L[l_index];
            A[i].count += r_index;
            l_index--;
        }
        else
        {
            A[i] = R[r_index];
            Low[r_index] = l_index ;
            r_index--;
        }
    }
    long long int sum = 0;
    for(int i = 1; i <= right_num; i++)
        sum += R[i].count * Low[i];
    return sum;
}

long long int Count_i_jk_1(Augmenting_a A[], int left, int right)
{
    if(left < right)
    {
        int mid = (left + right) / 2;
        long long int num1 = Count_i_jk_1(A, left, mid);
        long long int num2 = Count_i_jk_1(A, mid+1, right);
        return num1 + num2 + Merge_i_jk_1(A, left, mid+1, right);
    }
    else
        return 0;
}

/*i在左侧，j,k在右侧*/
//对固定的k进行，统计a[i] < a[k]的组合数,用于去除多余情况，此时count统计的是原来的子问题中，左侧大于该元素的数的数目
//Low是合并过程中左边的子问题中小于该元素的数的数目
long long Merge_i_jk_2(Augmenting_a A[], int left, int mid, int right)
{
    int left_num = mid - left;
    int right_num = right - mid + 1;
    for(int i = 1; i <= left_num; i++)
    {
        L[i] = A[left + i - 1];
        //temp_count[i] = L[i].count;
    }
    for(int i = 1; i <= right_num; i++)
        R[i] = A[mid + i - 1];
    L[0].key = INFI;
    R[0].key = INFI;

    int l_index = left_num;
    int r_index = right_num;
    for(int i = right; i >= left; i--)
    {
        if(L[l_index].key > R[r_index].key)
        {
            A[i] = L[l_index];           
            l_index--;
        }
        else
        {
            A[i] = R[r_index];
            Low[r_index] = l_index;
            A[i].count += left_num - l_index;
            r_index--;
        }
    }
    long long int sum = 0;
    for(int i = 1; i <= right_num; i++)
        sum += R[i].count * Low[i];
    return sum;
}

long long int Count_i_jk_2(Augmenting_a A[], int left, int right)
{
    if(left < right)
    {
        int mid = (left + right) / 2;
        long long int num1 = Count_i_jk_2(A, left, mid);
        long long int num2 = Count_i_jk_2(A, mid+1, right);
        return num1 + num2 + Merge_i_jk_2(A, left, mid+1, right);
    }
    else
        return 0;
}


int main()
{
    int n;
    scanf("%d", &n);

    int temp;
    for(int i = 1; i <= n; i++)
    {
        scanf("%d", &temp);
        a1[i].key = temp;
        a2[i].key = temp;
        a3[i].key = temp;
        a1[i].count = 0;
        a2[i].count = 0;
        a3[i].count = 0;
    }

    long long int sum1 = Count_ij_k(a1, 1, n);
    long long int sum2 = Count_i_jk_1(a2, 1, n);
    long long int sum3 = Count_i_jk_2(a3, 1, n);
    printf("%lld", sum1 + sum2 - sum3);
}