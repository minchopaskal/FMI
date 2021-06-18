#ifndef SDP_HOMEWORK_EDITOR_H
#define SDP_HOMEWORK_EDITOR_H

#include <stack>
#include <string>

class Editor {
private:
    std::string text;

    std::stack<std::string> undoStack;
    std::stack<std::string> redoStack;

public:
    Editor(std::string);

    void insert(unsigned long, std::string);
    void remove(unsigned long, unsigned long);

    void undo();
    void redo();

    std::string getText() const { return this->text; }
};


#endif //SDP_HOMEWORK_EDITOR_H
