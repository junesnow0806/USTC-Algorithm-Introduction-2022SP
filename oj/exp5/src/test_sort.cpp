#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

struct node {
    int id;
    int val;
    node() : id(0), val(0) {}
    node(int id, int val) : id(id), val(val) {}
};

bool cmp(node n1, node n2) {
    return (n1.val < n2.val);
}

int main() {
    node n1(1, 3);
    node n2(2, 2);
    node n3(3, 8);
    vector<node> vec;
    vec.push_back(n1);
    vec.push_back(n2);
    vec.push_back(n3);
    sort(vec.begin(), vec.end(), cmp);
    for (auto n : vec) {
        cout << n.id << " " << n.val << endl;
    }
}