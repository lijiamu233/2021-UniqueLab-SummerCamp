		
 

#include <assert.h>
#include <random>
#include <iostream>
using namespace std;
 
#define TESTNUM 100000
#define POPNUM 100000
 
template<class T>
class fibonacciNode
{
public:
	T keyValue;
	int degree;
	fibonacciNode *leftBrother;
	fibonacciNode *rightBrother;
	fibonacciNode *child;
	fibonacciNode()
	{
		leftBrother = this;
		rightBrother = this;
		child = NULL;
		degree = 0;
	}
	bool operator<(const fibonacciNode node)
	{
		return keyValue < node.keyValue;
	}
	bool operator<=(const fibonacciNode node)
	{
		return keyValue <= node.keyValue;
	}
	bool operator==(const fibonacciNode node)
	{
		return keyValue == node.keyValue;
	}
};
 
template<class T>
class CFibonacci
{
private:
	fibonacciNode<T> *minH;						
	int keyNum;								
	int maxDegree;								
 
private:
	void clearRecursive(fibonacciNode<T> *pCurrentNode);			
	inline void improveChild();					
	inline void deleteMinNode();				
	
	inline void conslidate();				
	inline fibonacciNode<T> *removeNode();		
												
	inline void linkNode(fibonacciNode<T> *x, fibonacciNode<T> *y);			
	fibonacciNode<T> **cons;					
	inline void combineCons();					
	inline void linkCons();						
 
public:
	CFibonacci();
	~CFibonacci();
	
public:
	void push(const T node);
	void pop();
	T top();
	void clear();	
 
};
 
template<class T>
CFibonacci<T>::CFibonacci()
	:minH(NULL), keyNum(0)
{
	maxDegree = int(log(TESTNUM * 1.0) / log(2.0)) + 1;
	cons = new fibonacciNode<T> *[maxDegree];
	for (int i=0; i<maxDegree; ++i)
	{
		*(cons + i) = NULL;
	}
}
 
template<class T>
CFibonacci<T>::~CFibonacci()
{
	if (minH)
	{
		clear();
	}
	delete []cons;
}
 
template<class T>
void CFibonacci<T>::push(const T node)
{
	fibonacciNode<T> *newNode = new fibonacciNode<T>;
	newNode->keyValue = node;
	keyNum ++;
	if (!minH)
	{
		minH = newNode;
		return;
	}
	newNode->leftBrother = minH;
	newNode->rightBrother = minH->rightBrother;
	minH->rightBrother->leftBrother = newNode;
	minH->rightBrother = newNode;
	if (*newNode <= *minH)
	{
		minH = newNode;
	}
}
 
template<class T>
void CFibonacci<T>::improveChild()
{
	fibonacciNode<T> *pCurrentNode;
 
	pCurrentNode = minH->child;
 
	fibonacciNode<T> *pTempNode;
	pTempNode = minH->rightBrother;
 
	pCurrentNode->rightBrother->leftBrother = minH;
	minH->rightBrother = pCurrentNode->rightBrother;
 
	pTempNode->leftBrother = pCurrentNode;
	pCurrentNode->rightBrother = pTempNode;
}
 
template<class T>
void CFibonacci<T>::deleteMinNode()
{
	fibonacciNode<T> *pCurrentNode;
	pCurrentNode = minH->rightBrother;
 
	minH->leftBrother->rightBrother = pCurrentNode;
	pCurrentNode->leftBrother = minH->leftBrother;
 
	delete minH;
	minH = pCurrentNode;
}
 
template<class T>
void CFibonacci<T>::pop()
{
	assert(keyNum>0);
	keyNum--;
	
	if (minH->child)
	{
		improveChild();
	}
 
	if (minH == minH->rightBrother)
	{
		delete minH;
		minH = NULL;
	}
	else
	{
		deleteMinNode();
		conslidate();
	}
}
 
template<class T>
T CFibonacci<T>::top()
{
	assert(keyNum>0);
	return minH->keyValue;
}
 
template<class T>
void CFibonacci<T>::clear()
{
	if (!minH)
	{
		return;
	}
	keyNum = 0;
	clearRecursive(minH);
	minH = NULL;
}
 
template<class T>
void CFibonacci<T>::clearRecursive(fibonacciNode<T> *pCurrentNode)
{
	fibonacciNode<T> *pCurrentRight;
	//处理孩子
	if (pCurrentNode->child)
	{
		clearRecursive(pCurrentNode->child);
		pCurrentNode->child = NULL;
	}
	//处理兄弟
	if (pCurrentNode != pCurrentNode->rightBrother)
	{
		pCurrentRight = pCurrentNode->rightBrother;
		pCurrentNode->leftBrother->rightBrother = pCurrentRight;
		pCurrentRight->leftBrother = pCurrentNode->leftBrother;
		delete pCurrentNode;
		clearRecursive(pCurrentRight);
	}
	else
	{
		delete pCurrentNode;
	}
}
 
template<class T>
fibonacciNode<T> *CFibonacci<T>::removeNode()
{
	fibonacciNode<T> *x = minH;
	//移除minH
	if (minH->rightBrother == minH)
	{
		minH = NULL;
	}
	else
	{
		minH->leftBrother->rightBrother = minH->rightBrother;
		minH->rightBrother->leftBrother = minH->leftBrother;
		minH = minH->rightBrother;
	}
	x->leftBrother = x;
	x->rightBrother = x;
 
	return x;
}
 
template<class T>
void CFibonacci<T>::linkNode(fibonacciNode<T> *x, fibonacciNode<T> *y)
{
	if (!x->child)
	{
		x->child = y;
	}
	else
	{
		x->child->rightBrother->leftBrother = y;
		y->rightBrother = x->child->rightBrother;
 
		x->child->rightBrother = y;
		y->leftBrother = x->child;
	}
	x->degree++;
}
 
template<class T>
void CFibonacci<T>::combineCons()
{
	fibonacciNode<T> *x, *y;
	int degree;
	while (NULL != minH)
	{
		x = removeNode();
		degree = x->degree;
		while (NULL != *(cons + degree))
		{
			y = *(cons + degree);
			if (*y < *x)
			{
				swap(x, y);
			}
			linkNode(x, y);					
			*(cons + degree) = NULL;
			degree++;
		}
		*(cons + degree) = x;
	}
}
 
template<class T>
void CFibonacci<T>::linkCons()
{
	for (int i=0; i<maxDegree; ++i)
	{
		if (*(cons + i))
		{
			if (!minH)
			{
				minH = *(cons + i);
			}
			else
			{
				minH->rightBrother->leftBrother = *(cons + i);
				(*(cons + i))->rightBrother = minH->rightBrother;
 
				minH->rightBrother = *(cons + i);
				(*(cons + i))->leftBrother = minH;
				minH =*(*(cons + i)) < *minH ? (*(cons + i)):minH;
			}
			*(cons + i) = NULL;
		}
	}
}
 
template<class T>
void CFibonacci<T>::conslidate()
{

	combineCons();
 

	linkCons();
}