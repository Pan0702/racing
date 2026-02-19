#include "EditorScene2.h"

#include "Buttom.h"
#include "Controller.h"
#include "../MapTips/Curve.h"

EditorScene2::EditorScene2()
{
    new Controller();
    new Camera();
    new Curve();
    new Buttom();
}
