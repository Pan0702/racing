#include "Controller.h"

#include "TRSObject/TRS.h"

Controller::Controller()
{
    camera_ = ObjectManager::FindGameObject<Camera>();
     trs_ = ObjectManager::FindGameObject<TRS>();
}

void Controller::Update()
{
    CameraControl();
    //つかむ処理
    if (true)
    {
        TRSControl();
    }else
    {
        trs_->SetState(TRS::State::kNone);
    }
}

void Controller::TRSControl() const
{
    auto input = GameDevice()->m_pDI;
    if (input == nullptr) return;

    if (input->CheckKey(KD_TRG, DIK_W))
    {
        trs_->SetState(TRS::State::kTranslation);
    }
    if (input->CheckKey(KD_TRG, DIK_E))
    {
        trs_->SetState(TRS::State::kRotation);
    }
    if (input->CheckKey(KD_TRG, DIK_R))
    {
        trs_->SetState(TRS::State::kScaling);
    }
}

void Controller::CameraControl() const
{
    auto input = GameDevice()->m_pDI;
    //nullちぇっく
    if (input == nullptr) return;

    //カメラの移動処理
    if (input->CheckMouse(KD_DAT, DIM_RBUTTON))
    {
        //回転
        if (input->IsMouseMove())
        {
            camera_->Rotate();
        }
        //移動
        if (input->IsMoveInput())
        {
            camera_->Move();
        }
    }

    //ズーム処理
    if (input->GetMouseWheel() != 0)
    {
        camera_->Zoom();
    }
}
