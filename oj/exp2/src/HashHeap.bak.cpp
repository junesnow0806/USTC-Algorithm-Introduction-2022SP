#include <algorithm>
#include <iostream>
#include <queue>
#include <string>
#include <vector>
using namespace std;

#define MAX_HASH_TABLE_LENGTH 700000

struct element {
    string name;
    int score;
};

int hash_str2int(string name) {
    /* string->int的哈希函数
     * BKDRHash
     */
    unsigned int seed = 80867;
    unsigned int radix = 0;
    for (auto ch : name) {
        radix = radix * seed + ch;
    }
    return radix % MAX_HASH_TABLE_LENGTH;
}

class HashHeap {
    /* 实际存储节点按大根堆来存
     * 根节点为0号, 对于i号元素, 其父节点为(i-1)/2
     * 为了实现O(1)查找, 还需要有name映射到节点下标的哈希函数
     * 但如果哈希函数是string -> int的映射的话就不可能保持score是一个大根堆
     * 所以考虑再建一个表pos, 用name -> i(string->int)映射, pos[i]处存放name对应节点在大根堆中的下标
     * 从string->int的哈希函数采用BKDRHash
     * 使用拉链法处理哈希冲突
     */
   public:
    HashHeap();
    void insert(string name, int score);
    void del(string name);
    int find(string name);
    void print_highest();
    int getsize() { return hheap.size(); }
    void update_highest_num();
    int which_invec(const string &name, const vector<int> &posi);

   private:
    vector<element> hheap;
    vector<int> *pos;  // 每个pos[i]是一个vector
    int highest_num;
};

HashHeap::HashHeap() {
    highest_num = 0;
    pos = new vector<int>[MAX_HASH_TABLE_LENGTH];
}

void swap(int &a, int &b) {
    int tmp = a;
    a = b;
    b = tmp;
}

void swap(element &a, element &b) {
    element tmp = a;
    a = b;
    b = tmp;
}

int HashHeap::which_invec(const string &name, const vector<int> &posi) {
    /* 给定一个name和这个name映射到哈希表中的vector
     * 查询vector中哪一个位置的元素才是name所对应节点在hheap中的位置
     */
    if (posi.size() == 1)
        return posi[0];
    for (int k = 0; k < posi.size(); k++) {
        if (name == hheap[posi[k]].name)
            return k;
    }
    return -1;
}

void HashHeap::update_highest_num() {
    if (hheap.size() == 0) {
        highest_num = 0;
        return;
    } else {
        int highest_score = hheap[0].score;
        highest_num = 0;
        queue<int> highest_e;
        highest_e.push(0);
        while (!highest_e.empty()) {
            int front = highest_e.front();
            highest_e.pop();
            highest_num++;
            int left = front * 2 + 1, right = front * 2 + 2;
            if (hheap[left].score == highest_score)
                highest_e.push(left);
            if (hheap[right].score == highest_score)
                highest_e.push(right);
        }
    }
}

void HashHeap::insert(string name, int score) {
    /* 插入一个节点
     * 现在堆末尾插入该节点, 然后不断将其往根部移动到合适的位置
     * 移动过程中更新pos表
     * 注意更新最高分人数
     */
    element newe;
    newe.name = name;
    newe.score = score;

    if (hheap.size() == 0 || score > hheap[0].score) {
        highest_num = 1;
    } else if (score == hheap[0].score) {
        highest_num++;
    }

    hheap.push_back(newe);
    int i = hheap.size() - 1;
    pos[hash_str2int(name)].push_back(i);
    while (i > 0) {
        /* 自底向上调整 */
        if (hheap[i].score > hheap[(i - 1) / 2].score) {
            // i元素的score比其父节点的score大, 需要交换两者
            // 除了交换两者在堆中的位置, 还需要交换两者在pos表对应的值
            string name1 = hheap[i].name, name2 = hheap[(i - 1) / 2].name;

            int vec1 = hash_str2int(name1), vec2 = hash_str2int(name2);
            int vec_pos1 = which_invec(name1, pos[vec1]);
            int vec_pos2 = which_invec(name2, pos[vec2]);
            swap(pos[vec1][vec_pos1], pos[vec2][vec_pos2]);

            swap(hheap[i], hheap[(i - 1) / 2]);
            i = (i - 1) / 2;
        } else {
            break;
        }
    }
}

