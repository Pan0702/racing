#include "UndoManager.h"

#include "TRSObject/TRS.h"

UndoManager::UndoManager()
{
    data_ = ObjectManager::FindGameObject<StageData>();
}

// 現在選択中のオブジェクトのTransformをUndoスタックに積む
void UndoManager::Push()
{
    int index = data_->GetIndex();
    UndoState state;
    state.index = index;
    state.trans = *data_->GetSelectedTransform();;
    undo_stack_.push(state);
    // 新しい操作をしたら Redo 履歴は無効になるためクリア
    redo_stack_ = std::stack<UndoState>();
}

// 直前の操作を取り消し、現在状態をRedoスタックに退避する
void UndoManager::Undo()
{
    // Redo できるよう、Undoを適用する前に現在状態を Redo スタックへ退避
    UndoState state;
    state.index = data_->GetIndex();
    state.trans = *data_->GetSelectedTransform();
    redo_stack_.push(state);

    if (undo_stack_.empty()) return;

    // Undo スタックの先頭を取り出して適用
    state = undo_stack_.top();
    undo_stack_.pop();

    data_->SetSelectedTransform(state.index, state.trans);
}

// 取り消した操作をやり直す
void UndoManager::Redo()
{
    if (redo_stack_.empty()) return;
    UndoState state = redo_stack_.top();
    redo_stack_.pop();   
    data_->SetSelectedTransform(state.index, state.trans);
}
