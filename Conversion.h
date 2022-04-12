#pragma once
#include "NFA.h"
#include "TransitionGraph.h"

class Conversion
{
public:
	Conversion() = default;

	static void Start(const NFA& nfa);
	static std::string RegularExpression();

private:
	static TransitionGraph m_transitionGraph;
};
