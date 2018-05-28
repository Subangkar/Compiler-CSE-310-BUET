#include <iostream>
#include "DataStructure.h"
#include "DataStructure.h"
int main() {
	std::cout << "Hello, World!" << std::endl;

	ScopeTable st(nullptr,0,10);

	st.setPrintEmptyList(true);

	st.Print();

	return 0;
}