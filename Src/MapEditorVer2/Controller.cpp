#include "Controller.h"

#include "MouseRay.h"
#include "UndoManager.h"
#include "StageData.h"
#include "TRSObject/TRS.h"

Controller::Controller()
{
    camera_ = ObjectManager::FindGameObject<Camera>();
    trs_ = ObjectManager::FindGameObject<TRS>();
    stage_data_ = ObjectManager::FindGameObject<StageData>();
    input_ = GameDevice()->m_pDI;
    undo_manager_ = new UndoManager();
}


void Controller::Update()
{
    //カメラの移動処理
    if (input_->CheckMouse(KD_DAT, DIM_RBUTTON))
    {
        CameraControl();
    }
    else
    {
        if (is_catch)
        {
            TRSControl();
        }
        else
        {
            trs_->SetState(TRS::State::kNone);
        }
    }

    if (input_->GetMouseWheel() != 0)
    {
        camera_->Zoom();
    }

    if (is_catch)
    {
        bool is_delete = input_->CheckKey(KD_DAT, DIK_BACK) || input_->CheckKey(KD_DAT, DIK_DELETE);
        if (is_delete)
        {
            stage_data_->DeleteModel();
        }
        if (input_->CheckMouse(KD_UTRG, DIM_LBUTTON))
        {
            trs_->SetDraggingAxis(Axis::None);
        }

    }

    // クリックした瞬間
    if (input_->CheckMouse(KD_TRG, DIM_LBUTTON))
    {
        HandleLeftClick();
    }

    HandleUndoRedo();
}

void Controller::HandleLeftClick()
{
    Ray ray = MouseRay::Create();

    Axis a = trs_->RayHitTest(ray);
    if (a != Axis::None)
    {
        undo_manager_->Push();
        trs_->SetDraggingAxis(a);
        return;
    }

    MeshCollider::CollInfo hit;
    int index = stage_data_->RayHitTest(ray, &hit);
    if (index >= 0)
    {
        is_catch = true;
        stage_data_->SetModel(index);
    }
    else
    {
        is_catch = false;
    }
}

void Controller::HandleUndoRedo()
{
    if (input_->CheckKey(KD_DAT, DIK_LCONTROL))
    {
        if (input_->CheckKey(KD_TRG, DIK_Z)) undo_manager_->Undo();
        if (input_->CheckKey(KD_TRG, DIK_Y)) undo_manager_->Redo();
    }
}

void Controller::TRSControl() const
{
    if (input_->CheckKey(KD_TRG, DIK_W))
    {
        trs_->SetState(TRS::State::kTranslation);
    }

    if (input_->CheckKey(KD_TRG, DIK_E))
    {
        trs_->SetState(TRS::State::kRotation);
    }

    if (input_->CheckKey(KD_TRG, DIK_R))
    {
        trs_->SetState(TRS::State::kScaling);
    }

    if (input_->CheckKey(KD_TRG, DIK_Q))
    {
        trs_->SetState(TRS::State::kNone);
    }
}

void Controller::CameraControl() const
{
    //回転
    if (input_->IsMouseMove())
    {
        camera_->Rotate();
    }
    //移動
    if (input_->IsMoveInput())
    {
        camera_->Move();
    }
}

void Controller::Draw()
{
    Transform* t = stage_data_ ? stage_data_->GetSelectedTransform() : nullptr;
    if (not t) return;
    if (not is_catch)return;
    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2(300, 160), ImGuiCond_Once);
    ImGui::Begin("Transform");

    ImGui::Separator();
    ImGui::Text("Position: %.2f, %.2f, %.2f", t->position.x,
                t->position.y, t->position.z);
    if (ImGui::DragFloat3("Position", &t->position.x, 0.1f))
    {
    }
    if (ImGui::IsItemActivated())
    {
        undo_manager_->Push();
    }
    ImGui::Separator();
    VECTOR3& tmp_r = t->rotation;
    ImGui::Text("Rotation: %.2f, %.2f, %.2f", tmp_r.x,
                tmp_r.y, tmp_r.z);
    if (ImGui::DragFloat3("Rotation", &tmp_r.x, 0.1f))
    {
    }
    if (ImGui::IsItemActivated())
    {
        undo_manager_->Push();
    }
    ImGui::Separator();
    ImGui::Text("Scale:    %.2f, %.2f, %.2f", t->scale.x,
                t->scale.y, t->scale.z);
    if (ImGui::DragFloat3("Scale", &t->scale.x, 0.1f))
    {
    }
    if (ImGui::IsItemActivated())
    {
        undo_manager_->Push();
    }
    ImGui::Separator();

    ImGui::End();
}
