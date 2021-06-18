/*
 * -----------------------------------------------------------------------------
 * File: nfa.hpp
 * -----------------------------------------------------------------------------
 * Author: Mincho Paskaleff
 * -----------------------------------------------------------------------------
 */

#ifndef NFA_H
#define NFA_H

#include "../src/stack.cpp"
#include "../src/dynamic_array.cpp"

class NFAHelper;

class NFA {
private:
    struct state {
        int name;
        bool isFinal;
        state& operator=(const state&);
        bool operator==(const state&) const;
    };

    struct transition {
        int stateFrom;
        int stateTo;
        char literal;
    };

    /*
    struct statesSet {
        int name;
        DynamicArray<state> stSet;
        void add(state);
        statesSet();
        statesSet(const statesSet&);
        ~statesSet();
        statesSet& operator=(const statesSet&);
    };

    struct transitionBnSets {
        statesSet* from;
        char literal;
        statesSet* to;
        transitionBnSets(statesSet* f = nullptr, char l = ' ', statesSet* t = nullptr);
        transitionBnSets(const transitionBnSets&);
        transitionBnSets& operator=(const transitionBnSets&);
    };*/

    static int automatonCount;
    int id;

    int statesCount;
    int maxStates;

    int transitionsCount;
    int maxTransitions;

    int alphabetSize;

    state* states;
    transition* transitions;
    char* alphabet;

    state* getFinalStates(int*) const;
    void epsilonClosure(int, int*, int*) const;
    void addState(state);
    void addLiteral(char);

public:

    // Constructors
    NFA();
    NFA(const NFA&);
    NFA& operator=(const NFA&);

    // Destructor
    ~NFA();

    // Getters
    int getID() const;
    int getStatesCount() const;
    int getTransitionsCount() const;
    bool isInfinite() const;

    // Setters
    void setStates(int);
    void setFinalState(int);
    void addTransition(int, char, int);
    void determinize();
    void minimize();

    // Member Functions
    bool recognise(const char*) const;
    bool isEmpty() const;
    bool isDeterministic() const;
    void printNFA() const;

    friend std::ostream& operator<<(std::ostream&, const NFA&);
    friend class NFAHelper;
    friend NFA** deserialize(const char*, int&);

};

#endif /* NFA_H */
