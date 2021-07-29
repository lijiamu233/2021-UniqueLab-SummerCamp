#define CATCH_CONFIG_MAIN

#include "set.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <catch2/catch.hpp>
using namespace lfp;

int main() {
	srand((unsigned)time(NULL));
	int length=(rand()%(1000))+1;	
	int ia[length];
	for(int i=0;i<length;i++){
		ia[i]=(rand()%(1000))+1;
	}
	set<int> iset(ia,ia+length);

	iset.insert(3);


	iset.insert(5);
	set<int>::iterator it = iset.begin();
	iset.erase(iset.find(3));	 
	it = iset.begin();
	auto end = iset.end();
	iset.clear();
	

	return 0;

}
