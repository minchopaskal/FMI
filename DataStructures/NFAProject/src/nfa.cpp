/*
 * -----------------------------------------------------------------------------
 * File: nfa.cpp
 * -----------------------------------------------------------------------------
 * Author: Mincho Paskaleff
 * -----------------------------------------------------------------------------
 */

#include <iostream>
#include <cstring>
#include "../headers/nfa.hpp"
#include "../headers/utils.hpp"

// state implementation
NFA::state& NFA::state::operator=(const state& other) {
    name = other.name;
    isFinal = other.isFinal;
    return *this;
}

bool NFA::state::operator==(const state& other) const {
  return this->name == other.name && this->isFinal == other.isFinal;
}

/*
// statesSet implementation
void NFA::statesSet::add(state val) {
    for(int i = 0; i < stSet.getLength(); ++i)
        if(stSet.getElemAt(i) == val)
            return;
    this->stSet.add(val);
}

NFA::statesSet::statesSet() : name(-1), stSet() { }

NFA::statesSet::statesSet(const statesSet& other) {
    *this = other;
}

NFA::statesSet::~statesSet() {
    stSet.purgeArray();
}

NFA::statesSet& NFA::statesSet::operator=(const statesSet& rhs) {
    if(this != &rhs){
        name = rhs.name;
        stSet = rhs.stSet;
    }
    return *this;
}

// transitionBnSets implementation
NFA::transitionBnSets::transitionBnSets(statesSet* from , char literal, statesSet* to) {
    this->from = from;
    this->to = to;
    this->literal = literal;
}

NFA::transitionBnSets::transitionBnSets(const transitionBnSets & other) {
    *this = other;
}

NFA::transitionBnSets& NFA::transitionBnSets::operator=(const transitionBnSets & rhs) {
    if(this != &rhs) {
        from = rhs.from;
        to = rhs.to;
        literal = rhs.literal;
    }
    return *this;
}*/

// Init of static member
int NFA::automatonCount = 0;

/*
 * Private functions
 */

NFA::state* NFA::getFinalStates(int* count) const {
    if(statesCount == 0) return nullptr;

    state* finalSt = new state[statesCount];
    int counter = 0;

    for(int i = 0; i < statesCount; ++i)
        if(states[i].isFinal)
            finalSt[counter++] = (this->states)[i];

    if(counter > 0) {
        state* finalStates = new state[counter];
        for(int i = 0; i < counter; ++i)
            finalStates[i] = finalSt[i];
        *count = counter;
        delete[] finalSt;
        return finalStates;
    }

    delete[] finalSt;
    *count = 0;
    return nullptr;
}

void NFA::epsilonClosure(int state, int* epsClosure, int* count) const {
    for(int i = 0; i < transitionsCount; ++i) {
        if(transitions[i].stateFrom == state
           && transitions[i].literal == UTILS::EPS
           && transitions[i].stateTo != state) {
            if(!UTILS::isInSet(epsClosure, *count, transitions[i].stateTo))
                epsClosure[*count] = transitions[i].stateTo;
            ++(*count);
            epsilonClosure(epsClosure[*count - 1], epsClosure, count);
        }
    }
}

void NFA::addState(state st) {
    if(statesCount == this->maxStates) {
        this->maxStates *= 2;
        state* newStates = new state[this->maxStates];
        for(int i = 0; i < this->statesCount; ++i)
            newStates[i] = (this->states)[i];

        delete[] this->states;

        this->states = newStates;
    }

    this->states[statesCount] = st;
    ++statesCount;
}

void NFA::addLiteral(char literal) {
    for(int i = 0; i < alphabetSize; ++i)
        if(literal == alphabet[i])
            return;
    alphabet[alphabetSize] = literal;
    ++alphabetSize;
    return;
}

/*
 * Constructors
 */
NFA::NFA() : id(++automatonCount), statesCount(0), maxStates(UTILS::MAX_SIZE),
             transitionsCount(0), maxTransitions(UTILS::MAX_SIZE), alphabetSize(0),
             states(nullptr), transitions(nullptr), alphabet(nullptr) {
    states = new state[maxStates];
    transitions = new transition[maxTransitions];
    alphabet = new char[UTILS::ALPHABET_SIZE];
}

NFA::NFA(const NFA& other) : id(++automatonCount), states(nullptr),
                             transitions(nullptr), alphabet(nullptr) {
  *this = other;
}

NFA& NFA::operator=(const NFA& other) {
    if(this != &other) {
        // This way we have different behaviour for the id-naming
        if(id != automatonCount)
            id = other.id;
        statesCount = other.statesCount;
        maxStates = other.maxStates;
        transitionsCount = other.transitionsCount;
        maxTransitions = other.maxTransitions;
        alphabetSize = other.alphabetSize;

        states = new state[maxStates];
        transitions = new transition[maxTransitions];
        alphabet = new char[UTILS::ALPHABET_SIZE];
        for(int i = 0; i < statesCount; ++i)
            states[i] = other.states[i];
        for(int i = 0; i < transitionsCount; ++i)
            transitions[i] = other.transitions[i];
        for(int i = 0; i < alphabetSize; ++i)
            alphabet[i] = other.alphabet[i];
    }
    return *this;
}

