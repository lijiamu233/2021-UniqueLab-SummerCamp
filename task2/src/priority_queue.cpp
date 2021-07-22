#include "algobase.h"
#include "vector.h"
#include "priority_queue.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>
using namespace lfp;

int main() {
	srand((unsigned)time(NULL));
	int length=(rand()%(20))+1;	
	priority_queue<int, vector<int>, greater<int>> ipq2;
	std::cout << "empty()=" << ipq2.empty() <<std::endl;
	for(int i=0;i<length;i++){
		ipq2.push((rand()%(1000))+1);
		std::cout << "ipq2.size() = " << ipq2.size() << "  ipq2.top() = " << ipq2.top()<< "empty()=" << ipq2.empty() << std::endl;
	}
	std::cout << "empty()=" << ipq2.empty() <<std::endl;
	std::cout << "size()=" << ipq2.size() <<std::endl;
	while (ipq2.size() > 0) {
		std::cout << "ipq2.size() = " << ipq2.size() << "  ipq2.top() = " << ipq2.top()<< "empty()=" << ipq2.empty() << std::endl;
		ipq2.pop();
	}
	std::cout << "empty()=" << ipq2.empty() <<std::endl;
	std::cout << std::endl << std::endl;

	return 0;
}