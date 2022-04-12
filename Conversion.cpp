#include "Conversion.h"

TransitionGraph Conversion::m_transitionGraph;

void Conversion::Start(const NFA& nfa)
{
	m_transitionGraph = nfa;
	std::cout << m_transitionGraph << std::endl;

	m_transitionGraph.NewInitialState();
	std::cout << "Transition graph after adding a new start state: " << std::endl;
	std::cout << m_transitionGraph << std::endl;

	m_transitionGraph.NewFinalState();
	std::cout << "Transition graph after adding a new final state: " << std::endl;
	std::cout << m_transitionGraph << std::endl;

	m_transitionGraph.Reconstruction();
	std::cout << "Transition graph after reconstruction: " << std::endl;
	std::cout << m_transitionGraph << std::endl;

	m_transitionGraph.StateElimination();
}

std::string Conversion::RegularExpression()
{
	return m_transitionGraph.RegularExpression();
}
