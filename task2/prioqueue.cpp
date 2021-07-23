#include "fiboheap.h"
#include <queue>
using namespace std;

#include <ctime>
#include <cstdlib>
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
    Priority_queue<int> p1; //待检测
    priority_queue<int,vector<int>,greater<int>> p0;
    int tmp;
    //少量测试
    for (int i = 1; i <= 10; i++)
    {
        tmp = (unsigned int)rand();
        p1.push(tmp);
        p0.push(tmp);
    }
    asse("empty", p0.empty() == p0.empty());
    asse("size", p1.size() == p0.size());
    for (int i=1;i<=10;i++)
    {
        cout<<p0.top()<<endl;
        p1.pop();
        p0.pop();
    }
    asse("empty", p1.empty() == p0.empty());
    asse("size", p1.size() == p0.size());

    //大数据量
    for(int i=1;i<=100000;i++)
    {
        tmp=rand();
        p1.push(tmp);
        p0.push(tmp);
    }
    for(int i=1;i<=100000;i++)
    {
        assert(p1.top()==p0.top());
        p1.pop();
        p0.pop();
    }
    cout<<"Final pass\n";
}