/*
 * Destructor
 */
NFA::~NFA() {
    delete[] states;
    delete[] transitions;
    delete[] alphabet;
}

/*
 * Getters
 */
int NFA::getID() const { return id; }

int NFA::getStatesCount() const { return statesCount; }

int NFA::getTransitionsCount() const { return transitionsCount; }

bool NFA::isInfinite() const {
    return true;
}

/*
 * Setters
 */

void NFA::setStates(int count) {
    state St;
    for(int i = 0; i < count; ++i) {
        St.name = i;
        St.isFinal = false;
        addState(St);
    }
}

void NFA::setFinalState(int state) {
    if(state <= statesCount)
        states[state].isFinal = true;
}

void NFA::addTransition(int from, char literal, int to) {
    if(!UTILS::isInSet(UTILS::ALPHABET, UTILS::ALPHABET_SIZE, literal)) {
        std::cout << "Literal not supported!" << std::endl;
        return;
    }

    transition tr;
    tr.stateFrom = from;
    tr.stateTo = to;
    tr.literal = literal;

    if(transitionsCount == this->maxTransitions) {
        this->maxTransitions *= 2;
        transition* newTransitions = new transition[this->maxTransitions];
        for(int i = 0; i < this->transitionsCount; ++i)
            newTransitions[i] = (this->transitions)[i];

        delete[] this->transitions;

        this->transitions = newTransitions;
    }

    this->transitions[transitionsCount++] = tr;
    addLiteral(literal);
}

void NFA::determinize() {
    // We'll need two set of states
    // The first is analyzed and the states it goes to are stored in the second one
    // The already analyzed sets are stored in an array

    // No time ;(
    /*
    statesSet current;
    statesSet* goesTo = new statesSet;
    Stack<statesSet> notAnalyzed;
    DynamicArray<statesSet> analyzed;
    DynamicArray<transitionBnSets> newTransitions;
    int* epsClosure = new int[getStatesCount()];
    int epsClosureCount = 0;
    epsClosure[epsClosureCount++] = UTILS::START_STATE;
    epsilonClosure(UTILS::START_STATE, epsClosure, &epsClosureCount);
    for(int i = 0; i < epsClosureCount; ++i)
        current.add(states[epsClosure[i]]);
    delete[] epsClosure;
    epsClosureCount = 0;

    current.name = UTILS::START_STATE;

    int counter = 1;
    goesTo->name = counter++;


    std::cout << current.stSet.getLength() <<std::endl;
    while(!notAnalyzed.empty() || current.name == UTILS::START_STATE) {
        for(int i = 0; i < current.stSet.getLength(); ++i){
            for(int j = 0; j < alphabetSize; ++j) {
                for(int k = 0; k < transitionsCount; ++k) {
                    if(transitions[k].stateFrom == current.stSet.getElemAt(i).name
                       && transitions[k].literal == alphabet[j]
                       && transitions[k].literal != UTILS::EPS){
                        goesTo->add(states[transitions[k].stateTo]);
                        epsClosure = new int[getStatesCount()];
                        epsilonClosure(transitions[k].stateTo, epsClosure, &epsClosureCount);
                        for(int m = 0; m < epsClosureCount; ++m)
                            goesTo->add(states[epsClosure[i]]);
                        delete[] epsClosure;
                        epsClosureCount = 0;
                        newTransitions.add(transitionBnSets
                                            (&current, transitions[k].literal, goesTo));
                    }
                }
                std::cout << "curr goest to" << goesTo->stSet.getElemAt(0).name
                          << goesTo->stSet.getElemAt(1).name << std::endl;
                std::cout << "new goes To len: " << goesTo->stSet.getLength() << std::endl;

                std::cout << goesTo->stSet.getLength() << std::endl;
                if(goesTo->stSet.getLength() > 0) {
                    std::cout << "pushed!" << std::endl;
                    notAnalyzed.push(*goesTo);
                }
                goesTo = new statesSet;
                goesTo->name = counter++;
            }
        }
        analyzed.add(current);
        current = notAnalyzed.pop();
        std::cout << "current name: " << current.name << std::endl;
        std::cout << "curr len: " << current.stSet.getLength() << std::endl;
    }

    for(int i = 0; newTransitions.getLength(); ++i){
        std::cout << newTransitions.getElemAt(i).from->name << " "
                  << newTransitions.getElemAt(i).literal << " ";
        std::cout << newTransitions.getElemAt(i).to->name << std::endl;
    }
    int a;
    std::cin >> a;
    */

}

void NFA::minimize() {
    determinize();
}

/*
 * Member Functions
 */

