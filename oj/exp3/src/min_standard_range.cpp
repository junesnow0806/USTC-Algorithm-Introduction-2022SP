#include <iostream>
#include <vector>
#include <deque>

using namespace std;

struct node {
    int index;
    int prefix_sum;
};

int main() {
    int n, K;
    cin >> n >> K;
    vector<int> a(n);
    vector<node> P(n+1);
    for (int i = 0; i < n; i++) {
        cin >> a[i];
        if (i == 0) {
            P[i].index = i;
            P[i].prefix_sum = 0;
        }
        else {
            P[i].index = i;
            P[i].prefix_sum = P[i-1].prefix_sum + a[i-1];
        }
    }
    P[n].prefix_sum = P[n-1].prefix_sum + a[n-1];

    deque<node> Q;
    Q.push_back(P[1]);
    int min_range_len = n;
    for (int i = 2; i <= n; i++) {
        while (!Q.empty() && Q.back().prefix_sum >= P[i].prefix_sum) {
            Q.pop_back();
        }
        while (!Q.empty() && (P[i].prefix_sum - Q[0].prefix_sum) >= K) {
            if (i - Q[0].index < min_range_len) {
                min_range_len = i - Q[0].index;
            }
            Q.pop_front();
        }
        Q.push_back(P[i]);
    }
    cout << min_range_len << endl;
}