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
    int find(node *r, int k);
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
    node *getroot() { return root; }
    node *successor(node *x);
    node *predecessor(node *x);
    int getrank(node *x);

   private:
    node *root;
    node *NIL;
};

RBTree::RBTree() {
    NIL = new node;
    NIL->left = NIL->right = NIL->parent = nullptr;
    NIL->color = BLACK;
    NIL->size = 0;
    root = NIL;
}

bool RBTree::isancestor(node *u, node *v) {
    /* 判断u是否是v的祖先 */
    if (u == NIL)
        return false;
    if (v == u->left || v == u->right)
        return true;
    return isancestor(u->left, v) && isancestor(u->right, v);
}

void RBTree::left_rotate(node *x) {
    /* 向左旋转以x为根节点的子树 */
    node *y = x->right;

    x->right = y->left;
    if (y->left != NIL)
        y->left->parent = x;

    y->parent = x->parent;
    y->left = x;
    x->parent = y;
    if (y->parent == NIL)
        root = y;
    else {
        if (x == y->parent->left)
            y->parent->left = y;
        else
            y->parent->right = y;
    }

    // 维护size
    y->size = x->size;
    x->size = x->left->size + x->right->size + 1;
}

void RBTree::right_rotate(node *y) {
    /* 向右旋转以y为根的子树 */
    node *x = y->left;

    y->left = x->right;
    if (x->right != NIL)
        x->right->parent = y;

    x->parent = y->parent;
    x->right = y;
    y->parent = x;
    if (x->parent == NIL)
        root = x;
    else {
        if (y == x->parent->left)
            x->parent->left = x;
        else
            x->parent->right = x;
    }

    // 维护size
    x->size = y->size;
    y->size = y->left->size + y->right->size + 1;
}

