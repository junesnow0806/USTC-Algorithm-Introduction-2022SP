#include <stdio.h>
#include <math.h>

void counting_sort(int A[], int n, int k, int d)
{
    int A_bit[n];
    int A_temp[n];
    int c[k+1];

    for(int i = 0; i < n; i++)
        A_bit[i] = (int)(A[i]/pow(10, d-1)) % 10; 

    for(int i = 0; i <= k; i++)
        c[i] = 0;
    for(int i = 0; i < n; i++)
        c[A_bit[i]] = c[A_bit[i]] + 1;
    for(int i = 1; i <= k; i++)
        c[i] = c[i] + c[i-1];
    

    for(int i = n-1; i >= 0; i--)
    {
        A_temp[c[A_bit[i]]-1] = A[i];
        c[A_bit[i]]--;
    }
    for(int i = 0; i < n; i++)
        A[i] = A_temp[i];     
       
}

void radix_sort(int A[], int n, int d)
{
    for(int i = 1; i <= d; i++)
        counting_sort(A, n, 9, i);
}
int main()
{
    int n;
    scanf("%d", &n);
    
    int A[n];
    int max = -1;
    for(int i = 0; i < n; i++)
    {  
        scanf("%d", &(A[i]));
        if(A[i] > max)
            max = A[i];
    }

    int d = 0;
    while(max > 0)
    {
        d++;
        max = max / 10;
    }
    
    radix_sort(A, n, d);

    for(int i = 0; i < n; i++)
        printf("%d ", A[i]);
}
