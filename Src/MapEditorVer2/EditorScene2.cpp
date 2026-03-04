#include "EditorScene2.h"

#include "Buttom.h"
#include "Controller.h"
#include "GridDraw.h"
#include "../MapTips/Curve.h"
#include "../MapTips/Plane.h"
#include "TRSObject/TRS.h"
#include "StageData.h"

EditorScene2::EditorScene2()
{
    new Camera();
    //new Curve();
    //new Plane();
    new StageData();
    new Button();
    new TRS();
    new Controller();
}