bool NFA::isDeterministic() const {
    // If Epsilon is in the alphabet we say the automaton is not deterministic
    if(UTILS::isInSet(alphabet, alphabetSize, UTILS::EPS));

    // With each letter we check if there is more than one
    // transition with the same stateFrom and the current literal
    for(int i = 0; i < alphabetSize; ++i) {
        bool* statesTransitions = new bool[statesCount];
        for(int k = 0; k < statesCount; ++k)
            statesTransitions[k] = false;

        for(int j = 0; j < transitionsCount; ++j) {
            if (transitions[j].literal == alphabet[i]) {
                if(statesTransitions[transitions[j].stateFrom]) {
                    return false;

                }
                statesTransitions[transitions[j].stateFrom] = true;
            }
        }
        delete[] statesTransitions;
    }
    return true;
}

bool NFA::isEmpty() const {
    // We check if there are any states or transitions at all
    if(statesCount == 0) return true;

        // If the start state is final our language contains the empty word
    else if(states[0].isFinal) return false;

        // .. but if it's not and there aren't any transitions so the lang is empty!
        // The rest is self explanatory.
    else if(transitionsCount == 0) return true;
    else {
        for(int i = 0; i < transitionsCount; ++i)
            if(transitions[i].stateFrom == 0)
                return false;
        return true;
    }
}

bool NFA::recognise(const char* regex) const {
    for(int i = 0; i < strlen(regex); ++i)
        if(!UTILS::isInSet(alphabet, alphabetSize, regex[i]))
            return false;

    if(!std::strcmp(regex, "E"))
        if(states[UTILS::START_STATE].isFinal)
            return true;

    int* currentStates = new int[getStatesCount()];
    int currCount = 0;
    currentStates[currCount++] = UTILS::START_STATE;
    epsilonClosure(currentStates[currCount - 1], currentStates, &currCount);

    for(int i = 0; i < strlen(regex); ++i) {
        int* newCurrStates = new int[getStatesCount()];
        int newCurrCount = 0;
        for(int j = 0; j < currCount; ++j) {
            for(int k = 0; k < transitionsCount; ++k) {
                if(transitions[k].stateFrom == currentStates[j]
                   && transitions[k].literal == regex[i]
                   && !UTILS::isInSet(newCurrStates, newCurrCount, transitions[k].stateTo)) {

                    int *epsilonClose = new int[getStatesCount()];
                    int epsilonStatesCount = 0;
                    epsilonClose[epsilonStatesCount++] = transitions[k].stateTo;
                    epsilonClosure(transitions[k].stateTo, epsilonClose, &epsilonStatesCount);
                    for (int m = 0; m < epsilonStatesCount; ++m)
                        if (!UTILS::isInSet(newCurrStates, newCurrCount, epsilonClose[m]))
                            newCurrStates[newCurrCount++] = epsilonClose[m];
                    delete[] epsilonClose;
                }
            }
        }
        delete[] currentStates;
        currCount = 0;
        currentStates = new int[getStatesCount()];
        for(int j = 0; j < newCurrCount; ++j)
            currentStates[currCount++] = newCurrStates[j];
        for(int j = 0; j < currCount; ++j) {
            int* epsilonClose = new int[getStatesCount()];
            int epsilonCloseStatesCount = 0;
            epsilonClosure(currentStates[j], epsilonClose, &epsilonCloseStatesCount);
            for(int k = 0; k < epsilonCloseStatesCount; ++k)
                if(!UTILS::isInSet(currentStates, currCount, epsilonClose[k]))
                    currentStates[currCount++] = epsilonClose[k];
        }

        delete[] newCurrStates;
    }
    for( int i = 0; i < currCount; ++i)
        if(states[currentStates[i]].isFinal)
            return true;
    return false;
}

void NFA::printNFA() const {
    std::cout << "ID: " << getID() << std::endl;
    std::cout << "Number of states: " << getStatesCount() << std::endl;
    std::cout << "Alphabet: ";
    for(int i = 0; i < alphabetSize; ++i)
        std::cout << alphabet[i] << ' ';
    std::cout << std::endl;
    std::cout << "Deterministic: " << (isDeterministic() ? "True" : "False") << std::endl;

    for(int i = 0; i < transitionsCount; ++i) {
        // These two are for putting a star after a final state
        char starFrom = states[transitions[i].stateFrom].isFinal ? '*' : ' ';
        char starTo = states[transitions[i].stateTo].isFinal ? '*' : ' ';

        std::cout << "state" << transitions[i].stateFrom << starFrom << ", "
                  << transitions[i].literal << " -> state" << transitions[i].stateTo
                  << starTo << std::endl;
    }
}

std::ostream& operator<<(std::ostream& os, const NFA& nfa) {
    os << 'i' << nfa.id << std::endl;
    os << nfa.getStatesCount() << std::endl;
    for(int i = 0; i < nfa.transitionsCount; ++i) {
        const char* star = nfa.states[nfa.transitions[i].stateFrom].isFinal ? " * " : " ";
        os << nfa.transitions[i].stateFrom << star
           << nfa.transitions[i].literal << " "
           << nfa.transitions[i].stateTo << std::endl;
    }
    return os;
}