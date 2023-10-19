/*
 *	@File	Haze.h
 *	@Brief	画面の周りのもやもや。
 *	@Date	2023-10-19
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef HAZE
#define HAZE

class Haze
{
private:

	// デバイスリソース
	DX::DeviceResources* m_pDR;

	// タイマー
	float m_timer;

public:

	Haze();
	~Haze();

	// シェーダーの作成
	void CreateShader(DX::DeviceResources* pDR);

	// タイマーのセット
	void SetTimer(const float& timer) { m_timer = timer; };
};

#endif // HAZE