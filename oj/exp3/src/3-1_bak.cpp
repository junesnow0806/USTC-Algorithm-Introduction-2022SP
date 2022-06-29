#include <iostream>
#include <vector>
using namespace std;

int main() {
    int n;
    cin >> n;
    vector<int> A(n+1);
    A[0] = 0;
    for (int i = 1; i <= n; i++) {
        cin >> A[i];
    }

    int count = 0;
    for (int j = 2; j <= n-1; j++) {
        for (int i = 1; i < j; i++) {
            if (A[i] >= A[j])
                continue;
            for (int k = j+1; k <= n; k++) {
                if (A[k] < A[i])
                    count++;
            }
        }
    }
    cout << count << endl;
}