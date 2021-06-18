#include "headers/includes.hpp"

/*
 * Unit tests
 */
void initAutomata(NFA& first, NFA& second) {
    first.setStates(4);
    first.addTransition(0, 'a', 1);
    first.addTransition(0, 'b', 2);
    first.addTransition(1, 'c', 3);
    first.addTransition(2, 'a', 3);
    first.addTransition(3, 'c', 1);
    first.setFinalState(3);

    second.setStates(3);
    second.addTransition(0, 'a', 1);
    second.addTransition(0, 'c', 2);
    second.addTransition(1, 'a', 0);
    second.addTransition(1, 'b', 2);
    second.addTransition(2, 'a', 0);
    second.addTransition(2, 'a', 1);
    second.setFinalState(0);
    second.setFinalState(2);
}

void testPrint(const NFA& first, const NFA& second) {
    std::cout << "*****************************\n";
    std::cout << " First NFA:\n";
    std::cout << "*****************************\n";
    first.printNFA();
    std::cout << "*****************************\n\n";

    std::cout << "*****************************\n";
    std::cout << " Second NFA:\n";
    std::cout << "*****************************\n";
    second.printNFA();
    std::cout << "*****************************\n\n";
}

void testUnion(const NFA& first, const NFA& second) {
    NFA unionize = NFAHelper::getUnion(first, second);

    std::cout << "*****************************\n";
    std::cout << " Union of both NFAs:\n";
    std::cout << "*****************************\n";
    unionize.printNFA();
    std::cout << "*****************************\n\n";
}

void testConcat(const NFA& first, const NFA& second) {
    NFA concat = NFAHelper::getConcatenation(first, second);


    std::cout << "*****************************\n";
    std::cout << " Concatenation of both NFAs:\n";
    std::cout << "*****************************\n";
    concat.printNFA();
    std::cout << "*****************************\n\n";
}

void testPositiveClosure(const NFA& first) {
    NFA pos = NFAHelper::getPositiveClosure(first);

    std::cout << "*****************************\n";
    std::cout << " Positive closure of the first NFA:\n";
    std::cout << "*****************************\n";
    pos.printNFA();
    std::cout << "*****************************\n\n";
}

void testKleene(const NFA& first) {
    NFA kleene = NFAHelper::getKleeneStar(first);

    std::cout << "*****************************\n";
    std::cout << " Kleene star of the first NFA:\n";
    std::cout << "*****************************\n";
    kleene.printNFA();
    std::cout << "*****************************\n\n";
}

void testStack(const NFA& one, const NFA& two) {
    Stack<NFA> nfaStack;
    nfaStack.push(one);
    nfaStack.push(two);
    NFA test = nfaStack.pop();
    test.printNFA();
    std::cout << "Nfa in nfaStack ID: " << nfaStack.peek().getID() << std::endl;

    Stack<int> intStack;
    for(int i = 0; i < 10; ++i)
        intStack.push(i);
    for(int i = 0; i < 10; ++i)
        std::cout << intStack.pop() << " ";
    std::cout << std::endl << "Is intStack empty? " << intStack.empty() << std::endl;
}

void testThompson(const char* regex) {
    NFA regNFA = NFAHelper::regexToNFA(regex);
    std::cout << "*****************************\n";
    std::cout << " NFA made by the regex: " << regex << std::endl;
    std::cout << "*****************************\n";
    regNFA.printNFA();
    std::cout << "*****************************\n\n";

}

void testDeterministic() {
    NFA deterministic;
    NFA nonDeterministic;

    // Init of automaton
    deterministic.setStates(3);
    deterministic.addTransition(0, 'a', 1);
    deterministic.addTransition(0, 'b', 2);
    deterministic.addTransition(0, 'c', 1);
    deterministic.addTransition(1, 'a', 0);
    deterministic.addTransition(1, 'b', 1);
    deterministic.addTransition(1, 'c', 2);
    deterministic.addTransition(2, 'a', 0);
    deterministic.addTransition(2, 'c', 2);
    deterministic.addTransition(2, 'b', 1);
    deterministic.setFinalState(2);

    nonDeterministic.setStates(2);
    nonDeterministic.addTransition(0, 'a', 0);
    nonDeterministic.addTransition(0, 'a', 1);
    nonDeterministic.addTransition(0, 'E', 0);
    nonDeterministic.addTransition(0, 'E', 1);
    nonDeterministic.addTransition(1, 'b', 1);
    nonDeterministic.addTransition(1, 'b', 0);
    nonDeterministic.addTransition(1, 'c', 0);

    std::cout << "*****************************\n";
    std::cout << " Deterministic NFA\n";
    std::cout << "*****************************\n";
    deterministic.printNFA();
    std::cout << "*****************************\n\n";

    std::cout << "*****************************\n";
    std::cout << " Non-deterministic NFA\n";
    std::cout << "*****************************\n";
    nonDeterministic.printNFA();
    std::cout << "*****************************\n\n";

}

