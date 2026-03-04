#include "GridDraw.h"

void GridDraw::Draw()
{

    if (is_grid_draw_)
    {
        Grid();
    }
}

void GridDraw::Grid()
{
    CSprite spr;
    //z軸
    constexpr float gridSize = 500.0f;
    for (int i = -gridSize; i <= gridSize; i++)
    {
        if (i % 10 == 0)
        {
            spr.DrawLine3D(VECTOR3(gridSize, 0, i), VECTOR3(-gridSize, 0, i),RGB(255, 255, 255));
        }
        else
        {   
            spr.DrawLine3D(VECTOR3(gridSize, 0, i), VECTOR3(-gridSize, 0, i),RGB(255, 255, 255), 0.5f);
        }
    }
    //x軸
    for (int i = -gridSize; i <= gridSize; i++)
    {
        if (i % 10 == 0)
        {
            spr.DrawLine3D(VECTOR3(i, 0, gridSize), VECTOR3(i, 0, -gridSize),RGB(255, 255, 255));
        }
        else
        {
            spr.DrawLine3D(VECTOR3(i, 0, gridSize), VECTOR3(i, 0, -gridSize),RGB(255, 255, 255), 0.5f);
        }
    }
}
