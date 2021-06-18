/*
 * -----------------------------------------------------------------------------
 * File: nfa_helper.cpp
 * -----------------------------------------------------------------------------
 * Author: Mincho Paskaleff
 * -----------------------------------------------------------------------------
 */

#include <iostream>
#include <cstring>
#include "../headers/nfa_helper.hpp"
using namespace UTILS;

/*
 * Private Functions
 */

// Function for translating final states, adderConst is for renaming states
void NFAHelper::translateFinalStates(NFA& result, const NFA& other, int adderConst) {
    for(unsigned int i = 0; i < other.getStatesCount(); ++i)
        if(other.states[i].isFinal)
            result.setFinalState(i + adderConst);
}

// Function for translating states in union
void NFAHelper::translateStatesUnion(NFA& result, const NFA& other, int adderConst) {
    // Translating final states
    translateFinalStates(result, other, adderConst);

    // Copying states
    for(int i = 0; i < other.getTransitionsCount(); ++i) {
        int stateFrom = other.transitions[i].stateFrom;
        char literal = other.transitions[i].literal;
        int stateTo = other.transitions[i].stateTo + adderConst;
        if(stateFrom == START_STATE)
            result.addTransition(START_STATE, literal, stateTo );
        result.addTransition(stateFrom + adderConst, literal, stateTo);
    }
}

void NFAHelper::translateStatesConcat(NFA& result, const NFA& fst, const NFA& snd) {
    int fstFinalStatesCount;
    NFA::state* fstFinalStates = fst.getFinalStates(&fstFinalStatesCount);
    // Copying transitions from first automaton without changes
    for(int i = 0; i < fst.getTransitionsCount(); ++i) {
        int stateFrom = fst.transitions[i].stateFrom;
        char literal = fst.transitions[i].literal;
        int stateTo = fst.transitions[i].stateTo;
        result.addTransition(stateFrom, literal, stateTo);
    }

    int addConst = fst.getStatesCount();
    for(int i = 0; i < snd.getTransitionsCount(); ++i) {
        int stateFrom = snd.transitions[i].stateFrom;
        char literal = snd.transitions[i].literal;
        int stateTo = snd.transitions[i].stateTo + addConst;
        if(stateFrom == START_STATE)
        for(int j = 0; j < fstFinalStatesCount; ++j)
            result.addTransition(fstFinalStates[j].name, literal, stateTo);
        result.addTransition(stateFrom + addConst, literal, stateTo);
    }
}

void NFAHelper::positivelyClose(NFA& result) {
    int finalStatesCount;
    NFA::state* finalStates = result.getFinalStates(&finalStatesCount);
    int transitionsCount = result.getTransitionsCount();
    for(int i = 0; i < transitionsCount; ++i) {
        if(result.transitions[i].stateFrom == START_STATE) {
            char literal = result.transitions[i].literal;
            int stateTo = result.transitions[i].stateTo;
            for(int j = 0; j < finalStatesCount; ++j)
                if(finalStates[j].name != START_STATE)
                    result.addTransition(finalStates[j].name, literal, stateTo);
        }
    }
}

void NFAHelper::thompsonTranslateStates(NFA& result, const NFA& other, const int adderConst) {
    // Copying states
    for(int i = 0; i < other.getTransitionsCount(); ++i) {
        int stateFrom = other.transitions[i].stateFrom + adderConst;
        char literal = other.transitions[i].literal;
        int stateTo = other.transitions[i].stateTo + adderConst;
        result.addTransition(stateFrom, literal, stateTo);
    }
}

bool NFAHelper::idIsInStack(DynamicArray<NFA*>& automata, int ID) {
    for(int i = 0; i < automata.getLength(); ++i)
        if(automata.getElemAt(i)->getID() == ID)
            return true;
    return false;
}

/*
 * Utility Functions
 *
 */
