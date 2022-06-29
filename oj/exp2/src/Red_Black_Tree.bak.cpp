#include <iostream>
#include <vector>
using namespace std;

#define RED false
#define BLACK true

struct node {
    int value;
    node *left, *right, *parent;
    int size;  // 以这个节点为根的子树所含的节点数
    bool color;
};

class RBTree {
   public:
    RBTree();
    void insert(int value);
    void del(int value);
    int find(int k);
    node *findmin(int A);
    node *findmax(int B);
    int getnum(int L, int R);
    void left_rotate(node *x);
    void right_rotate(node *y);
    void insert_fixup(node *z);
    void delete_fixup(node *x);
    node *findnode(int value);
    void transplant(node *u, node *v);
    node *minimum(node *r);
    node *maximum(node *r);
    bool isancestor(node *u, node *v);

   private:
    node *root;
};

RBTree::RBTree() {
    root = nullptr;
}

bool isancestor(node *u, node *v) {
    /* 判断u是否是v的祖先 */
    if (u == nullptr)
        return false;
    if (v == u->left || v == u->right)
        return true;
    return isancestor(u->left, v) && isancestor(u->right, v);
}

void RBTree::left_rotate(node *x) {
    /* 向左旋转以x为根节点的子树 */
    node *y = x->right;

    x->right = y->left;
    if (y->left != nullptr)
        y->left->parent = x;

    y->parent = x->parent;
    y->left = x;
    x->parent = y;
    if (y->parent == nullptr)
        root = y;
    else {
        if (x == y->parent->left)
            y->parent->left = y;
        else
            y->parent->right = y;
    }

    // 维护size
    y->size = x->size;
    int left_size = 0, right_size = 0;
    if (x->left)
        left_size = x->left->size;
    if (x->right)
        right_size = x->right->size;
    x->size = left_size + right_size + 1;
}

void RBTree::right_rotate(node *y) {
    /* 向右旋转以y为根的子树 */
    node *x = y->left;

    y->left = x->right;
    if (x->right != nullptr)
        x->right->parent = y;

    x->parent = y->parent;
    x->right = y;
    y->parent = x;
    if (x->parent == nullptr)
        root = x;
    else {
        if (y == x->parent->left)
            x->parent->left = x;
        else
            x->parent->right = x;
    }

    // 维护size
    x->size = y->size;
    int left_size = 0, right_size = 0;
    if (y->left)
        left_size = y->left->size;
    if (y->right)
        right_size = y->right->size;
    y->size = left_size + right_size + 1;
}

void RBTree::insert_fixup(node *z) {
    /* 对于z.p是z.p.p的左孩子, 有以下三种case
     * case1: z是红色, z.p是红色, 且z的uncle节点是红色
     * case2: z是红色, z.p是红色, 且z是z.p的右孩子, 且z的uncle节点是黑色
     * case3: z是红色, z.p是红色, 且z是z.p的左孩子, 且z的uncle节点是黑色
     * 维持z为红色
     */
    while (z->parent != nullptr && z->parent->color == RED) {
        if (z->parent->parent == nullptr) {
            /* 进入while循环的前提是z.p存在且z.p为红色
             * 则z.p.p不可能不存在
             * 否则z.p就是根节点, 而根节点不可能是红色
             */
            cout << "你进入了一种不可能发生的状态1, 建议检查一下代码有没有写错" << endl;
            break;
        }
        if (z->parent == z->parent->parent->left) {
            /* 跟else分支的区别就只在于z.p是左孩子还是右孩子 */
            node *y = z->parent->parent->right;  // y是z的uncle节点
            if (y != nullptr && y->color == RED) { // y == nullptr时相当于是黑色
                // case1
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->right) {
                    // case2
                    z = z->parent;
                    left_rotate(z);
                }
                // case3
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                right_rotate(z->parent->parent);
            }
        } else {
            /* 与上面分支left和right互换 */
            node *y = z->parent->parent->left;  // y是z的uncle节点
            if (y != nullptr && y->color == RED) { // y == nullptr时相当于是黑色
                // case1
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->left) {
                    // case2
                    z = z->parent;
                    right_rotate(z);
                }
                // case3
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                left_rotate(z->parent->parent);
            }
        }
    }
    // 不进去while循环或从循环中跳出
    root->color = BLACK;
}

void RBTree::insert(int value) {
    /* 插入一个值为value的节点
     * 注意需要更新新节点所有祖先节点的rank
     */
    node *y = nullptr;
    node *x = root;
    vector<node *> ancestors;
    while (x != nullptr) {
        y = x;
        ancestors.push_back(y);
        if (value < x->value) {
            x = x->left;
        } else {
            x = x->right;
        }
    }
    node *z = new node;
    z->value = value;
    z->left = z->right = nullptr;
    z->parent = y;
    z->size = 1;
    z->color = RED;
    if (y == nullptr) {
        root = z;
        // root->color = BLACK; 留在insert_fixup做
    } else {
        if (z->value < y->value)
            y->left = z;
        else
            y->right = z;
        for (auto anc : ancestors)
            anc->size++;
    }
    insert_fixup(z);
}

node *RBTree::findnode(int value) {
    /* 给定value, 查找树中对应的节点
     * 假定树中节点的value互不相同
     */
    node *now = root;
    while (now != nullptr) {
        if (now->value == value) {
            return now;
        }
        if (value < now->value)
            now = now->left;
        else
            now = now->right;
    }
    return now;
}

