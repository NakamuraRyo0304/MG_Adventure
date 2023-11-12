/*
 *	@File	ResultCamera.h
 *	@Brief	リザルトシーンのカメラクラス。
 *	@Date	2023-11-12
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef RESULTCAMERA
#define RESULTCAMERA

#include "../../CommonObjects/IGameCamera.h"

class ResultCamera : public IGameCamera
{
private:

	// タイマー
	float m_timer;
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="screenSize">スクリーンサイズ</param>
	/// <returns>なし</returns>
	ResultCamera(const DirectX::SimpleMath::Vector2& screenSize);
	~ResultCamera();

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="引数無し"></param>
	/// <returns>なし</returns>
	void Update() override;
};

#endif // RESULTCAMERA