void testSerialization() {
    NFA** nfas;
    int nfasCount;

    nfas = deserialize("sample.nfa", nfasCount);
    for(int i = 0; i < nfasCount; ++i)
        nfas[i]->printNFA();


    serialize(*(nfas[0]), "sample.nfa");

    for(int i = 0; i < nfasCount; ++i)
        delete nfas[i];
    delete[] nfas;

    std::cout << "*****************************\n";
    std::cout << " Serialization of first automaton in the same file:\n";
    std::cout << "*****************************\n";

    nfas = deserialize("sample.nfa", nfasCount);
    for(int i = 0; i < nfasCount; ++i)
        nfas[i]->printNFA();
    delete[] nfas;
}

void unitTests(NFA& first, NFA& second) {
    initAutomata(first, second);
    std::cout << "---------------------------------\n";
    testPrint(first, second);
    std::cout << "---------------------------------\n";
    testUnion(first, second);
    std::cout << "---------------------------------\n";
    testConcat(first, second);
    std::cout << "---------------------------------\n";
    testPositiveClosure(first);
    std::cout << "---------------------------------\n";
    testKleene(first);
    std::cout << "---------------------------------\n";
    testStack(first, second);
    std::cout << "---------------------------------\n";
    testThompson("((a.b)|((a*)|(a|b)))");
    std::cout << "---------------------------------\n";
    testDeterministic();
    std::cout << "---------------------------------\n";
    testSerialization();
    std::cout << "---------------------------------\n";


}

