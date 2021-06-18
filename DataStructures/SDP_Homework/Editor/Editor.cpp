//
// Created by ellie on 11/11/17.
//

#include <iostream>
#include "Editor.h"
Editor::Editor(std::string text) : text(text) {
    //undoStack.push(text);
}

void Editor::insert(unsigned long at, std::string substring) {
    if(at > text.length()) {
        return;
    }
    undoStack.push(text);

    std::string left = text.substr(0, at);
    std::string right = text.substr(at, text.length());

    left.append(substring);
    left.append(right);
    text = left;
}

void Editor::remove(unsigned long at, unsigned long length) {
    if(at > text.length()) {
        return;
    }
    undoStack.push(text);

    std::string left = text.substr(0, at);
    std::string right = text.substr(at + length, text.length() - at - length - 1);
    text = left.append(right);
}

void Editor::undo() {
    if(undoStack.empty()) {
        std::cout << "No operation to undo!\n";
        return;
    }
    redoStack.push(text);
    text = undoStack.top();
    undoStack.pop();
}

void Editor::redo() {
    if(redoStack.empty()) {
        std::cout << "No operation to redo!\n";
        return;
    }
    undoStack.push(text);
    text = redoStack.top();
    redoStack.pop();
}