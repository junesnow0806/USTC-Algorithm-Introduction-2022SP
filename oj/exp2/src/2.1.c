#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_ITEM_SIZE 700000

typedef struct hash_item
{
    char *key;
    int score;
    int pos;
    struct hash_item *next;
} hash_item;
hash_item hash_table[MAX_ITEM_SIZE];
char *(heap[MAX_ITEM_SIZE]);
int heap_size = 0;

char name_arr[MAX_ITEM_SIZE][6]; // name_arr is used to sort names with higest score
int name_arr_size = 0;
int name_arr_stack[MAX_ITEM_SIZE]; // name_arr_stack is used to get names with highest score
int name_arr_stack_size = 0;

unsigned int get_index(char *key)
{
    char *str = key;
    unsigned int seed = 31;
    unsigned int index = 0;
    while (*str)
    {
        index = index * seed + (*str++);
    }
    return index % MAX_ITEM_SIZE;
}
hash_item *hash_get_pointer(char *key, int create_if_not_exist)
{
    unsigned int index = get_index(key);
    hash_item *p = &hash_table[index];
    while (1)
    {
        if (p->key == NULL) // last node
        {
            if (create_if_not_exist)
            {
                p->key = (char *)malloc(sizeof(char) * (strlen(key) + 1));
                memcpy(p->key, key, sizeof(char) * (strlen(key) + 1));
                p->next = (hash_item *)malloc(sizeof(hash_item));
                p->next->key = NULL;
                return p;
            }
            else
                return NULL;
        }
        if (strcmp(p->key, key) == 0)
        {
            return p;
        }
        p = p->next;
    }
    return p;
}
void hash_delete_pointer(char *key) // if not found, then create
{

    hash_item *p = &hash_table[get_index(key)];
    while (1)
    {
        if (p->key == NULL) // last node
        {
            return;
        }
        if (strcmp(p->key, key) == 0)
        {
            free(p->key);
            p->key = p->next->key;
            p->pos = p->next->pos;
            p->score = p->next->score;
            hash_item *next = p->next;
            p->next = next->next;
            free(next);
            return;
        }
        p = p->next;
    }
}

int hash_get(char *key, int is_score)
{
    hash_item *p = hash_get_pointer(key, 0);
    if (p == NULL)
        return 0;
    return is_score ? p->score : p->pos;
}
void hash_set(char *key, int value, int is_score)
{
    hash_item *p = hash_get_pointer(key, 1);
    if (is_score)
        p->score = value;
    else
        p->pos = value;
}
void hash_delete(char *key)
{
    hash_delete_pointer(key);
}

