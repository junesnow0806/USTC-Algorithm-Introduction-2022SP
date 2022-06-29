#include <iostream>
using namespace std;
 
enum Color {RED, BLACK};
 
struct node
{
	int key;
	Color color;
	node *p;
	node *left;
	node *right;
	int size;
	node():key(0), color(RED), p(nullptr), left(nullptr), right(nullptr), size(1){}
	node(node *temp, int x):key(x), color(RED), p(temp), left(temp), right(temp), size(1){}  
	node(node* temp, int x, Color color, int size) : key(x), color(color), left(temp), right(temp), p(temp), size(size) {}
};

class OS_Tree
{
public:
	node *root;
	node *nil;
	OS_Tree(){nil = new node(nullptr, -1, BLACK, 0);root = nil;};  
	node *OS_Select_min(node *x, int i);	
	node *OS_Select_max(node *x, int i);	
	int OS_Rank(node *x);
    node* OS_Check(int x);
	void Left_Rotate(node *x);
	void Right_Rotate(node *x);
	void OS_Insert_Fixup(node *z);
	void OS_Insert(node *z);
	void OS_TransPlant(node* u, node* v);
	void OS_Delete_Fixup(node *x);
	void OS_Delete(node *z);
	node *OS_Search(node *x, int k);
	node *Tree_Successor(node *x);
	node *Tree_Minimum(node *x);
    node *Tree_Predecessor(node *x);
	node *Tree_Maximum(node *x);
};

void OS_Tree::Left_Rotate(node *x)
{
	node *y = x->right;
	x->right = y->left;
	if(y->left != nil)
		y->left->p = x;
	y->p = x->p;
	if(x->p == nil)
		root = y;
	else if(x == x->p->left)
		x->p->left = y;
	else 
		x->p->right = y;
	y->left = x;
	x->p = y;
	y->size = x->size;
	x->size = x->left->size + x->right->size + 1;
}

void OS_Tree::Right_Rotate(node *x)
{
	node *y = x->left;
	x->left = y->right;
	if(y->right != nil)
		y->right->p = x;
	y->p = x->p;
	if(x->p == nil)
		root = y;
	else if(x == x->p->right)
		x->p->right = y;
	else 
		x->p->left = y;
	y->right = x;
	x->p = y;
	y->size = x->size;
	x->size = x->left->size + x->right->size + 1;
}

void OS_Tree::OS_Insert_Fixup(node *z)
{
	node *y;
	while(z->p->color == RED)
	{
		if(z->p == z->p->p->left)
		{
			y = z->p->p->right;
			if(y->color == RED)
			{
				z->p->color = BLACK;
				y->color = BLACK;
				z->p->p->color = RED;
				z = z->p->p;
			}
			else
			{
				if(z == z->p->right)
				{
					z = z->p;
					Left_Rotate(z);
				}
				z->p->color = BLACK;
				z->p->p->color = RED;
				Right_Rotate(z->p->p);
			}
		}
		else if(z->p == z->p->p->right)
		{
			y = z->p->p->left;
			if(y->color == RED)
			{
				z->p->color = BLACK;
				y->color = BLACK;
				z->p->p->color = RED;
				z = z->p->p;
			}
			else
			{
				if(z == z->p->left)
				{
					z = z->p;
					Right_Rotate(z);
				}
				z->p->color = BLACK;
				z->p->p->color = RED;
				Left_Rotate(z->p->p);
			}
		}
	}
	root->color = BLACK;
}

void OS_Tree::OS_Insert(node *z)
{
	node *y = nil, *x = root;
	while(x != nil)
	{
		x->size++;
		y = x;
		if(z->key < x->key)
			x = x->left;
		else
			x = x->right;
	}
	z->p = y;
	if(y == nil)
		root = z;
	else if(z->key < y->key)
		y->left = z;
	else
		y->right = z;
	z->left = nil;
	z->right = nil;
	OS_Insert_Fixup(z);
}

void OS_Tree::OS_Delete_Fixup(node *x)
{
	node *w;
	while(x != root && x->color == BLACK)
	{
		if(x == x->p->left)
		{
			w = x->p->right;
			if(w->color == RED)
			{
				w->color = BLACK;
				x->p->color = RED;
				Left_Rotate(x->p);
				w = x->p->right;
			}
			if(w->left->color == BLACK && w->right->color == BLACK)
			{
				w->color = RED;
				x = x->p;
			}
			else
			{
				if(w->right->color == BLACK)
				{
					w->left->color = BLACK;
					w->color = RED;
					Right_Rotate(w);
					w = x->p->right;
				}
				w->color =x->p->color;
				x->p->color = BLACK;
				w->right->color = BLACK;
				Left_Rotate(x->p);
				x = root;
			}
		}
		else if(x == x->p->right)
		{
			w = x->p->left;
			if(w->color == RED)
			{
				w->color = BLACK;
				x->p->color = RED;
				Right_Rotate(x->p);
				w = x->p->left;
			}
			if(w->right->color == BLACK && w->left->color == BLACK)
			{
				w->color = RED;
				x = x->p;
			}
			else
			{
				if(w->left->color == BLACK)
				{
					w->right->color = BLACK;
					w->color = RED;
					Left_Rotate(w);
					w = x->p->left;
				}
				w->color =x->p->color;
				x->p->color = BLACK;
				w->left->color = BLACK;
				Right_Rotate(x->p);
				x = root;
			}
		}
	}
	x->color = BLACK;
} 