void mainLoop() {
    DynamicArray<NFA*> nfaStack;

    do {
        std::cout << "\n**************************************************\n";
        std::cout << " Please choose a option:\n";
        std::cout << "**************************************************\n";
        std::cout << "(0) Open and read a file of eNFA-s;\n";
        std::cout << "(1) List all eNFA-s' id-s;\n";
        std::cout << "(2) Print an eNFA by its ID;\n";
        std::cout << "(3) Check if an eNFA is empty by its ID;\n";
        std::cout << "(4) Check if an eNFA is deterministic;\n";
        std::cout << "(5) Check if a word is in an automaton's language;\n";
        std::cout << "(6) Find the union of two eNFA-s;\n";
        std::cout << "(7) Find the concatenation of two eNFA-s;\n";
        std::cout << "(8) Find the positive closure of an eNFA;\n";
        std::cout << "(9) Find the kleene star of an eNFA;\n";
        std::cout << "(10) Create a new eNFA by a regex;\n";
        std::cout << "(11) Save an eNFA in a file;\n";
        std::cout << "(  ) Currently not working:\n";
        std::cout << "(12) Find if the language of an eNFA is finite;\n";
        std::cout << "(13) Determinize an eNFA;\n";
        std::cout << "(14) Minimize an eNFA;\n";
        std::cout << "(15) Exit the program;\n";

        std::cout << "> ";
        int choice;
        bool exit = false;
        std::cin >> choice;
        std::cout << std::endl;

        int id;
        int id2;
        NFA currNFAFirst;
        NFA currNFASecond;
        NFA* newNFA;
        std::string fileName;
        std::string word;
        int nfasCount = 0;

        switch(choice) {
            case 0:
                std::cout << "Please enter a valid file name: ";
                std::cin >> fileName;
                NFA** nfas;
                nfas = deserialize(fileName.c_str(), nfasCount);
                if(nfas == nullptr) {
                    std::cout << "Reading unsuccessful...\n";
                    break;
                }
                for(int i = 0; i < nfasCount; ++i)
                    NFAHelper::addToStack(nfaStack, nfas[i]);
                std::cout << "File read successfully...\n";
                break;
            case 1:
                std::cout << "\nList of all eNFA-s id-s:\n";
                for(int i = 0; i < nfaStack.getLength(); ++i)
                    std::cout << i << ". id: " << nfaStack.getElemAt(i)->getID() << std::endl;
                break;
            case 2:
                std::cout << "Please enter a valid eNFA id: ";
                std::cin >> id;
                newNFA = NFAHelper::getNFAPointerByID(nfaStack, id, nfaStack.getLength());
                std::cout << "eNFA by id " << id << ":\n";
                newNFA->printNFA();
                break;
            case 3:
                std::cout << "Please enter a valid eNFA id: ";
                std::cin >> id;
                currNFAFirst = NFAHelper::getNFAByID(nfaStack, id, nfaStack.getLength());
                std::cout << "eNFA by id " << id << " is ";
                if(!currNFAFirst.isEmpty())
                    std::cout << "not ";
                std::cout << "empty!\n";
                break;
            case 4:
                std::cout << "Please enter a valid eNFA id: ";
                std::cin >> id;
                currNFAFirst = NFAHelper::getNFAByID(nfaStack, id, nfaStack.getLength());
                std::cout << "eNFA by id " << id << " is ";
                if(!currNFAFirst.isDeterministic())
                    std::cout << "not ";
                std::cout << "deterministic!\n";
                break;
            case 5:
                std::cout << "Please enter a valid eNFA id: ";
                std::cin >> id;
                currNFAFirst = NFAHelper::getNFAByID(nfaStack, id, nfaStack.getLength());
                std::cout << "Please enter a word to be recognised: ";
                std::cin >> word;
                std::cout << "eNFA by id " << id;
                if(!currNFAFirst.recognise(word.c_str()))
                    std::cout << " does not";
                std::cout << " have the word " << word << " in its language!\n";
                break;
            case 6:
                std::cout << "Please enter a valid id of the first eNFA: ";
                std::cin >> id;
                currNFAFirst = NFAHelper::getNFAByID(nfaStack, id, nfaStack.getLength());
                std::cout << "Please enter a valid id of the second eNFA: ";
                std::cin >> id2;
                currNFASecond = NFAHelper::getNFAByID(nfaStack, id2, nfaStack.getLength());
                std::cout << "Union of eNFAs with id-s " << id << " " << id2 << ":\n";
                newNFA = new NFA(NFAHelper::getUnion(currNFAFirst, currNFASecond));
                newNFA->printNFA();
                NFAHelper::addToStack(nfaStack, newNFA);
                break;
            case 7:
                std::cout << "Please enter a valid id of the first eNFA: ";
                std::cin >> id;
                currNFAFirst = NFAHelper::getNFAByID(nfaStack, id, nfaStack.getLength());
                std::cout << "Please enter a valid id of the second eNFA: ";
                std::cin >> id2;
                currNFASecond = NFAHelper::getNFAByID(nfaStack, id2, nfaStack.getLength());
                std::cout << "Concatenation of eNFAs with id-s " << id << " " << id2 << ":\n";
                newNFA = new NFA(NFAHelper::getUnion(currNFAFirst, currNFASecond));
                newNFA->printNFA();
                NFAHelper::addToStack(nfaStack, newNFA);
                break;
            case 8:
                std::cout << "Please enter a valid eNFA id: ";
                std::cin >> id;
                currNFAFirst = NFAHelper::getNFAByID(nfaStack, id, nfaStack.getLength());
                std::cout << "Pos. closure of eNFA with id " << id << ":\n";
                newNFA = new NFA(NFAHelper::getPositiveClosure(currNFAFirst));
                newNFA->printNFA();
                NFAHelper::addToStack(nfaStack, newNFA);
                break;
            case 9:
                std::cout << "Please enter a valid eNFA id: ";
                std::cin >> id;
                currNFAFirst = NFAHelper::getNFAByID(nfaStack, id, nfaStack.getLength());
                std::cout << "Kleene star of eNFA with id " << id << ":\n";
                newNFA = new NFA(NFAHelper::getKleeneStar(currNFAFirst));
                newNFA->printNFA();
                NFAHelper::addToStack(nfaStack, newNFA);
                break;
            case 10:
                std::cout << "Please enter a valid regular expr(in documentation): ";
                std::cin >> word;
                newNFA = new NFA(NFAHelper::regexToNFA(word.c_str()));
                std::cout << "eNFA constructed via regex " << word << ":\n";
                newNFA->printNFA();
                NFAHelper::addToStack(nfaStack, newNFA);
                break;
            case 11:
                std::cout << "Please enter a valid eNFA id: ";
                std::cin >> id;
                newNFA = NFAHelper::getNFAPointerByID(nfaStack, id, nfaStack.getLength());
                std::cout << "Please enter a valid file name: ";
                std::cin >> fileName;
                if(serialize(*newNFA, fileName.c_str()))
                    std::cout << "Serialization complete...\n";
                delete newNFA;
                break;
            case 12:
                /*
                std::cout << "Please enter a valid regular expr(in documentation): ";
                std::cin >> word;
                currNFAFirst = NFAHelper::getNFAByID(nfaStack, id, nfaStack.getLength());
                std::cout << "eNFA with id is" << id;
                if(currNFAFirst.isInfinite())
                    std::cout << " not";
                std::cout << "finite!\n";
                newNFA->printNFA();*/
            case 13:
                /*
                std::cout << "Please enter a valid regular expr(in documentation): ";
                std::cin >> word;
                newNFA = new NFA(NFAHelper::regexToNFA(word.c_str()));
                newNFA->determinize();
                std::cout << "eNFA with id " << id << " determinized:\n";
                newNFA->printNFA();*/
            case 14:
                /*
                std::cout << "Please enter a valid regular expr(in documentation): ";
                std::cin >> word;
                newNFA = new NFA(NFAHelper::regexToNFA(word.c_str()));
                newNFA->minimize();
                std::cout << "eNFA with id " << id << " determinized:\n";
                newNFA->printNFA();*/
                std::cout << "Not supported!\n";
                break;
            case 15:
                exit = true;
                break;
            default:
                exit = true;
                break;
        }

        if(exit)
            break;
    } while(true);
}

int main() {
    NFA first, second; unitTests(first, second);

    std::cout << "**************************************************\n";
    std::cout << "          Welcome to my eNFA interpreter.         \n";
    std::cout << "**************************************************\n";

    mainLoop();



    return 0;
}
