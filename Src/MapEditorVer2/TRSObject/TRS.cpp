#include "TRS.h"


TRS::TRS()
{
    translation_ = std::make_unique<CTranslation>();
    rotation_ = std::make_unique<CRotation>();
    scaling_ = std::make_unique<CScaling>();
    new TRSBase();
}

void TRS::Draw()
{
    switch (state_)
    {
    case kTranslation:
        translation_->Draw();
        break;
    case kRotation:
        rotation_->Draw();
        break;
    case kScaling:
        scaling_->Draw();
        break;
    default:
        break;
    }
}
