#include "UndoManager.h"

#include "TRSObject/TRS.h"

UndoManager::UndoManager()
{
    data_ = ObjectManager::FindGameObject<StageData>();
}

void UndoManager::Push()
{
    int index = data_->GetIndex();
    UndoState state;
    state.index = index;
    state.trans = *data_->GetSelectedTransform();;
    undo_stack_.push(state);
    redo_stack_ = std::stack<UndoState>();
}

void UndoManager::Undo()
{
    UndoState state;
    state.index = data_->GetIndex();
    state.trans = *data_->GetSelectedTransform();
    redo_stack_.push(state);
    if (undo_stack_.empty()) return;
    state = undo_stack_.top();
    undo_stack_.pop();   

    data_->SetSelectedTransform(state.index, state.trans);
}

void UndoManager::Redo()
{
    if (redo_stack_.empty()) return;
    UndoState state = redo_stack_.top();
    redo_stack_.pop();   
    data_->SetSelectedTransform(state.index, state.trans);
}
