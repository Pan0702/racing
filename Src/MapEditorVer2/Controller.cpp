#include "Controller.h"

Controller::Controller()
{
    camera_ = ObjectManager::FindGameObject<Camera>();
}

void Controller::Update()
{
    CameraControl();
    //つかむ処理
    auto input = GameDevice()->m_pDI;
    if (input == nullptr) return;

    if (input->CheckMouse(KD_TRG, DIM_LBUTTON))
    {
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