void remove(char *name)
{
    int pos = hash_get(name, 0);
    heap[pos] = heap[heap_size - 1];
    hash_set(heap[heap_size - 1], pos, 0);
    hash_delete_pointer(name);
    heap_size--;

    while (1)
    {
        if (pos > 0 && hash_get(heap[pos], 1) > hash_get(heap[(pos - 1) / 2], 1))
        {
            char *tmp = heap[pos];
            heap[pos] = heap[(pos - 1) / 2];
            heap[(pos - 1) / 2] = tmp;
            hash_set(heap[pos], pos, 0);
            hash_set(heap[(pos - 1) / 2], (pos - 1) / 2, 0);
            pos = (pos - 1) / 2;
        }
        else if (pos * 2 + 2 < heap_size && hash_get(heap[pos], 1) < hash_get(heap[pos * 2 + 2], 1) && hash_get(heap[pos * 2 + 1], 1) <= hash_get(heap[pos * 2 + 2], 1))
        {
            char *tmp = heap[pos];
            heap[pos] = heap[pos * 2 + 2];
            heap[pos * 2 + 2] = tmp;
            hash_set(heap[pos], pos, 0);
            hash_set(heap[pos * 2 + 2], pos * 2 + 2, 0);
            pos = pos * 2 + 2;
        }
        else if (pos * 2 + 1 < heap_size && hash_get(heap[pos], 1) < hash_get(heap[pos * 2 + 1], 1) && (pos * 2 + 2 >= heap_size || hash_get(heap[pos * 2 + 1], 1) >= hash_get(heap[pos * 2 + 2], 1)))
        {
            char *tmp = heap[pos];
            heap[pos] = heap[pos * 2 + 1];
            heap[pos * 2 + 1] = tmp;
            hash_set(heap[pos], pos, 0);
            hash_set(heap[pos * 2 + 1], pos * 2 + 1, 0);
            pos = pos * 2 + 1;
        }
        else
            break;
    }
}
void verify()
{
    for (int i = heap_size - 1; i >= 0; i--)
    {
        if (hash_get(heap[(i - 1) / 2], 1) < hash_get(heap[i], 1))
        {
            break;
        }
    }
}
void insert(char *name, int score)
{
    hash_set(name, score, 1);
    int name_size = sizeof(char) * (strlen(name) + 1);
    heap[heap_size] = (char *)malloc(name_size);
    memcpy(heap[heap_size], name, name_size);
    hash_set(name, heap_size, 0);

    int pos = heap_size;
    while (pos > 0 && hash_get(heap[pos], 1) > hash_get(heap[(pos - 1) / 2], 1))
    {
        char *tmp = heap[pos];
        heap[pos] = heap[(pos - 1) / 2];
        heap[(pos - 1) / 2] = tmp;
        hash_set(heap[pos], pos, 0);
        hash_set(heap[(pos - 1) / 2], (pos - 1) / 2, 0);
        pos = (pos - 1) / 2;
    }
    heap_size++;
}
void name_arr_insert(char *name)
{
    int len = strlen(name);
    for (int i = 0; i < 6; i++)
    {
        if (i < len)
            name_arr[name_arr_size][i] = name[i];
        else
            name_arr[name_arr_size][i] = '\0';
    }
    name_arr_size++;
}
int query(char *name)
{
    return hash_get(name, 1);
}
char b[MAX_ITEM_SIZE][6];
void counting_sort(char a[][6], int alen, int ii)
{
    int c[27];

    for (int i = 0; i <= 27; i++)
        c[i] = 0;
    for (int j = 0; j < alen; j++)
    {
        c[a[j][ii] > 0 ? (a[j][ii] - 'A' + 1) : 0]++;
    }
    for (int i = 1; i <= 27; i++)
        c[i] += c[i - 1];
    for (int j = alen - 1; j >= 0; j--)
    {
        memcpy(b[c[a[j][ii] > 0 ? (a[j][ii] - 'A' + 1) : 0]], a[j], sizeof(char) * 5);
        c[a[j][ii] > 0 ? (a[j][ii] - 'A' + 1) : 0]--;
    }
    for (int i = 0; i < alen; i++)
        memcpy(a[i], b[i + 1], sizeof(char) * 5);
}
void print_highest()
{
    if (heap_size == 0)
        return;
    name_arr_size = 0;
    name_arr_stack_size = 0;
    name_arr_stack[name_arr_stack_size++] = 0;

    int score = hash_get(heap[0], 1);
    while (name_arr_stack_size > 0)
    {
        int this_one = name_arr_stack[--name_arr_stack_size];
        name_arr_insert(heap[this_one]);
        if (this_one * 2 + 2 < heap_size && hash_get(heap[this_one * 2 + 2], 1) == score)
            name_arr_stack[name_arr_stack_size++] = this_one * 2 + 2;
        if (this_one * 2 + 1 < heap_size && hash_get(heap[this_one * 2 + 1], 1) == score)
            name_arr_stack[name_arr_stack_size++] = this_one * 2 + 1;
    }
    for (int i = 4; i >= 0; i--)
        counting_sort(name_arr, name_arr_size, i);
    for (int i = 0; i < name_arr_size; i++)
        printf("%s\n", name_arr[i]);
}
int main()
{
    freopen("input3.txt", "r", stdin);
    freopen("output3_my.txt", "w", stdout);
    int n;
    scanf("%d", &n);
    for (int i = 0; i < n; i++)
    {
        char action[2];
        char name[6];
        int score;
        scanf("%s", action);
        switch (action[0])
        {
        case 'I':
        case 'D':
        case 'S':
            scanf("%s", name);
        }
        if (action[0] == 'I')
            scanf("%d", &score);
        switch (action[0])
        {
        case 'I':
            insert(name, score);
            //verify();
            break;
        case 'D':
            remove(name);
            //verify();
            break;
        case 'S':
            printf("%d\n", query(name));
            break;
        case 'F':
            print_highest();
            break;
        default:
            break;
        }
    }
    return 0;
}