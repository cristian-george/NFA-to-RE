#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>

class PairHashing
{
public:
	template<class T1, class T2>
	std::size_t operator()(const std::pair<T1, T2>& pair) const
	{
		return std::hash<T1>()(pair.first) ^ std::hash<T2>()(pair.second);
	}
};

using State = std::string;
using Label = std::string;
using TransitionTable = std::unordered_map<std::pair<State, State>, std::vector<Label>, PairHashing>;

class TransitionGraph
{
public:
	TransitionGraph() = default;

	friend std::istream& operator>>(std::istream&, TransitionGraph&);
	friend std::ostream& operator<<(std::ostream&, const TransitionGraph&);

	void Clear();

public:
	std::unordered_set<State> GetStates() const;
	TransitionTable GetTransitionTable() const;
	State GetInitialState() const;
	std::unordered_set<State> GetFinalStates() const;

public:
	void InsertState(const State&);
	void RemoveState(const State&);

	void InsertTransition(const State&, const Label&, const State&);
	void SetInitialState(const State&);
	void InsertFinalState(const State&);

public:
	void AddNewInitialState();
	void AddNewFinalState();
	void ReconstructTransitionGraph();
	void StateElimination();
	std::string GetRegularExpression();

private:
	std::unordered_set<State> StatesThatGoesInto(const State&);
	std::unordered_set<State> StatesThatCanBeReachedFrom(const State&);
	bool HasSelfLoop(const State&);

private:
	const std::string EMPTY_STRING = "$";

private:
	std::unordered_set<State> m_states;
	TransitionTable m_transitionTable;
	State m_initialState;
	std::unordered_set<State> m_finalStates;
};