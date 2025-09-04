#pragma once
#include "Object3D.h"

class EditorCamera:public Object3D
{
public:
    EditorCamera();
    ~EditorCamera();
    void Update()override;
    void ViewMatrixUpdate(); 
    float MouseMove(const int& oldCoordinates, const int& nowCoordinates);

private:
    VECTOR3 m_comeraPos;
    int m_prevMouseX;
    int m_prevMouseY;
    bool m_isMiddleButtonPressed;

};
