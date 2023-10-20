/*
 *	@File	Haze.cpp
 *	@Brief	画面の周りのもやもや。
 *	@Date	2023-10-19
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include "Haze.h"

 /// <summary>
 /// コンストラクタ
 /// </summary>
 /// <param name="引数無し"></param>
 /// <returns>なし</returns>
Haze::Haze()
	: m_pDR{ nullptr }		// デバイスリソース
	, m_timer{ 0.0f }		// タイマー
{
}

/// <summary>
/// デストラクタ
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
Haze::~Haze()
{
}

/// <summary>
/// シェーダーの作成
/// </summary>
/// <param name="pDR">デバイスリソースのポインタ</param>
/// <returns>なし</returns>
void Haze::CreateShader(DX::DeviceResources* pDR)
{
	m_pDR = pDR;

	auto _context = m_pDR->GetD3DDeviceContext();
	auto _device = m_pDR->GetD3DDevice();



}

