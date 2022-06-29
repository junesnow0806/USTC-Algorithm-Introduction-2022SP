#include <iostream>
#include <vector>
using namespace std;

int main() {
    int n, V;
    cin >> n >> V;

    vector<int> a(n);
    for (int i = 0; i < n; i++)
        cin >> a[i];

    vector<int> dp(V+1);
    dp[0] = 1;
    for (int i = 0; i < n; i++) {
        for (int j = a[i]; j <= V; j++) {
            dp[j] = (dp[j] + dp[j-a[i]]) % 1000000007;
        }
    }   
    cout << dp[V] << endl;
}