// Towards GPGPU JSON parsing C++

#include <iostream>
#include <list>

using namespace std;
void CNCND(char *inputsymble, int *node, int *depth, int n, int *depth_true)//, int *index, int *depth_true, int counter_number)
{
    int j = 0;
    for (int i = 0; i < n - 1; i++)
    {
        node[i] = 0;
        depth[i] = 0;
        depth_true[i] = 0;
    }

    for (int i = 0; i < n - 1; i++)
    {
        if (inputsymble[i] == '[')
        {
            node[i + 1] = node[i] + 1;
            depth[i + 1] = depth[i] + 1;
            depth_true[j] = depth[i];
            j++;
        }
        if (inputsymble[i] == ']')
        {
            node[i + 1] = node[i];
            depth[i + 1] = depth[i] - 1;
        }
    }
    //return(counter_number);
}
int counter_number(char *inputsymble, int n, int *index)
{
    int j = 0;
    for (int i = 0; i < n; i++)
    {
        index[i] = 0;
    }
    for (int i = 0; i < n - 1; i++)
    {
        if (inputsymble[i] == '[')
        {
            j++;
        }
    }
    for (int i = 0; i < j; i++)
    {
        index[i] = i;
    }
    return(j);
}
void BFS(int *index, int *depth, int *bfs, int n)
{
    int temparray[n];
    for (int i = 0; i < n; i++)
    {
        temparray[i] = depth[i];
    }
    
    for (int i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < n - i - 1; j++)
        {
            if (temparray[j] > temparray[j + 1])
            {
                int temp = temparray[j];
                temparray[j] = temparray[j + 1];
                temparray[j + 1] = temp;
                temp = index[j];
                index[j] = index[j + 1];
                index[j + 1] = temp;
            }
        }
    }
    for (int i = 0; i < n; i++)
    {
        bfs[index[i]] = i;
    }
}
void DPOFC(int *bfs, int *depth, int *par, int n)
{
    for (int i = 0; i < n; i++)
    {
        par[i] = 0;
    }

    for (int i = 1; i < n; i++)
    {
        if (depth[i] == depth[i - 1] + 1)
        {
            par[i] = bfs[i - 1];
        }
        else
        {
            par[i] = 100;
        }
    }
}

void sort(int *bfs, int *depth, int *par, int n)
{
    for (int i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < n - i - 1; j++)
        {
            if (bfs[j] > bfs[j + 1])
            {
                int temp = bfs[j];
                bfs[j] = bfs[j + 1];
                bfs[j + 1] = temp;
                // temp = depth[j];
                // depth[j] = depth[j + 1];
                // depth[j + 1] = temp;
                temp = par[j];
                par[j] = par[j + 1];
                par[j + 1] = temp;
            }
        }
    }
}

void generate_parent(int *bfs, int *par, int n, int *parent, int *count)
{
    for (int i = 0; i < n; i++)
    {
        parent[i] = 0;
        count[i] = 0;
    }
    for (int i = 1; i < n; i++)
    {
        if ((par[i] == par[i - 1]) || (par[i] == 100))
        {
            count[i] = count[i - 1] + 1;
            parent[i] = parent[i - 1];
        }
        else
        {
            count[i] = 1;
            parent[i] = par[i];
        }
    }
}

void scatter_children(int *bfs, int *parent, int *count, int *nchild, int n)
{
    for (int i = 0; i < n; i++)
    {
        nchild[i] = 0;
    }
    for (int i = 0; i < n; i++)
    {
        if (parent[i] != parent[i + 1])
        {
            nchild[parent[i]] = count[bfs[i]];
        }
    }
}

void allocate(int *alloc, int *nchild, int n)
{
    alloc[0] = 0;
    for (int i = 1; i < n; i++)
    {
        alloc[i] = nchild[i - 1] + alloc[i - 1] + 1;
    }
}

int main()
{
    int n = 18;
    int index[n];
    int counter = 0;
    int depth_true[n];
    int node[n];
    int depth[n];
    char inputsymble[n] = {
        '[',
        '[',
        ']',
        '[',
        '[',
        ']',
        '[',
        ']',
        '[',
        '[',
        ']',
        ']',
        ']',
        '[',
        ']',
        '[',
        ']',
        ']',
    };
    CNCND(inputsymble, node, depth, n,depth_true);
    counter = counter_number(inputsymble,n,index);
    int bfs[counter];
    BFS(index, depth_true, bfs, counter);
    int par[counter];
    int parent[counter];
    int count[counter];
    int nchild[counter];
    int alloc[counter];
    DPOFC(bfs, depth_true, par, counter);
    sort(bfs, depth_true, par, counter);
    generate_parent(bfs, par, counter, parent, count);
    scatter_children(bfs, parent, count, nchild, counter);
    allocate(alloc, nchild, counter);
    int j = alloc[counter - 1];
    int value[j] = {0};
    for (int i = 0; i < counter; i++)
    {
        value[alloc[i]] = nchild[i];
    }
    for (int i = 1; i < counter; i++)
    {
        value[parent[i] + bfs[i]] = alloc[i];
    }
    for (int i = 0; i < j+1; i++)
    {
        cout << value[i] << endl;
    }
    return 0;
}