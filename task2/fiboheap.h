#ifndef FIBOHEAP_H
#define FIBOHEAP_H
#include <cstddef>
#include <cmath>
#include <limits>
#include <iostream>
template <class T, class Comp = std::less<T>>
class Priority_queue
{
public:
    class FibNode
    {
    public:
        FibNode(T k, void *pl)
            : key(std::move(k)), mark(false), p(nullptr), left(nullptr), right(nullptr), child(nullptr), degree(-1), payload(pl)
        {}
        T key;
        bool mark;
        FibNode *p;
        FibNode *left;
        FibNode *right;
        FibNode *child;
        int degree;
        void *payload;
    };
    Priority_queue() : Priority_queue(std::less<T>())
    {
    }
    Priority_queue(Comp comp)
        : n(0), min(nullptr), comp(comp)
    {
    }
    void insert(FibNode *x)
    {
        x->degree = 0;
        x->p = nullptr;
        x->child = nullptr;
        x->mark = false;
        if (min == nullptr)
        {
            min = x->left = x->right = x;
        }
        else
        {
            min->left->right = x;
            x->left = min->left;
            min->left = x;
            x->right = min;
            if (comp(x->key, min->key))
            {
                min = x;
            }
        }
        ++n;
    }
    FibNode *minimum()
    {
        return min;
    }
    FibNode *pop_min()
    {
        FibNode *z, *x, *next;
        FibNode **childList;
        z = min;
        if (z != nullptr)
        {
            x = z->child;
            if (x != nullptr)
            {
                childList = new FibNode *[z->degree];
                next = x;
                for (int i = 0; i < (int)z->degree; i++)
                {
                    childList[i] = next;
                    next = next->right;
                }
                for (int i = 0; i < (int)z->degree; i++)
                {
                    x = childList[i];
                    min->left->right = x;
                    x->left = min->left;
                    min->left = x;
                    x->right = min;
                    x->p = nullptr;
                }
                delete[] childList;
            }
            z->left->right = z->right;
            z->right->left = z->left;
            if (z == z->right)
            {
                min = nullptr;
            }
            else
            {
                min = z->right;
                reconstruct();
            }
            n--;
        }
        return z;
    }
    void reconstruct()
    {
        FibNode *w, *next, *x, *y, *temp;
        FibNode **A, **rootList;
        int d, rootSize;
        //要处理的最大的度
        int max_degree = static_cast<int>(floor(log(static_cast<double>(n)) / log(static_cast<double>(1 + sqrt(static_cast<double>(5))) / 2)));
        A = new FibNode *[max_degree + 2];
        std::fill_n(A, max_degree + 2, nullptr);
        w = min;
        rootSize = 0;
        next = w;
        do
        {
            rootSize++;
            next = next->right;
        } while (next != w);
        rootList = new FibNode *[rootSize];
        for (int i = 0; i < rootSize; i++)
        {
            rootList[i] = next;
            next = next->right;
        }
        for (int i = 0; i < rootSize; i++)
        {
            w = rootList[i];
            x = w;
            d = x->degree;
            while (A[d] != nullptr)
            {
                y = A[d];
                if (comp(y->key, x->key))
                {
                    temp = x;
                    x = y;
                    y = temp;
                }
                link_to_others(y, x);
                A[d] = nullptr;
                d++;
            }
            A[d] = x;
        }
        delete[] rootList;
        min = nullptr;
        for (int i = 0; i < max_degree + 2; i++)
        {
            if (A[i] != nullptr)
            {
                if (min == nullptr)
                {
                    min = A[i]->left = A[i]->right = A[i];
                }
                else
                {
                    min->left->right = A[i];
                    A[i]->left = min->left;
                    min->left = A[i];
                    A[i]->right = min;
                    if (comp(A[i]->key, min->key))
                    {
                        min = A[i];
                    }
                }
            }
        }
        delete[] A;
    }
    void link_to_others(FibNode *y, FibNode *x)
    {
        y->left->right = y->right;
        y->right->left = y->left;
        if (x->child != nullptr)
        {
            x->child->left->right = y;
            y->left = x->child->left;
            x->child->left = y;
            y->right = x->child;
        }
        else
        {
            x->child = y;
            y->right = y;
            y->left = y;
        }
        y->p = x;
        x->degree++;
        y->mark = false;
    }
    bool empty() const
    {
        return n == 0;
    }
    T top()
    {
        return minimum()->key;
    }
    void pop()
    {
        if (empty())
            return;
        FibNode *x = pop_min();
        if (x)
            delete x;
    }
    FibNode *push(T k, void *pl)
    {
        FibNode *x = new FibNode(std::move(k), pl);
        insert(x);
        return x;
    }
    FibNode *push(T k)
    {
        return push(std::move(k), nullptr);
    }
    unsigned int size()
    {
        return (unsigned int)n;
    }
    int n;
    FibNode *min;
    Comp comp;
};
#endif