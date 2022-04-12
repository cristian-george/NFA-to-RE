#pragma once
#include <iostream>
#include <unordered_set>

const std::string EMPTY_STRING = "$";

class Helper
{
public:
	template<class T>
	static void PrintSet(const std::unordered_set<T>&);

	template<class T>
	static T Union(const std::vector<T>&);

	template<class T>
	static T Concatenation(const T&, const T&);

	template<class T>
	static T KleeneStar(const T&);
};

template<class T>
inline void Helper::PrintSet(const std::unordered_set<T>& set)
{
	for (const auto& element : set)
		std::cout << element << ", ";

	std::cout << "\b\b \n";
}

template<class T>
inline T Helper::Union(const std::vector<T>& labels)
{
	T unionLabel;

	unionLabel.push_back('(');
	for (const auto& label : labels)
	{
		unionLabel += label + "+";
	}

	unionLabel.pop_back();
	unionLabel.push_back(')');

	return unionLabel;
}

template<class T>
inline T Helper::Concatenation(const T& label1, const T& label2)
{
	if (label1 == EMPTY_STRING && label2 == EMPTY_STRING)
	{
		return EMPTY_STRING;
	}
	if (label1 == EMPTY_STRING)
	{
		return label2;
	}
	else if (label2 == EMPTY_STRING)
	{
		return label1;
	}

	return T(label1 + label2);
}

template<class T>
inline T Helper::KleeneStar(const T& label)
{
	if (label.length() == 1)
		return T(label + "*");

	return T("(" + label + ")*");
}