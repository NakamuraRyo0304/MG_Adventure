/*
 *	@File	IParts.cpp
 *	@Brief	パーツの基底クラス。
 *	@Date	2023-01-18
 *  @Author NakamuraRyo
 */

#include "pch.h"
#include "IParts.h"

// コンストラクタ
IParts::IParts(std::unique_ptr<DirectX::Model> model)
	: m_matrix{}		// マトリクス
	, m_parentMatrix{}  // 親マトリクス
{
	m_model = std::move(model);
}