/*
 * -----------------------------------------------------------------------------
 * File: nfa_helper.hpp
 * -----------------------------------------------------------------------------
 * Author: Mincho Paskaleff
 * -----------------------------------------------------------------------------
 */

#ifndef NFA_HELPER_H
#define NFA_HELPER_H

#include "nfa.hpp"
#include "../src/dynamic_array.cpp"

class NFAHelper {
private:
    // Help functions for current class
    static void translateFinalStates(NFA&, const NFA&, int);
    static void translateStatesUnion(NFA&, const NFA&, int);
    static void translateStatesConcat(NFA&, const NFA&, const NFA&);
    static void positivelyClose(NFA&);
    static void thompsonTranslateStates(NFA&, const NFA&, const int);

    static bool idIsInStack(DynamicArray<NFA*>&, int);
public:
    // Utility Functions
    static NFA getNFAByID(DynamicArray<NFA*>, int, int);
    static NFA* getNFAPointerByID(DynamicArray<NFA*>, int, int);
    static NFA emptyNFA();
    static void addToStack(DynamicArray<NFA*>&, NFA*);

    /* Functions returning standard eNFA-s supporting multiple
     * final states
     */

    // Union of two NFAs
    static NFA getUnion(const NFA&, const NFA&);

    // Concatenation of two NFAs
    static NFA getConcatenation(const NFA&, const NFA&);

    // Positive closure of NFA
    static NFA getPositiveClosure(const NFA&);

    // Kleene Star of NFA
    static NFA getKleeneStar(const NFA&);

    /* End of basic functions */

    /*
     * Thompson Algorithm
     */
    /* Standart eNFA-s for Thompson's construction
     * Always work with one final state
     */
    static NFA getEpsilonNFA();
    static NFA getLiteralNFA(char);
    static NFA thompsonUnion(const NFA&, const NFA&);
    static NFA thompsonConcat(const NFA&, const NFA&);
    static NFA thompsonPositiveClosure(const NFA&);
    static NFA thompsonKleene(const NFA&);
    static NFA regexToNFA(const char*);
};

#endif /* NFA_HELPER_H */
