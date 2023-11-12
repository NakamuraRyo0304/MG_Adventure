/*
 *	@File	TitleCamera.h
 *	@Brief	タイトルシーンのカメラクラス。
 *	@Date	2023-11-11
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef TITLECAMERA
#define TITLECAMERA

#include "../../CommonObjects/IGameCamera.h"

class TitleCamera : public IGameCamera
{
private:

	// ロゴの座標
	DirectX::SimpleMath::Vector3 m_logoPosition;
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="screenSize">スクリーンサイズ</param>
	/// <returns>なし</returns>
	TitleCamera(const DirectX::SimpleMath::Vector2& screenSize);
	~TitleCamera();

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="引数無し"></param>
	/// <returns>なし</returns>
	void Update() override;

	// ロゴの座標の設定
	void SetLogoPosition(const DirectX::SimpleMath::Vector3& pos) { m_logoPosition = pos; }
};

#endif // TITLECAMERA