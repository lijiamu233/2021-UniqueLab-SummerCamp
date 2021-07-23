#include <iostream>

#include "rbtree.h"
#include <set>
using namespace std; //实验和对照组

#include <cstdlib>
#include <ctime>
#include <cassert>
#include <cstdbool>

void asse(const char *msg, bool expr)
{
    assert(expr);
    printf("%s pass\n\n", msg);
}

int main()
{
    srand(time(NULL));
    int tmp;
    set<int> st0;
    Set<int> st1; //待测试
    set<int>::iterator st_it;
    Set<int>::Iterator ts_it;
    for (int i = 1; i < 100000; i++)
    {
        tmp = (unsigned int)rand();
        st0.insert(tmp);
        st1.insert(tmp);
    }
    asse("func empty", st0.empty() == st1.empty());
    asse("func size", st0.size() == st1.size()); //size empty

    //随机查找若干个数,upper_bound,lower-bound
    for (int i = 1; i <= 10; i++)
    {
        tmp = rand();
        assert(*st0.upper_bound(tmp)==*st1.upper_bound(tmp));
    }
    cout<<"upper pass"<<endl<<endl;
    for (int i = 1; i <= 10; i++)
    {
        tmp = rand();
        assert(*st0.lower_bound(tmp)==*st1.lower_bound(tmp));
    }
    cout<<"lower pass"<<endl<<endl;

    //顺序与Iterator检测
    for(st_it=st0.begin(),ts_it=st1.begin();st_it!=st0.end();st_it++,ts_it++)
        assert(*st_it==*ts_it);
    cout<<"Order and Iterator"<<endl<<endl;
    
    //count
    for (int i = 1; i <= 10; i++)
    {
        tmp = rand();
        assert(st0.count(tmp)==st1.count(tmp));
    }
    cout<<"count pass"<<endl<<endl;

    //clear
    st0.clear();
    st1.clear();
    asse("clear",st0.empty() == st1.empty()&& st0.size() == st1.size());
}