node *OS_Tree::Tree_Minimum(node *x)  
{   
	while(x->left != nil)  
        x = x->left;  
    return x;  
} 

node* OS_Tree::Tree_Maximum(node *x){
    while(x->right != nil)
        x=x->right;
    return x;
}  

node *OS_Tree::Tree_Successor(node *x)  
{    
	if(x->right != nil)   
        return Tree_Minimum(x->right);   
    node *y = x->p;  
    while(y != nil && x == y->right)  
    {  
        x = y;  
        y = y->p;  
    }  
    return y;  
}  

node *OS_Tree::Tree_Predecessor(node* x)
{
    if(x->left != nil)
        return Tree_Maximum(x->left);
    node* y=x->p;
    while(y != nil && x == y->left)
    {
        x = y;
        y = y->p;
    }
    return y;
}  

node *OS_Tree::OS_Search(node *x, int k)  
{     
	if(x == nil || k == x->key)  
        return x;    
    if(k < x->key)  
        return OS_Search(x->left, k);    
    else  
        return OS_Search(x->right, k);  
} 

void OS_Tree::OS_TransPlant(node* u, node* v){
    if(u->p == nil){
        root = v;
    }
    else if(u == u->p->left){
        u->p->left = v;
    }
    else{
        u->p->right = v;
    }
    v->p = u->p;
}

void OS_Tree::OS_Delete(node *z)
{
	node* x;
    node* y = z;
    Color y_orignal_color = y->color;
    if(z->left == nil){
        x = z->right;
        OS_TransPlant(z, z->right);
        node* Temp = x->p;
        while(Temp != nil){
            Temp->size = Temp->size - 1;
            Temp = Temp->p;
        }
    }
    else if(z->right == nil){
        x = z->left;
        OS_TransPlant(z, z->left); 
        node* Temp = x->p;
        while(Temp != nil){
            Temp->size = Temp->size - 1;
            Temp = Temp->p;
        }
    }
    else{
        y = Tree_Minimum(z->right);
        y_orignal_color = y->color;
        x = y->right;
        if(y->p = z){
            x->p = y;
        }
        else{
            OS_TransPlant(y, y->right);
            y->right = z->right;
            y->right->p = y;

            node* Temp = x->p;
            while(Temp != y){
                Temp->size = Temp->size - 1;
                Temp = Temp->p;
            }
        }
        OS_TransPlant(z, y);
        y->left = z->left;
        y->left->p = y;
        y->color = z->color;
        y->size = y->left->size + y->right->size + 1;
    }
    if(y_orignal_color == BLACK){
        OS_Delete_Fixup(x);
    }
}

node *OS_Tree::OS_Select_min(node *x, int i)
{
	int r = x->left->size +1;
	if(r == i)
		return x;
	else if(i < r)
		return OS_Select_min(x->left, i);
	else
		return OS_Select_min(x->right, i - r);
}

node *OS_Tree::OS_Select_max(node *x, int i)
{
	int r = x->right->size +1;
	if(r == i)
		return x;
	else if(i < r)
		return OS_Select_max(x->right, i);
	else
		return OS_Select_max(x->left, i - r);
}

int OS_Tree::OS_Rank(node *x)
{
	int r = x->left->size + 1;
	node *y = x;
	while(y != root)
	{
		if(y == y->p->right)
			r = r + y->p->left->size + 1;
		y = y->p;
	}
	return r;
}

node* OS_Tree::OS_Check(int A){	
    node* y = nil;
    node* x = root;
    while(x != nil){
        y = x;
        if(A == x->key){
            return x;
        }
        else if(A < x->key){
            x = x->left;
        }
        else{
            x = x->right;
        }
    }
    if(y == nil){
        return nil;
    }
    else if(A > y->key){
        return Tree_Successor(y);
    }
    else{
        return y;
    }
}

int main(){
    char in;
	int x;
	int L, R;
	int l, r;
	int n;
    cin >> n;
	OS_Tree *T = new OS_Tree;

	for(int i = 0; i < n; i++)
	{
		cin>>in;
		switch(in)
		{
		case 'I':
			{
				cin>>x;
				node *z = new node(T->nil, x);
				T->OS_Insert(z);
				break;
			}
		case 'D':
			{
				cin>>x;
				node *z = T->OS_Search(T->root, x);
				if(z == T->nil)
					break;
				else
					T->OS_Delete(z);
				break;
			}
		case 'S':
			{
				cin>>x;
				node *z = T->OS_Select_max(T->root, x);
				cout<< z->key <<endl;
				break;
			}
		case 'L':
			{
				cin>>x;
                node *z = T->OS_Check(x);
				cout << z->key << endl;
				break;	
			}
		case 'T':
			{
			cin >> L;
			cin >> R;
			node* Lnode = T->OS_Check(L);
			node* Rnode = T->OS_Check(R);
			l = T->OS_Rank(Lnode);
			r = T->OS_Rank(Rnode);
			if(T->OS_Search(T->root, R) != T->nil)
				r++;
			//l = T->RBSearchKeyth(T->root, L, 0);
            //r = T->RBSearchKeyth(T->root, R, 1);
            cout << (r - l) << endl;
			break;
			}
		default:
			break;
		}
	}
	return 0;
}