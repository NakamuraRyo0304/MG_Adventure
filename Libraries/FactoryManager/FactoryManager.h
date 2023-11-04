/*
 *	@File	FactoryManager.h
 *	@Brief	ファクトリーの管理クラス。
 *	@Date	2023-11-04
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef FACTORYMANAGER
#define FACTORYMANAGER

//-------------------------------------------------------------------------------------//
// ファクトリーのインクルード
#include "Libraries/Factories/ModelFactory.h"
#include "Libraries/Factories/ShaderFactory.h"

//-------------------------------------------------------------------------------------//

class FactoryManager
{
private:

	// モデルファクトリ
	std::unique_ptr<ModelFactory> m_modelFactory;

	// シェーダーファクトリ
	std::unique_ptr<ShaderFactory> m_shaderFactory;

public:
	FactoryManager();
	~FactoryManager();

	// 初期作成関数
	void CreateFactory();

	// モデルファクトリを建設
	void BuildModelFactory();
	// モデルファクトリを訪問
	const std::unique_ptr<ModelFactory>& VisitModelFactory();
	// モデルファクトリを退出
	void LeaveModelFactory();
	// シェーダーファクトリを建設
	void BuildShaderFactory();
	// シェーダーファクトリを訪問
	const std::unique_ptr<ShaderFactory>& VisitShaderFactory();
	// シェーダーファクトリを退出
	void LeaveShaderFactory();

};

#endif // FACTORYMANAGER