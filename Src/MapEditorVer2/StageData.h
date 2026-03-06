#pragma once
#include "MouseRay.h"
#include "../ModelStorage.h"
#include "../Object3D.h"

struct StageDataInfo
{
    Transform transform;
    std::string model_name;
    StageDataInfo()
    {
        transform.position = VECTOR3(0, 0, 0);
        model_name = "";
    }
    StageDataInfo(const std::string& model_name_,const VECTOR3& pos_)
    {
        model_name = std::move(model_name_);
        transform.position = pos_;
    }
};

class StageData : public Object3D
{
private:

    std::vector<StageDataInfo> stage_data_;
    CModelStorage* model_storage_;
    int selected_model_ = -1;
private:
    void Draw () override;
public:
    StageData();
    ~StageData() = default;

    /// <summary>指定座標とモデル名でオブジェクトをステージに追加する</summary>
    /// <param name="pos">配置するワールド座標</param>
    /// <param name="model_name">使用するモデルの名前</param>
    void AddModel(const VECTOR3& pos, const std::string& model_name);
    
    /// <summary>Transform  全体を指定してオブジェクト をステージに追加する（インポート用）</summary>               
    /// <param name="model_name">使用する モデルの名前</param>        
    /// <param name="transform">配置するTransform（位置・回転・スケール）</param>  
    void AddModelWithTransform(const std::string& model_name, const Transform& transform);

    /// <summary>ステージデータをJSONファイルにエクスポートする</summary>
    /// <param name="filename">出力ファイル名（拡張子なし）</param>
    void Export(const std::string& filename) const;

    /// <summary>レイとステージ上の全オブジェクトのメッシュコライダーを判定し、最近接のインデックスを返す</summary>
    /// <param name="ray">判定に使用するレイ</param>
    /// <param name="collOut">衝突情報の出力先</param>
    /// <returns>当たったオブジェクトのインデックス。当たらなければ-1</returns>
    int RayHitTest(const Ray& ray, MeshCollider::CollInfo* collOut);

    /// <summary>現在選択中のオブジェクトをステージから削除する</summary>
    void DeleteModel();

    /// <summary>現在選択中のオブジェクトのTransformポインタを返す。未選択時はnullptr</summary>
    Transform* GetSelectedTransform();

    /// <summary>指定インデックスのオブジェクトのTransformを上書きする（Undo/Redo用）</summary>
    /// <param name="index">対象インデックス</param>
    /// <param name="transform">設定するTransform</param>
    void SetSelectedTransform(int index, const Transform& transform);

    /// <summary>選択中のオブジェクトインデックスを設定する</summary>
    /// <param name="index">選択するインデックス</param>
    void SetModel(int index);

    /// <summary>現在選択中のオブジェクトインデックスを返す</summary>
    int GetIndex() const;

    /// <summary>ステージに設置されてるすべてのオブジェクトを返す</summary>
    const std::vector<StageDataInfo>& GetStageDataInfo() const;
};

