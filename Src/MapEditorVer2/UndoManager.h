#pragma once
#include <stack>
#include "StageData.h"

class UndoManager
{
private:
    struct UndoState
    {
        int index; 
        Transform trans;
    };
    std::stack<UndoState> undo_stack_;
    std::stack<UndoState> redo_stack_;
    StageData* data_;
    

public:
    UndoManager();
    ~UndoManager() = default;
    void Push();
    void Undo();
    void Redo();
};
