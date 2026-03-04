#pragma once
#include "Camera.h"
#include "UndoManager.h"
#include "../Object3D.h"
#include "TRSObject/TRS.h"

class Controller : public Object3D
{
private:
    Camera* camera_;
    TRS* trs_;
    CDirectInput* input_;
    UndoManager* undo_manager_;
    StageData* stage_data_;
    bool is_catch = false;
private:
    void Update() override;
    void Draw() override;
    void TRSControl() const;
    void CameraControl() const;
    void HandleLeftClick();
    void HandleUndoRedo();
public:
    Controller();
    ~Controller() = default;
    
};
