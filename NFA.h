#pragma once
#include <iostream>
#include <unordered_map>
#include <unordered_set>

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

class NFA
{
public:
	NFA() = default;

	friend std::istream& operator>>(std::istream&, NFA&);
	friend std::ostream& operator<<(std::ostream&, const NFA&);

	bool Verify();
	void Clear();

public:
	void InsertState(const State&);
	void RemoveState(const State&);

	void InsertTransition(const State&, const Label&, const State&);
	void SetInitialState(const State&);
	void InsertFinalState(const State&);

public:
	std::unordered_set<State> States;
	TransitionTable Transitions;
	State InitialState;
	std::unordered_set<State> FinalStates;
};
