#include "set.h"
#include <iostream>

using namespace lfp;

int main() {
	int ia[5] = { 0, 1, 2 , 3};
	set<int> iset(ia,ia+4);

	std::cout << "size() = " << iset.size() << "  count(3) = " << iset.count(3) << std::endl;
	iset.insert(3);
	std::cout << "size() = " << iset.size() << "  count(3) = " << iset.count(3) << std::endl << std::endl;
	std::cout << "empty() = " << iset.empty() << std::endl;
	iset.erase(iset.find(3));	 
	std::cout << "size() = " << iset.size() << std::endl;
	it = iset.begin();
	auto end = iset.end();
	while (it != end) {
		std::cout << *it << " ";
		++it;
	}
	std::cout << std::endl << std::endl;
	iset.clear();
	
	std::cout << "empty() = " << iset.empty() << std::endl;
	for (int i : iset)
		std::cout << i << " ";
	std::cout << std::endl << std::endl;

	return 0;

}
