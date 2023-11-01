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
	/// モデルを作成し、返却する(std::unique_ptr<Model>で格納可)
	/// </summary>
	/// <param name="device">ID3D11Device1ポインタ</param>
	/// <param name="filename">ファイルパス　拡張子は「.cmo」</param>
	/// <returns>作成したモデルデータ</returns>
	static std::unique_ptr<DirectX::DX11::Model> GetCreateModel(ID3D11Device1* device, const wchar_t* filename);

	/// <summary>
	/// モデルの解放
	/// </summary>
	/// <param name="model">解放したいモデルデータ</param>
	/// <returns>なし</returns>
	static void DeleteModel(std::unique_ptr<DirectX::Model>& model);
};

#endif // MODELFACTORY