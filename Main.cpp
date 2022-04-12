#include <iostream>
#include <fstream>
#include "NFA.h"
#include "Conversion.h"

int main()
{
	std::ifstream fin("nfa_elements.txt");

	NFA nfa;
	fin >> nfa;

	if (nfa.Verify())
	{
		Conversion::Start(nfa);
		std::cout << "The regular expression: " << Conversion::RegularExpression() << std::endl;
	}
	else
	{
		std::cout << "The NFA is not valid. \n";
	}

	return 0;
}