#include "ModelCreator.h"
#include <string>

#include "Buttom.h"
#include "../ObjectManager.h"

void ModelCreator::CreateModel(const std::string& path)
{
    size_t last_slash = path.find_last_of("\\/");
    size_t last_dot = path.find_last_of(".");

    size_t start = (last_slash == std::string::npos) ? 0 : last_slash + 1;
    std::string name;
    if (last_dot != std::string::npos && last_dot > start)
    {
        name = path.substr(start, last_dot - start);
    }
    else
    {
        name = path.substr(start);
    }

    for (auto& model : models_)
    {
        if (model.name == name)
        {
            MessageBox(0, _T("Already Exis"), nullptr, MB_OK);
            return;
        }
    }
    ModelInfo info = {};
    info.name = name;
    info.mesh = new CFbxMesh();
    if (info.mesh->Load(path.c_str()))
    {
        models_.push_back(info);
        ObjectManager::FindGameObject<Button>()->AddButton(name, info.mesh);
    }
}