void RBTree::insert_fixup(node *z) {
    /* 对于z.p是z.p.p的左孩子, 有以下三种case
     * case1: z是红色, z.p是红色, 且z的uncle节点是红色
     * case2: z是红色, z.p是红色, 且z是z.p的右孩子, 且z的uncle节点是黑色
     * case3: z是红色, z.p是红色, 且z是z.p的左孩子, 且z的uncle节点是黑色
     * 维持z为红色
     */
    while (z->parent->color == RED) {
        if (z->parent == z->parent->parent->left) {
            /* 跟else分支的区别就只在于z.p是左孩子还是右孩子 */
            node *y = z->parent->parent->right;     // y是z的uncle节点
            if (y->color == RED) {  
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
        } else if (z->parent == z->parent->parent->right) {
            /* 与上面分支left和right互换 */
            node *y = z->parent->parent->left;      // y是z的uncle节点
            if (y->color == RED) {  
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
    node *y = NIL;
    node *x = root;
    vector<node *> ancestors;
    while (x != NIL) {
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
    z->left = z->right = NIL;
    z->parent = y;
    z->size = 1;
    z->color = RED;
    if (y == NIL) {
        root = z;
        // root->color = BLACK; 留在insert_fixup做
    } else {
        if (z->value < y->value)
            y->left = z;
        else
            y->right = z;
    }
    for (auto anc : ancestors)
        anc->size++;
    insert_fixup(z);
}

node *RBTree::findnode(int value) {
    /* 给定value, 查找树中对应的节点
     * 假定树中节点的value互不相同
     */
    node *now = root;
    while (now != NIL) {
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
    if (u->parent == NIL)
        root = v;
    else if (u == u->parent->left)
        u->parent->left = v;
    else
        u->parent->right = v;
    v->parent = u->parent;
}

node *RBTree::minimum(node *r) {
    /* 寻找以r为根节点的子树中拥有最小value的节点 */
    node *now = r, *p = NIL;
    while (now != NIL) {
        p = now;
        now = now->left;
    }
    return p;
}

node *RBTree::maximum(node *r) {
    /* 寻找以r为根节点的子树中拥有最大value的节点 */
    node *now = r, *p = NIL;
    while (now != NIL) {
        p = now;
        now = now->right;
    }
    return p;
}

void RBTree::delete_fixup(node *x) {
    node *w = nullptr;
    while (x != root && x->color == BLACK) {
        if (x == x->parent->left) {
            w = x->parent->right;
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                left_rotate(x->parent);
                w = x->parent->right;
            }
            if (w->left->color == BLACK && w->right->color == BLACK) {
                w->color = RED;
                x = x->parent;
            } else {
                if (w->right->color == BLACK) {
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
            }
        } else if (x == x->parent->right) {
            w = x->parent->left;
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                right_rotate(x->parent);
                w = x->parent->left;
            }
            if (w->right->color == BLACK && w->left->color == BLACK) {
                w->color = RED;
                x = x->parent;
            } else {
                if (w->left->color == BLACK) {
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
    }
    x->color = BLACK;
}

void RBTree::del(int value) {
    /* 删除指定value对应的节点
     * 注意维护节点的size
     */
    node *z = findnode(value);
    node *y = z, *x = nullptr;
    bool y_original_color = y->color;
    if (z->left == NIL) {
        x = z->right;
        transplant(z, z->right);
        // maintain size
        node *tmp = x->parent;
        while (tmp != NIL) {
            tmp->size--;
            tmp = tmp->parent;
        }
    } else if (z->right == NIL) {
        x = z->left;
        transplant(z, z->left);
        // maintain size
        node *tmp = x->parent;
        while (tmp != NIL) {
            tmp->size--;
            tmp = tmp->parent;
        }
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
            // maintain size
            node *tmp = x->parent;
            while (tmp != y) {
                tmp->size--;
                tmp = tmp->parent;
            }
        }
        transplant(z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;

        y->size = y->left->size + y->right->size + 1;
    }
    if (y_original_color == BLACK)
        delete_fixup(x);
}

int RBTree::find(node *r, int k) {
    /*  查询以r为根的子树中第k大元素的value */
    if (r->right->size + 1 == k) {
        return r->value;
    } else if (k <= r->right->size) {
        return RBTree::find(r->right, k);
    } else {
        return RBTree::find(r->left, k - (r->right->size + 1));
    }
}

node * RBTree::successor(node *x) {
    /* 找节点x的后继 */
    if (x->right != NIL) {
        return minimum(x->right);
    }
    node *y = x->parent;
    while (y != NIL && x == y->right) {
        x = y;
        y = x->parent;
    }
    return y;
}

node * RBTree::predecessor(node *x) {
    /* 找节点x的前驱 */
    if (x->left != NIL) {
        return maximum(x->left);
    }
    node *y = x->parent;
    while (y != NIL && x == y->left) {
        x = y;
        y = x->parent;
    }
    return y;
}

node *RBTree::findmin(int A) {
    /* 查询>=A的最小元素对应的节点 */
    // TODO:to check
    node *now = root;
    // node *pre = NIL;
    node *min_node = maximum(root);
    int min_value = min_node->value;  // 根节点不可能为空
    while (now != NIL) {
        if (now->value == A)
            return now;
        else if (now->value < A) {
            now = now->right;
            /*
            pre = now;
            now = successor(now);
            */
        } else {  // now->value > A
            
            if (now->value < min_value) {
                min_value = now->value;
                min_node = now;
            }
            now = now->left;
            /*
            if (pre != NIL && pre->value < A)
                return now;
            else {
                node *tmp = predecessor(now);
                if (tmp == NIL)
                    return now;
                else
                    now = tmp;
            }
            */
        }
    }
    return min_node;
}

node *RBTree::findmax(int B) {
    /* 查询<=B的最大元素对应的节点 */
    // TODO:to check
    node *now = root;
    node *pre = NIL;
    node *max_node = minimum(root);
    int max_value = max_node->value;  // 根节点不可能为空
    while (now != NIL) {
        if (now->value == B)
            return now;
        else if (now->value > B) {
            now = now->left;
            /*
            pre = now;
            now = predecessor(now);
            */
        } else {  // now->value < B
            if (now->value > max_value) {
                max_value = now->value;
                max_node = now;
            }
            now = now->right;
            /*
            if (pre != NIL && pre->value > B)
                return now;
            else {
                node *tmp = successor(now);
                if (tmp == NIL)
                    return now;
                else
                    now = tmp;
            }
            */
        }
    }
    return max_node;
}

int RBTree::getrank(node *x) {
    /* 查询x的顺序 */
    int r = x->left->size + 1;
    node *y = x;
    while (y != root) {
        if (y == y->parent->right) {
            r += y->parent->left->size + 1;
        }
        y = y->parent;
    }
    return r;
}

int RBTree::getnum(int L, int R) {
    /* 查询[L, R]区间中元素的个数 
     * 查询>=L的最小节点lnode, <=R的最大节点rnode
     * 结果即为rnode.rank - lnode.rank + 1
     */
    node *lnode = findmin(L);
    // node *rnode = findmax(R);
    node *rnode = findmin(R);
    int lnum = getrank(lnode);
    int rnum = getrank(rnode);
    if (findnode(R) != NIL)
        rnum++;
    return rnum - lnum;
}

int main() {
    vector<int> result;
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
            result.push_back(rbtree.find(rbtree.getroot(), k));
        } else if (op == 'L') {
            int A;
            cin >> A;
            result.push_back(rbtree.findmin(A)->value);
        } else if (op == 'T') {
            int L, R;
            cin >> L >> R;
            result.push_back(rbtree.getnum(L, R));
        }
    }
    for (auto res : result) {
        cout << res << endl;
    }
    return 0;
}