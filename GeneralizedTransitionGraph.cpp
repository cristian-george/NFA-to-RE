#include "GeneralizedTransitionGraph.h"
#include "Helper.h"

std::istream& operator>>(std::istream& in, TransitionGraph& nfa)
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

std::ostream& operator<<(std::ostream& out, const TransitionGraph& nfa)
{
	out << "States: ";
	for (const auto& state : nfa.GetStates())
	{
		out << state << ", ";
	}
	out << "\b\b \n";

	out << "Initial state: " << nfa.GetInitialState() << std::endl;

	out << "Final states: ";
	for (const auto& finalState : nfa.GetFinalStates())
	{
		out << finalState << ", ";
	}
	out << "\b\b \n";

	std::cout << "Transitions: " << std::endl;
	for (const auto& [states, labels] : nfa.GetTransitionTable())
	{
		for (const auto& label : labels)
		{
			out << "	(" << states.first << ", " << label << ") -> " << states.second << "\n";
		}
	}
	out << std::endl;

	return out;
}

void TransitionGraph::Clear()
{
	m_states.clear();
	m_initialState.clear();
	m_finalStates.clear();
	m_transitionTable.clear();
}

std::unordered_set<State> TransitionGraph::GetStates() const
{
	return m_states;
}

TransitionTable TransitionGraph::GetTransitionTable() const
{
	return m_transitionTable;
}

State TransitionGraph::GetInitialState() const
{
	return m_initialState;
}

std::unordered_set<State> TransitionGraph::GetFinalStates() const
{
	return m_finalStates;
}

void TransitionGraph::InsertState(const State& state)
{
	m_states.insert(state);
}

void TransitionGraph::RemoveState(const State& refState)
{
	m_states.erase(refState);

	TransitionTable transitionTable = m_transitionTable;
	for (const auto& [states, labels] : transitionTable)
	{
		if (states.first == refState or states.second == refState)
		{
			m_transitionTable.erase(states);
		}
	}
}

void TransitionGraph::InsertTransition(const State& state1, const Label& label, const State& state2)
{
	m_transitionTable[std::make_pair(state1, state2)].push_back(label);
}

void TransitionGraph::SetInitialState(const State& state)
{
	m_initialState = state;
}

void TransitionGraph::InsertFinalState(const State& state)
{
	m_finalStates.insert(state);
}

void TransitionGraph::AddNewInitialState()
{
	State newInitialState("S");
	State oldInitialState(m_initialState);

	SetInitialState(newInitialState);
	InsertState(newInitialState);
	InsertTransition(newInitialState, EMPTY_STRING, oldInitialState);
}

void TransitionGraph::AddNewFinalState()
{
	State newFinalState("F");

	for (const auto& oldFinalState : m_finalStates)
	{
		InsertTransition(oldFinalState, EMPTY_STRING, newFinalState);
	}

	m_finalStates.clear();
	InsertFinalState(newFinalState);
	InsertState(newFinalState);
}

void TransitionGraph::ReconstructTransitionGraph()
{
	for (const auto& [states, labels] : m_transitionTable)
	{
		if (labels.size() > 1)
		{
			Label newLabel = Helper::Union<Label>(labels);

			m_transitionTable[states].clear();
			m_transitionTable[states].push_back(newLabel);
		}
	}
}

void TransitionGraph::StateElimination()
{
	size_t counter = 0;
	while (m_states.size() > 2)
	{
		State stateToRemove = "q" + std::to_string(counter);
		bool hasLoop = HasLoop(stateToRemove);

		std::unordered_set<State> inSet =
			StatesThatGoesInto(stateToRemove);

		std::unordered_set<State> outSet =
			StatesThatCanBeReachedFrom(stateToRemove);

		if (!inSet.empty() && !outSet.empty())
		{
			std::cout << "States that goes into " << stateToRemove << ": ";
			Helper::PrintSet<State>(inSet);

			std::cout << "States that can be reached from " << stateToRemove << ": ";
			Helper::PrintSet<State>(outSet);

			for (const auto& firstState : inSet)
			{
				for (const auto& secondState : outSet)
				{
					Label firstLabel = m_transitionTable[std::make_pair(firstState, stateToRemove)][0];
					Label lastLabel = m_transitionTable[std::make_pair(stateToRemove, secondState)][0];

					Label newLabel = Helper::Concatenation<Label>(EMPTY_STRING, firstLabel);
					if (hasLoop)
					{
						Label intermediateLabel = Helper::KleeneStar<Label>(m_transitionTable[std::make_pair(stateToRemove, stateToRemove)][0]);
						newLabel = Helper::Concatenation<Label>(newLabel, intermediateLabel);
					}
					newLabel = Helper::Concatenation<Label>(newLabel, lastLabel);

					InsertTransition(firstState, newLabel, secondState);
				}
			}
		}
		else
		{
			std::cout << stateToRemove << " it's a dead state \n";
		}

		RemoveState(stateToRemove);
		ReconstructTransitionGraph();
		++counter;
	}
}

std::string TransitionGraph::GetRegularExpression()
{
	return m_transitionTable[std::make_pair("S", "F")][0];
}

std::unordered_set<State> TransitionGraph::StatesThatGoesInto(const State& refState)
{
	std::unordered_set<State> statesThatGoesInto;
	for (const auto& [states, labels] : m_transitionTable)
	{
		if (states.second == refState && states.first != refState)
		{
			statesThatGoesInto.insert(states.first);
		}
	}

	return statesThatGoesInto;
}

std::unordered_set<State> TransitionGraph::StatesThatCanBeReachedFrom(const State& refState)
{
	std::unordered_set<State> statesThatCanBeReachedFrom;
	for (const auto& [states, labels] : m_transitionTable)
	{
		if (states.first == refState && states.second != refState)
		{
			statesThatCanBeReachedFrom.insert(states.second);
		}
	}

	return statesThatCanBeReachedFrom;
}

bool TransitionGraph::HasLoop(const State& refState)
{
	return m_transitionTable.find(std::make_pair(refState, refState)) != m_transitionTable.end();
}