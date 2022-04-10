# NFA to RE

This is my C++ implementation of the problem challenge proposed at the course of __Automata, Calculability and Complexity__.

## Description

This algorithm uses a NFA given as input in a file. If the input is correct, then the regular expression will be returned, using the _State Elimination Method_.
There is an example in the __NFA to RE example__ folder. I wrote my approach in __nfa-steps__ file so that I can visualize how to solve the problem.

The steps of the method involved are:
1. Add a new initial state S and an ε-transition δ(S, ε) = q₀.
2. Add a new final state F and ε-transitions from every final states that were before in the NFA to the F state.
3. Construct transition graph.
4. State elimination.

I used a __Helper__ class where I implemented __Union__ of regexes, __Concatenation__ of two regexes, and __Kleene Star__ operations.
