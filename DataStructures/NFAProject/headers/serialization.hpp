/*
 * -----------------------------------------------------------------------------
 * File: serialization.hpp
 * -----------------------------------------------------------------------------
 * Author: Mincho Paskaleff
 * -----------------------------------------------------------------------------
 */

#ifndef NFA_SERIALIZATION_H
#define NFA_SERIALIZATION_H

#include <fstream>
#include <string>
#include "nfa.hpp"
#include "utils.hpp"

bool serialize(const NFA& nfa, const char* fileName) {
    std::string data = "../data/";
    std::string replaceFile = "replace.nfa";
    std::string sFileName = fileName;
    std::string ext = ".nfa";
    if(sFileName.find(ext) == std::string::npos)
        sFileName.append(ext.c_str());

    std::string pathCurr = data + sFileName;
    std::string pathReplace = data + replaceFile;

    std::ifstream ifs(pathCurr);
    std::ofstream ofs(pathReplace, std::ios::app);

    if(!ifs.is_open() || !ofs.is_open()) {
        std::cout << "Unable to open file... \n";
        return false;
    }

    bool isEmpty = ifs.peek() == std::ifstream::traits_type::eof();
    if(!isEmpty) {
        int nfasCount;
        ifs >> nfasCount;
        ++nfasCount;
        ofs << nfasCount;
        std::string line;
        while (std::getline(ifs, line))
            ofs << line << std::endl;
    } else {
        ofs << '1' << std::endl;
    }

    ofs << nfa;
    std::remove(pathCurr.c_str());
    std::rename(pathReplace.c_str(), pathCurr.c_str());
    return true;
}

NFA** deserialize(const char* fileName, int& nfasCount) {
    std::string data = "../data/";

    std::string sFileName = fileName;
    std::string ext = ".nfa";
    if(sFileName.find(ext) == std::string::npos)
        sFileName.append(ext.c_str());


    std::string path = data + sFileName;

    std::ifstream ifs(path);
    if(!ifs.is_open())
        return nullptr;

    nfasCount = ifs.get() - '0';
    if(!nfasCount)
        return nullptr;

    NFA** nfas = new NFA*[nfasCount];
    for(int i = 0; i < nfasCount; ++i)
        nfas[i] = new NFA();

    ifs.get(); ifs.get();
    for(int i = 0; i < nfasCount; ++i) {
        ifs >> nfas[i]->id;
        ifs.get();
        int statesCount = ifs.get() - '0';
        nfas[i]->setStates(statesCount);

        ifs.get();
        ifs.get();
        int j = 0;
        do {
            ifs.seekg(-1, std::ios::cur);
            int stateFrom;
            ifs >> stateFrom;
            ifs.seekg(1, std::ios::cur);
            char literal = ifs.get();
            if (literal == UTILS::KLEENE) {
                nfas[i]->setFinalState(stateFrom);
                ifs.seekg(1, std::ios::cur);
                literal = ifs.get();
            }
            int stateTo;
            ifs >> stateTo;
            nfas[i]->addTransition(stateFrom, literal, stateTo);
            ifs.get();
            ++j;
        }while(ifs.get() != 'i' && !ifs.eof());
        nfas[i]->transitionsCount = j;
    }

    return nfas;
}

#endif //NFA_SERIALIZATION_H
