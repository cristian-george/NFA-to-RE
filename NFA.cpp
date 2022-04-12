#include "NFA.h"

std::istream& operator>>(std::istream& in, NFA& nfa)
{
	nfa.Clear();

	size_t numberOfStates;
	in >> numberOfStates;
	for (size_t index = 0; index < numberOfStates; ++index)
	{
		State state;
		in >> state;
		nfa.InsertState(state);
	}

	size_t numberOfTransitions;
	in >> numberOfTransitions;
	for (size_t index = 0; index < numberOfTransitions; ++index)
	{
		State state1, state2;
		Label symbol;
		in >> state1 >> symbol >> state2;

		nfa.InsertTransition(state1, symbol, state2);
	}

	State initialState;
	in >> initialState;
	nfa.SetInitialState(initialState);

	size_t numberOfFinalStates;
	in >> numberOfFinalStates;
	for (size_t index = 0; index < numberOfFinalStates; ++index)
	{
		State state;
		in >> state;
		nfa.InsertFinalState(state);
	}

	return in;
}

std::ostream& operator<<(std::ostream& out, const NFA& nfa)
{
	out << "States: ";
	for (const auto& state : nfa.States)
	{
		out << state << ", ";
	}
	out << "\b\b \n";

	out << "Initial state: " << nfa.InitialState << std::endl;

	out << "Final states: ";
	for (const auto& finalState : nfa.FinalStates)
	{
		out << finalState << ", ";
	}
	out << "\b\b \n";

	std::cout << "Transitions: " << std::endl;
	for (const auto& [states, labels] : nfa.Transitions)
	{
		for (const auto& label : labels)
		{
			out << "	(" << states.first << ", " << label << ") -> " << states.second << "\n";
		}
	}
	out << std::endl;

	return out;
}

bool NFA::Verify()
{
	if (States.find(InitialState) == States.end())
		return false;

	for (const auto& finalState : FinalStates)
		if (States.find(finalState) == States.end())
			return false;

	for (const auto& [states, symbols] : Transitions)
	{
		if (States.find(states.first) == States.end() or
			States.find(states.second) == States.end())
			return false;

		for (const auto& symbol : symbols)
			if (symbol.length() > 1)
				return false;
	}

	return true;
}

void NFA::Clear()
{
	States.clear();
	InitialState.clear();
	FinalStates.clear();
	Transitions.clear();
}

void NFA::InsertState(const State& state)
{
	States.insert(state);
}

void NFA::RemoveState(const State& refState)
{
	States.erase(refState);

	TransitionTable transitionTable = Transitions;
	for (const auto& [states, labels] : transitionTable)
	{
		if (states.first == refState or states.second == refState)
		{
			Transitions.erase(states);
		}
	}
}

void NFA::InsertTransition(const State& state1, const Label& label, const State& state2)
{
	Transitions[std::make_pair(state1, state2)].push_back(label);
}

void NFA::SetInitialState(const State& state)
{
	InitialState = state;
}

void NFA::InsertFinalState(const State& state)
{
	FinalStates.insert(state);
}