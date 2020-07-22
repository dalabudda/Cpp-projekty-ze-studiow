#include <stdio.h>
#include <string>

#define MAX_NODES 100

struct Node
{
    int value = 0;
    Node* parent = 0;
    Node* brother = 0;
    Node* son = 0;
};

struct Tree
{
    int number_of_leaves = 0;
    int number_of_branches = 1;
    Node tree;
    Node* leaves[MAX_NODES];
    Node* branches[MAX_NODES];
};

void constructTree(const char* line, int &i, Tree &tree, Node* parent)
{
    Node* node = new Node;
    node->parent = parent;
    parent->son = node;
    char id[8];
    int id_len = 0;
    i++;
    while (line[i] != ')')
    {
        if (line[i] == '(')
        {
            node->value = -tree.number_of_branches;
            tree.branches[tree.number_of_branches] = node;
            tree.number_of_branches++;
            constructTree(line, i, tree, node);
        }
        else if (line[i] >= '0' && line[i] <= '9')
        {
            id[id_len] = line[i];
            id_len++;
        }
        else if (line[i] == ',')
        {
            if (id_len)
            {
                tree.leaves[tree.number_of_leaves] = node;
                tree.number_of_leaves++;
                id[id_len] = 0;
                node->value = std::stoi(id);
                id_len = 0;
            }
            node->brother = new Node;
            node = node->brother;
            node->parent = parent;
        }
        i++;
    }
    if (id_len)
    {
        tree.leaves[tree.number_of_leaves] = node;
        tree.number_of_leaves++;
        id[id_len] = 0;
        node->value = std::stoi(id);
        id_len = 0;
    }
}

void destructTree(Node* node)
{
    if (node->brother)
        destructTree(node->brother);
    if (node->son)
        destructTree(node->son);
    delete node;
}

void createSiblingsTable(Node* node, Node** siblings, int &counted)
{
    while (node)
    {
        siblings[counted] = node;
        counted++;
        node = node->brother;
    }
}

void lookForLeaf(const int key, const Node* node, int &found)
{
    if (!node)
        return;
    if (node->value == key)
    {
        found = 1;
        return;
    }
    lookForLeaf(key, node->son, found);
    lookForLeaf(key, node->brother, found);
}

int lookForIndex(Node* const* table, const int key)
{
    for (int i = 0; i < MAX_NODES; i++)
        if (table[i]->value == key)
            return i;
    return -1;
}

void lookForMaxSum(int y, const int maxY, const int maxX, int** mini_table, int* restricted, int &maxSum)
{
    if (y >= maxY)
    {
        int sum = 0;
        for (int j = 0; j < maxY; j++)
        {
            if (restricted[j] >= 0)
                sum += mini_table[j][restricted[j]];
        }
        if (sum > maxSum)
            maxSum = sum;
        return;
    }
    for (int i = -1; i < maxX; i++)
    {
        bool isRestricted = false;
        if (i >= 0)
        {
            for (int j = 0; j < y; j++)
            {
                if (restricted[j] == i)
                {
                    isRestricted = true;
                    break;
                }
            }
        }
        if (!isRestricted)
        {
            restricted[y] = i;
            lookForMaxSum(y+1, maxY, maxX, mini_table, restricted, maxSum);
        }
    }
}

