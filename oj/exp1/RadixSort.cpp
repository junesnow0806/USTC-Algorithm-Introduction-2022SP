#include <algorithm>
#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

#define RADIX 10

void CountingSort(vector<int> &array, vector<int> &digit) {
    int n = array.size();
    vector<int> C(RADIX);
    for (auto d : digit) {
        C[d]++;
    }
    for (int i = 1; i < RADIX; i++) {
        C[i] += C[i - 1];
    }
    vector<int> temp(n);
    for (int i = n - 1; i >= 0; i--) {  // 注意这里必须要从后往前遍历才能保持稳定性
        temp[C[digit[i]] - 1] = array[i];
        C[digit[i]]--;
    }
    array.assign(temp.begin(), temp.end());
}

int main() {
    int n;
    cin >> n;
    vector<int> array(n);
    int maxelement = -100;
    for (int i = 0; i < n; i++) {
        cin >> array[i];
        if (array[i] > maxelement)
            maxelement = array[i];
    }

    /*
    for (int i = 0; i < n; i++)
        cout << array[i] << " ";
    cout << endl;
    */

    vector<int> digit(n);
    int maxdigit = 0;  // 最大元素的位数
    while (maxelement > 0) {
        maxdigit++;
        maxelement /= 10;
    }

    // RadixSort主过程
    for (int i = 1; i <= maxdigit; i++) {
        int tomod = 1;
        for (int j = 0; j < i; j++) {
            tomod *= 10;
        }
        for (int j = 0; j < n; j++) {
            // 注意是先除再取余
            digit[j] = array[j] / (tomod / 10);
            digit[j] %= 10;
        }
        

        CountingSort(array, digit);
    }

    for (int i = 0; i < n; i++) {
        if (i < n - 1)
            cout << array[i] << " ";
        else
            cout << array[i] << endl;
    }
}