void HashHeap::del(string name) {
    /* 删除一个节点
     * 根据name映射找到该节点在堆中的位置
     * 将其与最后一个节点last交换
     * 然后根据last的大小向上或向下heapify
     * 注意在向上或向下heapify的过程中需要相应地更新pos表
     * 注意删除之后更新highest_num
     */
    int highest_score = hheap[0].score;

    int todel_vec_index = hash_str2int(name);  // 先找到对应哪一个vector
    int todel_pos_invec = which_invec(name, pos[todel_vec_index]);
    int todel_pos_inheap = pos[todel_vec_index][todel_pos_invec];  // todel_pos_inheap是要删除的元素在堆中的位置
    int end = pos[todel_vec_index].size() - 1;
    swap(pos[todel_vec_index][todel_pos_invec], pos[todel_vec_index][end]);
    pos[todel_vec_index].pop_back();

    element todel_e = hheap[todel_pos_inheap];
    element final_e = hheap.back();
    hheap[todel_pos_inheap] = final_e;  // 将最后一个元素放到被删除元素的位置
    hheap.pop_back();
    int final_element_vec_index = hash_str2int(final_e.name);
    int final_element_pos_invec = which_invec(final_e.name, pos[final_element_vec_index]);
    pos[final_element_vec_index][final_element_pos_invec] = todel_pos_inheap;  // 这个地方的pos原本存放的值是hheap.size()-1

    if (hheap[todel_pos_inheap].score > todel_e.score) {
        // 向上heapify
        int i = todel_pos_inheap;
        while (i > 0) {
            if (hheap[i].score > hheap[(i - 1) / 2].score) {
                // i元素的score比其父节点的score大, 需要交换两者
                // 除了交换两者在堆中的位置, 还需要交换两者在pos表对应的值

                // 更新pos表
                string name1 = hheap[i].name, name2 = hheap[(i - 1) / 2].name;
                int vec1 = hash_str2int(name1), vec2 = hash_str2int(name2);
                int vec_pos1 = which_invec(name1, pos[vec1]);
                int vec_pos2 = which_invec(name2, pos[vec2]);
                swap(pos[vec1][vec_pos1], pos[vec2][vec_pos2]);

                swap(hheap[i], hheap[(i - 1) / 2]);  // 交换堆中的元素
                i = (i - 1) / 2;
            } else {
                break;
            }
        }
    } else {
        // 向下Heapify
        int left = todel_pos_inheap * 2 + 1, right = todel_pos_inheap * 2 + 2;
        while (right < hheap.size() || left < hheap.size()) {
            int largest = todel_pos_inheap;
            if (right < hheap.size() && hheap[right].score > hheap[largest].score) {
                largest = right;
            }
            if (left < hheap.size() && hheap[left].score > hheap[largest].score) {
                largest = left;
            }
            // exchange heap[todel_pos] and heap[largest]
            if (largest != todel_pos_inheap) {
                int vec_index1 = hash_str2int(hheap[todel_pos_inheap].name);
                int vec_index2 = hash_str2int(hheap[largest].name);
                int posinvec1 = which_invec(hheap[todel_pos_inheap].name, pos[vec_index1]);
                int posinvec2 = which_invec(hheap[largest].name, pos[vec_index2]);
                swap(pos[vec_index1][posinvec1], pos[vec_index2][posinvec2]);

                swap(hheap[todel_pos_inheap], hheap[largest]);
                todel_pos_inheap = largest;
                // 注意更新left和right
                left = todel_pos_inheap * 2 + 1;
                right = todel_pos_inheap * 2 + 2;
            } else {
                break;
            }
        }
    }
    if (todel_e.score == highest_score)
        update_highest_num();
}

int HashHeap::find(string name) {
    int vec_index = hash_str2int(name);
    int posinvec = which_invec(name, pos[vec_index]);
    int posinheap = pos[vec_index][posinvec];
    return hheap[posinheap].score;
}

void HashHeap::print_highest() {
    /* 按字典序输出最高分的k个同学名字
     * 注意最高分的同学在堆中的位置不一定都集中在最前面
     * 可以使用一个队列来找出所有最高分的同学
     */
    vector<string> toprint;
    queue<int> highest_e;
    highest_e.push(0);
    int highest_score = hheap[0].score;
    while (!highest_e.empty()) {
        int front = highest_e.front();
        highest_e.pop();
        toprint.push_back(hheap[front].name);
        int left = front * 2 + 1, right = front * 2 + 2;
        if (left < hheap.size() && hheap[left].score == highest_score)
            highest_e.push(left);
        if (right < hheap.size() && hheap[right].score == highest_score)
            highest_e.push(right);
    }
    sort(toprint.begin(), toprint.end());
    for (auto name : toprint)
        cout << name << endl;
}

int main() {
    HashHeap hash_heap;
    int m;
    cin >> m;
    while (m--) {
        string op;
        cin >> op;
        if (op == "I") {
            string name;
            cin >> name;
            int score;
            cin >> score;
            hash_heap.insert(name, score);
        } else if (op == "D") {
            string name;
            cin >> name;
            hash_heap.del(name);
        } else if (op == "S") {
            string name;
            cin >> name;
            cout << hash_heap.find(name) << endl;
        } else if (op == "F") {
            hash_heap.print_highest();
        }
    }
    return 0;
}