int getMinLeavesToRemove(const Tree &left, const Tree &right)
{
    int maxLeaves = 0;
    int t = 0;//top
    int b = 1;//bottom
    int l = 0;//left
    int r = 1;//right
    int** table[2][2];
    //top left
    table[t][l] = new int*[left.number_of_leaves];
    for (int y = 0; y < left.number_of_leaves; y++)
    {
        table[t][l][y] = new int[right.number_of_leaves];
        for (int x = 0; x < right.number_of_leaves; x++)
        {
            table[t][l][y][x] = (left.leaves[y]->value == right.leaves[x]->value)? 1: 0;
        }
    }
    //top right
    table[t][r] = new int*[left.number_of_leaves];
    for (int y = 0; y < left.number_of_leaves; y++)
    {
        table[t][r][y] = new int[right.number_of_branches];
        for (int x = 0; x < right.number_of_branches; x++)
        {
            table[t][r][y][x] = 0;
            lookForLeaf(left.leaves[y]->value, right.branches[x]->son, table[t][r][y][x]);
        }
    }
    //bottom left
    table[b][l] = new int*[left.number_of_branches];
    for (int y = 0; y < left.number_of_branches; y++)
    {
        table[b][l][y] = new int[right.number_of_leaves];
        for (int x = 0; x < right.number_of_leaves; x++)
        {
            table[b][l][y][x] = 0;
            lookForLeaf(right.leaves[x]->value, left.branches[y]->son, table[b][l][y][x]);
        }
    }
    //bottom right
    table[b][r] = new int*[left.number_of_branches];
    for (int y = 0; y < left.number_of_branches; y++)
    {
        table[b][r][y] = new int[right.number_of_branches];
        for (int x = 0; x < right.number_of_branches; x++)
        {
            table[b][r][y][x] = -1;
        }
    }
    for (int y = left.number_of_branches-1; y >= 0; y--)
    {
        for (int x = right.number_of_branches-1; x >= 0; x--)
        {
            int left_siblings_number = 0;
            Node* left_siblings[MAX_NODES];
            createSiblingsTable(left.branches[y]->son, left_siblings, left_siblings_number);
            int right_siblings_number = 0;
            Node* right_siblings[MAX_NODES];
            createSiblingsTable(right.branches[x]->son, right_siblings, right_siblings_number);
            int** mini_table = new int*[left_siblings_number];
            for (int Y = 0; Y < left_siblings_number; Y++)
            {
                mini_table[Y] = new int[right_siblings_number];
                for (int X = 0; X < right_siblings_number; X++)
                {
                    int tb, lr, lookY, lookX;
                    if (left_siblings[Y]->value > 0)
                    {
                        tb = 0;
                        lookY = lookForIndex(left.leaves, left_siblings[Y]->value);
                    }
                    else
                    {
                        tb = 1;
                        lookY = lookForIndex(left.branches, left_siblings[Y]->value);
                    }
                    if (right_siblings[X]->value > 0)
                    {
                        lr = 0;
                        lookX = lookForIndex(right.leaves, right_siblings[X]->value);
                    }
                    else
                    {
                        lr = 1;
                        lookX = lookForIndex(right.branches, right_siblings[X]->value);
                    }

                    if (lookY >= 0 && lookX >= 0)
                    {
                        if (table[tb][lr][lookY][lookX] == -1)
                            mini_table[Y][X] = -1;
                        else
                            mini_table[Y][X] = table[tb][lr][lookY][lookX];
                    }

                    else
                        mini_table[Y][X] = 0;
                }
            }

            /*for (int Y = 0; Y < left_siblings_number; Y++)
            {
                for (int X = 0; X < right_siblings_number; X++)
                {
                    printf("%d ", mini_table[Y][X]);
                }
                printf("\n");
            }
            printf("\n");*/

            int maxSum = 0;
            int restricted[MAX_NODES];
            for (int i = 0; i < MAX_NODES; i++)
                restricted[i] = -1;
            lookForMaxSum(0, left_siblings_number, right_siblings_number, mini_table, restricted, maxSum);

            for (int Y = 0; Y < left_siblings_number; Y++)
            {
                if (left_siblings[Y]->value > 0)
                {
                    int lookY = lookForIndex(left.leaves, left_siblings[Y]->value);
                    if (table[t][r][lookY][x] > maxSum)
                        maxSum = table[t][r][lookY][x];
                }
                else
                {
                    int lookY = lookForIndex(left.branches, left_siblings[Y]->value);
                    if (table[b][r][lookY][x] > maxSum)
                        maxSum = table[b][r][lookY][x];
                }
            }
            for (int X = 0; X < right_siblings_number; X++)
            {
                if (right_siblings[X]->value > 0)
                {
                    int lookX = lookForIndex(right.leaves, right_siblings[X]->value);
                    if (table[b][l][y][lookX] > maxSum)
                        maxSum = table[b][l][y][lookX];
                }
                else
                {
                    int lookX = lookForIndex(right.branches, right_siblings[X]->value);
                    if (table[b][r][y][lookX] > maxSum)
                        maxSum = table[b][r][y][lookX];
                }
            }

            table[b][r][y][x] = maxSum;
            if (maxSum > maxLeaves)
                maxLeaves = maxSum;

            for (int Y = 0; Y < left_siblings_number; Y++)
                delete[] mini_table[Y];
            delete[] mini_table;
        }
    }

    /*for (int y = 0; y < left.number_of_branches; y++)
    {
        for (int x = 0; x < right.number_of_branches; x++)
        {
            printf("%d ", table[b][r][y][x]);
        }
        printf("\n");
    }*/

    for (int y = 0; y < left.number_of_leaves; y++)
        delete[] table[t][l][y];
    delete[] table[t][l];
    for (int y = 0; y < left.number_of_leaves; y++)
        delete[] table[t][r][y];
    delete[] table[t][r];
    for (int y = 0; y < left.number_of_branches; y++)
        delete[] table[b][l][y];
    delete[] table[b][l];
    for (int y = 0; y < left.number_of_branches; y++)
        delete[] table[b][r][y];
    delete[] table[b][r];

    return left.number_of_leaves - maxLeaves;
}

int main()
{
    int number_of_trees;
    scanf("%d", &number_of_trees);
    Tree* trees = new Tree[number_of_trees];
    for (int i = 0; i < number_of_trees; i++)
    {
        char line[1024];
        scanf("%s", line);
        int k = 0;
        trees[i].branches[0] = &trees[i].tree;
        constructTree(line, k, trees[i], &trees[i].tree);
    }

    for (int i = 0; i < number_of_trees-1; i++)
        for (int j = i+1; j < number_of_trees; j++)
            printf("%d\n", getMinLeavesToRemove(trees[i], trees[j]));

    for (int i = 0; i < number_of_trees; i++)
        destructTree(trees[i].tree.son);
    delete[] trees;

    return 0;
}