NFA NFAHelper::getNFAByID(DynamicArray<NFA*> automata, int ID, int automataCount) {
    for(int i = 0; i < automataCount; ++i)
        if(automata.getElemAt(i)->getID() == ID)
            return *(automata.getElemAt(i));
    std::cout << "No such automaton! Take an empty one!" << std::endl;
    return emptyNFA();
}

NFA* NFAHelper::getNFAPointerByID(DynamicArray<NFA*> automata, int ID, int automataCount) {
    for(int i = 0; i < automataCount; ++i)
        if(automata.getElemAt(i)->getID() == ID)
            return automata.getElemAt(i);
    return nullptr;
}

NFA NFAHelper::emptyNFA() {
    NFA empty;
    empty.setStates(1);
    return empty;
}

void NFAHelper::addToStack(DynamicArray<NFA*>& automata, NFA* nfa) {
    while(NFAHelper::idIsInStack(automata, nfa->getID())) {
        ++nfa->id;
    }
    automata.add(nfa);

}

/*
 * Basic Functions
 */

// Union of NFAs
NFA NFAHelper::getUnion(const NFA& first, const NFA& second) {
    /* first = {Q1(states), T1(transitions), s1(start state), F1(final states)};
     * second = {Q2, T2, s2, F2};
     * result = { Q1 + Q2, T, s1, F};
     * Where :
     * T = | T1(q,a) if q belongs to Q1
     *     | T2(q,a) if q belongs to Q2
     *     | T1(s1,a)+T2(s2,a) if q = s
     * F = | F1 + F2 if s2 belongs to F2
     *     | F2 else
     */
    NFA result;
    result.setStates(first.getStatesCount() + second.getStatesCount() + 1);

    if(first.states[START_STATE].isFinal || second.states[START_STATE].isFinal)
    result.setFinalState(START_STATE);

    // We translate the final states to the result NFA
    translateStatesUnion(result, first, 1);
    translateStatesUnion(result, second, 1 + first.getStatesCount());

    return result;

}

// Concatenation of NFAs
NFA NFAHelper::getConcatenation(const NFA& first, const NFA& second) {
    /* first = {Q1, T1, s1, F1};
     * second = {Q2, T2, s2, F2};
     * result = { Q1 + Q2, T, s1, F};
     * Where :
     * T = | T1(q, a) if q belongs to Q1 \ F1
     *     | T1(q, a) + T2(s2, a) if q belongs to F1
     *     | T2(q, a) if q belongs to Q2
     * F = | F1 + F2 if s2 belongs to F2
     *     | F2 else
     */
    NFA result;
    result.setStates(first.getStatesCount() + second.getStatesCount());

    if(second.states[START_STATE].isFinal)
    translateFinalStates(result, first, 0);
    translateFinalStates(result, second, first.getStatesCount());

    translateStatesConcat(result, first, second);
    return result;
}

// Positive closure of a NFA
NFA NFAHelper::getPositiveClosure(const NFA& original) {
    /* original = {Q, T, s, F};
     * result = {Q, T+, s, F};
     * Where :
     * T+ = | T(q, a) if q belongs to Q \ F
     *      | T(q, a) + T2(s, a) if q belongs to F
     */
    NFA result(original);
    positivelyClose(result);
    return result;
}

// Kleene star of a NFA
NFA NFAHelper::getKleeneStar(const NFA& original) {
    /* Kleene Star:
     * L* = EPS + (L+)
     */
    NFA result = getPositiveClosure(original);
    result.setFinalState(0);
    return result;
}

/*
 * Thompson Algorithm
 */

// NFA of the empty word
NFA NFAHelper::getEpsilonNFA() {
    NFA result;
    result.setStates(2);
    result.setFinalState(1);
    result.addTransition(START_STATE, EPS , 1);

    return result;
}

// NFA of one character regex
NFA NFAHelper::getLiteralNFA(char literal) {
    if(literal == 'E')
        return getEpsilonNFA();

    NFA result;
    result.setStates(2);
    result.setFinalState(1);
    result.addTransition(START_STATE, literal, 1);

    return result;
}

