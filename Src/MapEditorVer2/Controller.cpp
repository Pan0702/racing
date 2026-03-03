#include "Controller.h"

#include "MouseRay.h"
#include "StageData.h"
#include "TRSObject/TRS.h"

Controller::Controller()
{
    camera_ = ObjectManager::FindGameObject<Camera>();
    trs_ = ObjectManager::FindGameObject<TRS>();
    input_ = GameDevice()->m_pDI;
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

    //ズーム処理
    if (input_->GetMouseWheel() != 0)
    {
        camera_->Zoom();
    }

    // クリックした瞬間
    if (input_->CheckMouse(KD_TRG, DIM_LBUTTON))
    {
        Ray ray = MouseRay::Create();

        // まずTRSギズモの矢印に当たったか確認
        Axis a = trs_->RayHitTest(ray);
        if (a != Axis::None)
        {
            trs_->SetDraggingAxis(a);
        }
        else
        {
            // TRSに当たっていなければオブジェクト選択
            StageData* data = ObjectManager::FindGameObject<StageData>();
            MeshCollider::CollInfo hit;
            int index = data->RayHitTest(ray, &hit);
            if (index >= 0)
            {
                is_catch = true;
                data->SetModel(index);
            }
        }
    }

    // ボタンを離したらドラッグ終了
    if (input_->CheckMouse(KD_UTRG, DIM_LBUTTON))
    {
        trs_->SetDraggingAxis(Axis::None);
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
    //nullちぇっく
    if (input_ == nullptr) return;


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
    
    if (not is_catch)return;
    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2(300, 160), ImGuiCond_Once);   
    ImGui::Begin("Transform");
    StageData* data = ObjectManager::FindGameObject<StageData>();
    Transform* t = data ? data->GetSelectedTransform() : nullptr;
    if (!t) return;
    ImGui::Separator();
    ImGui::Text("Position: %.2f, %.2f, %.2f", t->position.x,
                t->position.y, t->position.z);
    ImGui::DragFloat3("Position", &t->position.x, 0.1f);
    ImGui::Separator();
    ImGui::Text("Rotation: %.2f, %.2f, %.2f", t->rotation.x,
                t->rotation.y, t->rotation.z);
    ImGui::DragFloat3("Rotation", &t->rotation.x, 0.1f);
    ImGui::Separator();
    ImGui::Text("Scale:    %.2f, %.2f, %.2f", t->scale.x,
                t->scale.y, t->scale.z);
    ImGui::DragFloat3("Scale", &t->scale.x, 0.1f);
    ImGui::Separator();

    ImGui::End();
}
