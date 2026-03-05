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
    /// <summary>
    /// 指定パスのメッシュをロードし、ボタンリストとモデルストレージに登録する
    /// </summary>
    /// <param name="path">ロードするメッシュファイルのパス</param>
    void CreateModel(const std::string& path);

};
