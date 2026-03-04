#include "ExportData.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include "StageData.h"

using json = nlohmann::json;

json ExportData::TransformToJson(const std::string& model_name, const Transform& transform)
{
    json j;
    const char* p = ObjectManager::FindGameObject<CModelStorage>()->GetModelPath(model_name);
    j["path"] = p;
    j["model_name"] = model_name;
    
    // Transformの内容を詰める
    j["transform"]["position"] = { {"x", transform.position.x}, {"y", transform.position.y}, {"z", transform.position.z} };
    j["transform"]["rotation"] = { {"x", transform.rotation.x}, {"y", transform.rotation.y}, {"z", transform.rotation.z} };
    j["transform"]["scale"]    = { {"x", transform.scale.x},    {"y", transform.scale.y},    {"z", transform.scale.z} };
    
    return j;
}

void ExportData::ExportAllModels(const std::string& filename,
                                 const std::vector<StageDataInfo>& modelList)
{
    json root = json::array(); // 全体を配列として定義
    
    for (const auto& item : modelList) {
        root.push_back(TransformToJson(item.model_name, item.transform));
    }
    std::string path = "data/" + filename + ".json";
    std::ofstream file(path);
    if (file.is_open()) {
        file << root.dump(4); // インデント付きで見やすく保存
    }
    MessageBox(0, _T("Export Success"), nullptr, MB_OK);
}
