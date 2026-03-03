#pragma once
#include <vector>

#include "../FbxMesh.h"

class ModelCreator
{
private:
    struct ModelInfo
    {
        std::string name;
        CFbxMesh* mesh;
    };
    std::vector<ModelInfo> models_;
public:
    void CreateModel(const std::string& path);
    
};
