#include "EditorScene2.h"

#include "Buttom.h"
#include "Controller.h"
#include "../MapTips/Curve.h"
#include "TRSObject/TRS.h"

EditorScene2::EditorScene2()
{

    new Camera();
    new Curve();
    new Button();
    new TRS();
    new Controller();
}
