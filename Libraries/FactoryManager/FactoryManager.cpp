/*
 *	@File	FactoryManager.cpp
 *	@Brief	ファクトリーの管理クラス。
 *	@Date	2023-11-04
 *  @Author NakamuraRyo
 */

#include "pch.h"
#include "FactoryManager.h"

// コンストラクタ
FactoryManager::FactoryManager()
{
}

// デストラクタ
FactoryManager::~FactoryManager()
{
	m_modelFactory.reset();
	m_shaderFactory.reset();
}

// 初期作成関数
void FactoryManager::CreateFactory()
{
	m_modelFactory = std::make_unique<ModelFactory>();
	m_shaderFactory = std::make_unique<ShaderFactory>();
}

// モデルファクトリを建設
void FactoryManager::BuildModelFactory()
{
	m_modelFactory = std::make_unique<ModelFactory>();
}

// モデルファクトリを訪問
const std::unique_ptr<ModelFactory>& FactoryManager::VisitModelFactory()
{
	if (not m_modelFactory)
	{
		m_modelFactory = std::make_unique<ModelFactory>();
	}
	return m_modelFactory;
}

// モデルファクトリを退出
void FactoryManager::LeaveModelFactory()
{
	if (not m_modelFactory) return;

	m_modelFactory.reset();
}

// シェーダーファクトリを建設
void FactoryManager::BuildShaderFactory()
{
	m_shaderFactory = std::make_unique<ShaderFactory>();
}

// シェーダーファクトリを訪問
const std::unique_ptr<ShaderFactory>& FactoryManager::VisitShaderFactory()
{
	if (not m_shaderFactory)
	{
		m_shaderFactory = std::make_unique<ShaderFactory>();
	}
	return m_shaderFactory;
}

// シェーダーファクトリを退出
void FactoryManager::LeaveShaderFactory()
{
	if (not m_shaderFactory) return;

	m_shaderFactory.reset();
}
