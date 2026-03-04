#pragma once
#include <stack>
#include "StageData.h"

class UndoManager
{
private:
    struct UndoState
    {
        int index; 
        Transform before;
    };
    std::stack<UndoState> stack_;
    StageData* data_;

public:
    UndoManager();
    ~UndoManager() = default;
    void Push();
    void Undo();
};