void RBTree::transplant(node *u, node *v) {
    if (u->parent == nullptr)
        root = v;
    else if (u == u->parent->left)
        u->parent->left = v;
    else
        u->parent->right = v;
    if (v != nullptr)
        v->parent = u->parent;
}

node *RBTree::minimum(node *r) {
    /* 寻找以r为根节点的子树中拥有最小value的节点 */
    node *now  = r, *p = nullptr;
    while (now) {
        p = now;
        now = now->left;
    }
    return p;
}

node *RBTree::maximum(node *r) {
    node *now = r, *p = nullptr;
    while (now) {
        p = now;
        now = now->right;
    }
    return p;
}

void RBTree::delete_fixup(node *x) {
    node *w = nullptr;
    while (x != root && x->color == BLACK) {
        if (x == x->parent->left) {
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                left_rotate(x->parent);
                w = x->parent->right;
            }
            if (w->left->color == BLACK && w->right->color == BLACK) {
                w->color = RED;
                x = x->parent;
            } else if (w->right->color == BLACK) {
                w->left->color = BLACK;
                w->color = RED;
                right_rotate(w);
                w = x->parent->right;
            }
            w->color = x->parent->color;
            x->parent->color = BLACK;
            w->right->color = BLACK;
            left_rotate(x->parent);
            x = root;
        } else {
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                right_rotate(x->parent);
                w = x->parent->left;
            }
            if (w->right->color == BLACK && w->left->color == BLACK) {
                w->color = RED;
                x = x->parent;
            } else if (w->left->color == BLACK) {
                w->right->color = BLACK;
                w->color = RED;
                left_rotate(w);
                w = x->parent->left;
            }
            w->color = x->parent->color;
            x->parent->color = BLACK;
            w->left->color = BLACK;
            right_rotate(x->parent);
            x = root;
        }
    }
    x->color = BLACK;
}

void RBTree::del(int value) {
    /* 删除指定value对应的节点
     */
    node *z = findnode(value);
    if (z == nullptr)
        return;
    node *y = z, *x = nullptr;
    bool y_original_color = y->color;
    if (z->left == nullptr) {
        x = z->right;
        transplant(z, z->right);
    } else if (z->right == nullptr) {
        x = z->left;
        transplant(z, z->left);
    } else {
        y = minimum(z->right);
        y_original_color = y->color;
        x = y->right;
        if (y->parent == z) {
            x->parent = y;
        } else {
            transplant(y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }
        transplant(z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }
    if (y_original_color == BLACK)
        delete_fixup(x);
}

int RBTree::find(int k) {
    /*  查询树中第k大元素的value */
    int result = 0;
    int r_size = 0, l_size = 0;
    if (root->right)
        r_size = root->right->size;
    if (root->left)
        l_size = root->left->size;
    if (r_size + 1 == k) {
        result = root->value;
    } else if (k <= r_size) {
        node *root_bak = root;
        root = root->right;
        result = RBTree::find(k);
        root = root_bak;
    } else {
        node *root_bak = root;
        root = root->left;
        result = RBTree::find(k - (r_size + 1));
        root = root_bak;
    }
    return result;
}

node *RBTree::findmin(int A) {
    /* 查询>=A的最小元素对应的节点 */
    node *now = root;
    node *min_node = maximum(root);
    int min_value = min_node->value; // 根节点不可能为空 
    while (now) {
        if (now->value == A)
            return now;
        else if (now->value < A) {
            now = now->right;
        } else { // now->value > A
            if (now->value < min_value) {
                min_value = now->value;
                min_node = now;
            }
            now = now->left;
        }
    }
    return min_node;
}

node *RBTree::findmax(int B) {
    /* 查询<=B的最大元素对应的节点 */
    node *now = root;
    node *max_node = minimum(root);
    int max_value = max_node->value; // 根节点不可能为空 
    while (now) {
        if (now->value == B)
            return now;
        else if (now->value > B) {
            now = now->left;
        } else { // now->value < B
            if (now->value > max_value) {
                max_value = now->value;
                max_node = now;
            }
            now = now->right;
        }
    }
    return max_node;
}

int RBTree::getnum(int L, int R) {
    /* 查询[L, R]区间中元素的个数 */
    node *lnode = findmin(L), *rnode = findmax(R);
    int l_size = 0, r_size = 0;
    if (lnode && lnode->right)
        l_size = lnode->right->size + 1;
    if (rnode && rnode->left)
        r_size = rnode->left->size + 1;
    /* 注意考虑rnode是lnode的右后代或者lnode是rnode的左后代的情况 */
    if (isancestor(lnode, rnode))
        r_size = 0;
    else if (isancestor(rnode, lnode))
        l_size = 0;
    // int result = (lnode->right->size+1) + (rnode->left->size+1);
    int result = l_size + r_size;
    return result;
}

int main() {
    RBTree rbtree;
    int m;
    cin >> m;
    while (m--) {
        char op;
        cin >> op;
        if (op == 'I') {
            int value;
            cin >> value;
            rbtree.insert(value);
        } else if (op == 'D') {
            int value;
            cin >> value;
            rbtree.del(value);
        } else if (op == 'S') {
            int k;
            cin >> k;
            cout << rbtree.find(k) << endl;
        } else if (op == 'L') {
            int A;
            cin >> A;
            cout << rbtree.findmin(A)->value << endl;
        } else if (op == 'T') {
            int L, R;
            cin >> L >> R;
            cout << rbtree.getnum(L, R) << endl;
        }
    }
    return 0;
}