#pragma once
/// </summary>   
///SceneManagerから呼び出されてSceneを作成するクラス    
/// SceneManagerを汚染しないように、作成処理を分けて書いてある                                           
/// 新シーンは、ここで指定してもらうようにする       
/// </summary>   
#include <memory>
#include <string>

class SceneBase;

class SceneFactory {
public:
	/// <summary>
	/// 最初にシーンを作る
	/// </summary>
	/// <returns>作成したインスタンス</returns>
	static std::unique_ptr<SceneBase> CreateFirst();

	/// <summary>
	/// 指定されたシーンを作成する
	/// </summary>
	/// <param name="name">シーンの名称</param>
	/// <returns>作成したインスタンス</returns>
	static std::unique_ptr<SceneBase> Create(const std::string& name);
};
