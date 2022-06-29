#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>

using namespace std;

#define MAX_SIZE 700000

typedef struct HASH_node
{ 
    HASH_node* next = NULL;
    string name;
    int heap_index; 
} HASH_node;

HASH_node* HASH_table[MAX_SIZE] = {NULL};

typedef struct HEAP_node
{
    string name;
    int score;
}HEAP_node;

HEAP_node Heap[MAX_SIZE];

int hash_function(string name)
{
    int len = name.length();
    int hash_index = 0;
    for(int i = 0; i < len; i++)
    {
        hash_index = 26 * hash_index + (name[i] - 'A' + 1);
    }
    return hash_index % MAX_SIZE;
}

void Update_hash_table(string name, int new_index)
{
   int hash_index = hash_function(name); 

   HASH_node* search_node = HASH_table[hash_index];
   while (search_node != NULL)
   {
       if(search_node->name == name)
       {
           int heap_index = search_node->heap_index;
           search_node->heap_index = new_index;
           return;
       }
       search_node = search_node->next;
   }
}

void Insert(string name, int score)
{
    Heap[0].score ++; //Heap[0].score存储堆的size
    int cur_index = Heap[0].score;
    Heap[cur_index].name = name;
    Heap[cur_index].score = score;
    while(cur_index > 1)
    {
        /*maintain the max heap*/
        int parent = cur_index / 2;
        if(Heap[parent].score < Heap[cur_index].score)
        {
            Update_hash_table(Heap[parent].name, cur_index);
            Update_hash_table(Heap[cur_index].name, parent);
            
            HEAP_node temp;
            temp = Heap[cur_index];
            Heap[cur_index] = Heap[parent];
            Heap[parent] = temp;  
        }
        else
            break;
        cur_index = parent;
    }
    
    int hash_index = hash_function(name);
    if(HASH_table[hash_index] == NULL)
    {
        HASH_table[hash_index] = new HASH_node;
        HASH_table[hash_index]->heap_index = cur_index;
        HASH_table[hash_index]->name = *new string(name);
        HASH_table[hash_index]->next = NULL;
    }
    else
    {
        HASH_node* pre = HASH_table[hash_index];
        HASH_node* cur = pre->next;
        while(cur != NULL)
        {
            pre = cur;
            cur = cur->next;
        }
        cur = new HASH_node;
        cur->heap_index = cur_index;
        cur->name = name;
        cur->next = NULL;
        pre->next = cur;
    }

}

void Delete(string name)
{
    int hash_index = hash_function(name);
    int heap_index;

    /*delete hash_node*/
    HASH_node* delete_node = HASH_table[hash_index];
    if(delete_node == NULL)
    {
        cout << "the node doesn't exist" << endl; 
        return;
    }
    if(delete_node->name == name)
    {
        heap_index = delete_node->heap_index;
        HASH_table[hash_index] = delete_node->next;
        delete delete_node;
    }
    else
    {
        HASH_node* pre = delete_node;
        HASH_node* cur = delete_node->next;
        while(cur)
        {
            if(cur->name == name)
                break;
            pre = cur;
            cur = cur->next;
        }
        pre->next = cur->next;
        heap_index = cur->heap_index;
        delete cur;
    }

    int heap_size = Heap[0].score;
    Heap[heap_index] = Heap[heap_size--];
    Heap[0].score--;

    /*maintain the max heap*/
    int parent = heap_index / 2;
    int lchild = heap_index * 2;
    int rchild = heap_index * 2 + 1;
    if(heap_index > 1 && parent >= 1 && Heap[heap_index].score > Heap[parent].score)
    {//向上走
        while(heap_index > 1 && parent >= 1 && Heap[heap_index].score > Heap[parent].score)
        {
            Update_hash_table(Heap[parent].name, heap_index);
            Update_hash_table(Heap[heap_index].name, parent);
            
            HEAP_node temp = Heap[parent];
            Heap[parent] = Heap[heap_index];
            Heap[heap_index] = temp;
            heap_index = parent;
            parent = heap_index / 2; 
        }
    }
    else if((lchild <= heap_size && Heap[heap_index].score < Heap[lchild].score )
            || (rchild <= heap_size && Heap[heap_index].score < Heap[rchild].score ))
    {//向下走
        while((lchild <= heap_size && Heap[heap_index].score < Heap[lchild].score )
            || (rchild <= heap_size && Heap[heap_index].score < Heap[rchild].score ))
        {   
            HEAP_node temp = Heap[heap_index];
            if(Heap[heap_index].score < Heap[lchild].score)
            {
                Update_hash_table(Heap[lchild].name, heap_index);
                Update_hash_table(Heap[heap_index].name, lchild);

                Heap[heap_index] = Heap[lchild];
                Heap[lchild] = temp;
                heap_index = lchild;
                lchild = heap_index * 2;
                rchild = heap_index * 2 + 1;
            }
            else
            {
                Update_hash_table(Heap[rchild].name, heap_index);
                Update_hash_table(Heap[heap_index].name, rchild);

                Heap[heap_index] = Heap[rchild];
                Heap[rchild] = temp;
                heap_index = rchild;
                lchild = heap_index * 2;
                rchild = heap_index * 2 + 1;
            }  
        }
    }

}

int Search(string name)
{
   int hash_index = hash_function(name); 

   HASH_node* search_node = HASH_table[hash_index];
   while (search_node != NULL)
   {
       if(search_node->name == name)
       {
           int heap_index = search_node->heap_index;
           return Heap[heap_index].score;
       }
       search_node = search_node->next;
   }
   return 0;
}

void Top_score(vector<string> &name_STACK, int cur_root)
{
    name_STACK.push_back(Heap[cur_root].name);
    int lchild = cur_root * 2;
    int rchild = cur_root * 2 + 1;
    if(Heap[cur_root].score == Heap[lchild].score && lchild <= Heap[0].score)
        Top_score(name_STACK, lchild);
    if(Heap[cur_root].score == Heap[rchild].score && rchild <= Heap[0].score)
        Top_score(name_STACK, rchild);
}

int main()
{
    int n;
    cin >> n;
    char op;
    string name;
    int score;
    
    Heap[0].score = 0;
    ofstream output;
    output.open("output.txt", ios::out);
    if(!output.is_open())
    {
        cout << " can't open file" << "\n";
        return 1;
    }
    for(int i = 0; i < n; i++)
    {
        name.clear();
        cin >> op;
        switch (op)
        {
            case 'I':
                    cin >> name;
                    cin >> score;
                    Insert(name, score);  
                    break;
            case 'D':
                    cin >> name;
                    Delete(name);
                    break;
            case 'S':
                    cin >> name;
                    output << Search(name) << endl;
                    break;
            case 'F':
                    vector<string> name_STACK;
                    name_STACK.clear();
                    Top_score(name_STACK, 1);
                    int k = name_STACK.size();
                    sort(name_STACK.begin(), name_STACK.end());
                    for(int i = 0; i < k; i++)
                        output << name_STACK[i] << endl;
                    break;
        }
        
    }
    output.close();
}

