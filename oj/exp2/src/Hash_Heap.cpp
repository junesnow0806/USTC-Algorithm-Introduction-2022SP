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

struct htnode {
    // hash table node
    string name;
    int hpos;  // pos in heap
    htnode *next;

    htnode() { next = nullptr; }
    htnode(string nm, int pos) : name(nm), hpos(pos), next(nullptr) {}
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
    htnode *final_node(htnode *lsit);
    htnode *find_node(const string &name, htnode *list);
    void delete_node(htnode *todel, htnode *&list);
    ~HashHeap();

   private:
    vector<element> hheap;
    htnode *htable[MAX_HASH_TABLE_LENGTH];
    int highest_num;
};

HashHeap::HashHeap() {
    highest_num = 0;
    for (int i = 0; i < MAX_HASH_TABLE_LENGTH; i++) {
        htable[i] = nullptr;
    }
}

HashHeap::~HashHeap() {
    for (int i = 0; i < MAX_HASH_TABLE_LENGTH; i++) {
        if (htable[i] != nullptr) {
            htnode *now = htable[i]->next;
            while (now) {
                delete_node(now, htable[i]);
                now = htable[i]->next;
            }
        }
    }
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

htnode *HashHeap::find_node(const string &name, htnode *list) {
    /* 给定链表list, 找到该链表中name域等于指定name的节点 */
    htnode *now = list;
    while (now) {
        if (now->name == name)
            return now;
        now = now->next;
    }
    return nullptr;
}

void HashHeap::delete_node(htnode *todel, htnode *&list) {
    /* 给定链表list, 删除该链表中指令节点todel */
    if (list == nullptr || todel == nullptr)
        return;
    if (list == todel) {
        list = list->next;
        delete todel;
        return;
    }
    htnode *now = list;
    while (now && now->next != todel) {
        now = now->next;
    }
    if (now) {
        now->next = todel->next;
        delete todel;
    }
}

htnode *HashHeap::final_node(htnode *list) {
    /* 给定链表list, 找到该链表中的最后一个节点 */
    if (list == nullptr) {
        return list;
    }
    htnode *now = list;
    while (now->next != nullptr) {
        now = now->next;
    }
    return now;
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
    int htpos = hash_str2int(name);
    if (htable[htpos] == nullptr) {
        htable[htpos] = new htnode(name, i);
    } else {
        htnode *tail = final_node(htable[htpos]);
        tail->next = new htnode(name, i);
    }

    while (i > 0) {
        /* 自底向上调整 */
        if (hheap[i].score > hheap[(i - 1) / 2].score) {
            // i元素的score比其父节点的score大, 需要交换两者
            // 除了交换两者在堆中的位置, 还需要交换两者在pos表对应的值
            string name1 = hheap[i].name, name2 = hheap[(i - 1) / 2].name;

            int htpos1 = hash_str2int(name1), htpos2 = hash_str2int(name2);
            htnode *htnode1 = find_node(name1, htable[htpos1]);
            htnode *htnode2 = find_node(name2, htable[htpos2]);
            swap(htnode1->hpos, htnode2->hpos);

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
     * 注意考虑最后一个节点就是要删除的节点的情况!!
     * 然后根据last的大小向上或向下heapify
     * 注意在向上或向下heapify的过程中需要相应地更新pos表
     * 注意删除之后更新highest_num
     */
    int highest_score = hheap[0].score;

    int which_htable = hash_str2int(name);
    htnode *which_node = find_node(name, htable[which_htable]);
    int todel_pos_inheap = which_node->hpos;  // 找到要删除节点在堆中的下标
    // 因为要删除这个元素了, 所以哈希表中不需要which_node了
    delete_node(which_node, htable[which_htable]);

    element todel_e = hheap[todel_pos_inheap];
    element final_e = hheap.back();
    // 将最后一个元素放到被删除元素的位置, 再pop, 就相当于从堆中删除那个元素了
    hheap[todel_pos_inheap] = final_e;
    hheap.pop_back();
    if (todel_e.name == final_e.name)
        return;
    // 更新最后一个元素哈希表中存的堆下标
    which_htable = hash_str2int(final_e.name);
    which_node = find_node(final_e.name, htable[which_htable]);
    which_node->hpos = todel_pos_inheap;

    if (hheap[todel_pos_inheap].score > todel_e.score) {
        // 新取代的节点比可能其父节点大, 向上heapify
        int i = todel_pos_inheap;
        while (i > 0) {
            if (hheap[i].score > hheap[(i - 1) / 2].score) {
                // i元素的score比其父节点的score大, 需要交换两者
                // 除了交换两者在堆中的位置, 还需要交换两者在pos表对应的值

                // 更新pos表
                string name1 = hheap[i].name, name2 = hheap[(i - 1) / 2].name;
                int htpos1 = hash_str2int(name1), htpos2 = hash_str2int(name2);
                htnode *htnode1 = find_node(name1, htable[htpos1]);
                htnode *htnode2 = find_node(name2, htable[htpos2]);
                swap(htnode1->hpos, htnode2->hpos);

                swap(hheap[i], hheap[(i - 1) / 2]);  // 交换堆中的元素
                i = (i - 1) / 2;
            } else {
                break;
            }
        }
    } else if (hheap[todel_pos_inheap].score < todel_e.score) {
        // 新取代的节点可能比子节点小, 向下Heapify
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
                int which_table1 = hash_str2int(hheap[todel_pos_inheap].name);
                int which_table2 = hash_str2int(hheap[largest].name);
                htnode *which_node1 = find_node(hheap[todel_pos_inheap].name, htable[which_table1]);
                htnode *which_node2 = find_node(hheap[largest].name, htable[which_table2]);
                swap(which_node1->hpos, which_node2->hpos);

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
    int which_table = hash_str2int(name);
    htnode *which_node = find_node(name, htable[which_table]);
    if (which_node == nullptr)
        return 0;
    int posinheap = which_node->hpos;
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