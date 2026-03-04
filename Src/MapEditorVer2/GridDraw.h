#pragma once
#include "../Object3D.h"

class GridDraw : public Object3D
{
private:
    bool is_grid_draw_ = false;

private:
    void Draw() override;
    static void Grid();

public:
    bool GetDrawFlag() const { return is_grid_draw_; }
};
