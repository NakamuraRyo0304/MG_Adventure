/*
 *	@File	ModelFactory.h
 *	@Brief	モデルを作成するファクトリークラス。
 *	@Date	2023-06-03
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef MODELFACTORY
#define MODELFACTORY

class ModelFactory
{
public:
	ModelFactory();
	~ModelFactory();

	/// <summary>
	/// モデルを作成し、返却する
	/// </summary>
	/// <param name="filename">ファイルパス　拡張子は「.cmo」</param>
	/// <returns>モデルデータ</returns>
	std::unique_ptr<DirectX::Model> GetCreateModel(const wchar_t* filename);

	/// <summary>
	/// モデルの解放
	/// </summary>
	/// <param name="model">解放したいモデルデータ</param>
	/// <returns>なし</returns>
	void DeleteModel(std::unique_ptr<DirectX::Model>& model);
};

#endif // MODELFACTORY