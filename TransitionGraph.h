#pragma once
#include <iostream>
#include <string>
#include "NFA.h"

class TransitionGraph
{
public:
	TransitionGraph() = default;
	TransitionGraph(const NFA& nfa);

	friend std::ostream& operator<<(std::ostream&, const TransitionGraph&);

public:
	void NewInitialState();
	void NewFinalState();

	void Reconstruction();
	void StateElimination();
	std::string RegularExpression();

private:
	std::unordered_set<State> StatesThatGoesInto(const State&);
	std::unordered_set<State> StatesThatCanBeReachedFrom(const State&);
	bool HasSelfLoop(const State&);

private:
	NFA m_nfa;
};
