#include "TransitionGraph.h"
#include "Helper.h"

TransitionGraph::TransitionGraph(const NFA& nfa) : m_nfa(nfa) {}

std::ostream& operator<<(std::ostream& out, const TransitionGraph& transitionGraph)
{
	out << transitionGraph.m_nfa;
	return out;
}

void TransitionGraph::NewInitialState()
{
	State newInitialState("S");
	State oldInitialState(m_nfa.InitialState);

	m_nfa.SetInitialState(newInitialState);
	m_nfa.InsertState(newInitialState);
	m_nfa.InsertTransition(newInitialState, EMPTY_STRING, oldInitialState);
}

void TransitionGraph::NewFinalState()
{
	State newFinalState("F");

	for (const auto& oldFinalState : m_nfa.FinalStates)
	{
		m_nfa.InsertTransition(oldFinalState, EMPTY_STRING, newFinalState);
	}

	m_nfa.FinalStates.clear();
	m_nfa.InsertFinalState(newFinalState);
	m_nfa.InsertState(newFinalState);
}

void TransitionGraph::Reconstruction()
{
	for (const auto& [states, labels] : m_nfa.Transitions)
	{
		if (labels.size() > 1)
		{
			Label newLabel = Helper::Union<Label>(labels);

			m_nfa.Transitions[states].clear();
			m_nfa.Transitions[states].push_back(newLabel);
		}
	}
}

void TransitionGraph::StateElimination()
{
	size_t counter = 0;
	while (m_nfa.States.size() > 2)
	{
		State stateToRemove = "q" + std::to_string(counter);
		bool hasSelfLoop = HasSelfLoop(stateToRemove);

		std::unordered_set<State> inSet = StatesThatGoesInto(stateToRemove);
		std::unordered_set<State> outSet = StatesThatCanBeReachedFrom(stateToRemove);

		std::cout << "---------- Removing state " << stateToRemove << " ---------- \n";
		if (!inSet.empty() && !outSet.empty())
		{
			std::cout << "States that goes into " << stateToRemove << ": ";
			Helper::PrintSet<State>(inSet);

			std::cout << "States that can be reached from " << stateToRemove << ": ";
			Helper::PrintSet<State>(outSet);

			for (const auto& inState : inSet)
			{
				for (const auto& outState : outSet)
				{
					Label firstLabel = *m_nfa.Transitions[std::make_pair(inState, stateToRemove)].begin();
					Label lastLabel = *m_nfa.Transitions[std::make_pair(stateToRemove, outState)].begin();

					Label newLabel = Helper::Concatenation<Label>(EMPTY_STRING, firstLabel);
					if (hasSelfLoop)
					{
						Label intermediateLabel = Helper::KleeneStar<Label>(
							*m_nfa.Transitions[std::make_pair(stateToRemove, stateToRemove)].begin());
						newLabel = Helper::Concatenation<Label>(newLabel, intermediateLabel);
					}
					newLabel = Helper::Concatenation<Label>(newLabel, lastLabel);

					m_nfa.InsertTransition(inState, newLabel, outState);
				}
			}
		}
		else
		{
			std::cout << "The state " << stateToRemove << " it's a trap state \n";
		}

		m_nfa.RemoveState(stateToRemove);
		Reconstruction();

		std::cout << *this;
		++counter;
	}
}

std::string TransitionGraph::RegularExpression()
{
	return m_nfa.Transitions[std::make_pair("S", "F")][0];
}

std::unordered_set<State> TransitionGraph::StatesThatGoesInto(const State& refState)
{
	std::unordered_set<State> statesThatGoesInto;
	for (const auto& [states, labels] : m_nfa.Transitions)
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
	for (const auto& [states, labels] : m_nfa.Transitions)
	{
		if (states.first == refState && states.second != refState)
		{
			statesThatCanBeReachedFrom.insert(states.second);
		}
	}

	return statesThatCanBeReachedFrom;
}

bool TransitionGraph::HasSelfLoop(const State& refState)
{
	return m_nfa.Transitions.find(std::make_pair(refState, refState)) != m_nfa.Transitions.end();
}