#pragma once
#include <string> 
#include <vector>
#include <nlohmann/json.hpp>

#include "StageData.h"
class Transform;

class ExportData
{
public:
    bool Export(const std::string& name, const Transform& t);
    nlohmann::json TransformToJson(const std::string& model_name, const Transform& transform);
    void ExportAllModels(const std::string& filename, const std::vector<StageDataInfo>& modelList);
};
