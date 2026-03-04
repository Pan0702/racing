#include "UndoManager.h"

#include "TRSObject/TRS.h"

UndoManager::UndoManager()
{
    data_ = ObjectManager::FindGameObject<StageData>();
}

void UndoManager::Push()
{
    StageData* data = data_;
    Transform* trans = data->GetSelectedTransform();
    if (trans == nullptr) return;  
    int index = data->GetIndex();
    UndoState state;
    state.index = index;
    state.before = *trans;
    stack_.push(state);
}

void UndoManager::Undo()
{
    if (stack_.empty()) return;
    UndoState state = stack_.top();
    stack_.pop();   
    data_->SetSelectedTransform(state.index, state.before);
}
