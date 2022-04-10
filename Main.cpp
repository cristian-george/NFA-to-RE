#include <iostream>
#include <fstream>
#include "GeneralizedTransitionGraph.h"

int main()
{
	std::ifstream fin("nfa_elements.txt");
	TransitionGraph transitionGraph;

	fin >> transitionGraph;
	if (!transitionGraph.Verify())
	{
		std::cout << "The NFA is not valid. \n";
		return 0;
	}

	std::cout << transitionGraph << std::endl;

	transitionGraph.AddNewInitialState();
	std::cout << "Transition graph after adding a new start state: " << std::endl;
	std::cout << transitionGraph << std::endl;

	transitionGraph.AddNewFinalState();
	std::cout << "Transition graph after adding a new final state: " << std::endl;
	std::cout << transitionGraph << std::endl;

	transitionGraph.ReconstructTransitionGraph();
	std::cout << "Transition graph after reconstruction: " << std::endl;
	std::cout << transitionGraph << std::endl;

	transitionGraph.StateElimination();
	std::cout << "The regular expression: " << transitionGraph.GetRegularExpression() << std::endl;
	return 0;
}