NFA NFAHelper::thompsonUnion(const NFA& first, const NFA& second) {
    /*
     * 'Start state goes via ε either to the initial state of first or second.
     * Their final states become intermediate states of
     * the whole NFA and merge via two ε-transitions into the final state of the result NFA.'
     * - Quoted from Wikipedia
     */

    NFA result;
    int fstCount = first.getStatesCount();
    int sndCount = second.getStatesCount();
    int resCount = fstCount + sndCount + 2;

    result.setStates(resCount);
    // Add EPS transitions to the start states
    result.addTransition(START_STATE, EPS, 1);
    result.addTransition(START_STATE, EPS, fstCount + 1);
    // Add EPS transitions to the final state - it's always the last one
    result.addTransition(fstCount, EPS, resCount - 1);
    result.addTransition(resCount - 2, EPS, resCount - 1);

    // Copying the states from the automata in the result
    thompsonTranslateStates(result, first, 1);
    thompsonTranslateStates(result, second, fstCount + 1);

    result.setFinalState(resCount - 1);
    return result;
}

NFA NFAHelper::thompsonConcat(const NFA& first, const NFA& second) {
    /*
     * The initial state of first is the initial state of the whole NFA.
     * The final state of first becomes the initial state of  second.
     * The final state of second is the final state of the whole NFA.
     */
    NFA result;
    int fstCount = first.getStatesCount();
    int sndCount = second.getStatesCount();
    int resCount = fstCount + sndCount - 1;

    result.setStates(resCount);

    thompsonTranslateStates(result, first, 0);
    thompsonTranslateStates(result, second, fstCount - 1);
    result.setFinalState(resCount - 1);

    return result;
}

NFA NFAHelper::thompsonPositiveClosure(const NFA& original) {
    /*
     * ε-transition from the inner final to the inner
     * initial state of original allows for repetition of expression s according
     * to the star operator.
     */
    NFA result;
    int origCount = original.getStatesCount();
    int resCount = origCount + 2;
    result.setStates(resCount);

    thompsonTranslateStates(result, original, 1);
    result.addTransition(START_STATE, EPS, 1);
    result.addTransition(origCount, EPS, resCount - 1);
    result.addTransition(origCount, EPS, 1);
    result.setFinalState(resCount - 1);

    return result;
}


NFA NFAHelper::thompsonKleene(const NFA& original) {
    /*
     * Positive closure except we have
     * another ε-transition connecting initial and final state of the NFA with the sub-NFA original
     * in between.
     */
    NFA result = thompsonPositiveClosure(original);
    result.addTransition(START_STATE, EPS, result.getStatesCount() - 1);

    return result;
}

NFA NFAHelper::regexToNFA(const char* regex) {
    // Reading the regex using something similar to Shunting-yard algorithm
    Stack<char> operators;
    Stack<NFA> operands;

    size_t regexLength = strlen(regex);
    for(int i = 0; i < regexLength; ++i) {
        char currSymbol = regex[i];
        switch(currSymbol) {
            case UTILS::LEFT_BRACKET:
                break;
            case UTILS::KLEENE: {
                NFA kleene = operands.pop();
                operands.push(thompsonKleene(kleene));
                break;
            }
            case UTILS::PLUS: {
                NFA positive = operands.pop();
                operands.push(thompsonPositiveClosure(positive));
                break;
            }
            case UTILS::CONCAT:
            case UTILS::UNION:
                operators.push(currSymbol);
                break;
            case UTILS::RIGHT_BRACKET: {
                if (regex[i - 1] == UTILS::KLEENE || regex[i - 1] == UTILS::PLUS)
                    break;
                NFA second = operands.pop();
                NFA first = operands.pop();
                char operation = operators.pop();
                if (operation == UTILS::CONCAT)
                    operands.push(thompsonConcat(first, second));
                else if(operation == UTILS::UNION)
                    operands.push(thompsonUnion(first, second));
                break;
            }
            default:
                operands.push(getLiteralNFA(currSymbol));
                break;
        }
    }
    return operands.pop();

}
