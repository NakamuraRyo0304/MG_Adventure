/*
 *	@File	SelectCamera.h
 *	@Brief	セレクトシーンのカメラクラス。
 *	@Date	2023-11-11
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef SELECTCAMERA
#define SELECTCAMERA

#include "../../CommonObjects/IGameCamera.h"

class SelectCamera : public IGameCamera
{
private:

	// 切り替え可能か判定する
	bool is_canChangeFlag;

private:

	// カメラのY座標
	const float POS_Y = 30.0f;

	// カメラの回転半径
	const float CAMERA_RADIUS = 4.0f;

	// 接近速度
	const float APPROACH_SPEED = 0.085f;

	// 上下割合
	const float VERTICAL_MOTION = 1.5f;

	// 視点の動作速度
	const float LOOK_VALUE = 60.0f;
	const float LOOK_SPAN = 0.25f;
	const float LOOK_SPEED = 0.75f;

public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="screenSize">スクリーンサイズ</param>
	/// <returns>なし</returns>
	SelectCamera(const DirectX::SimpleMath::Vector2& screenSize);
	~SelectCamera();

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="引数無し"></param>
	/// <returns>なし</returns>
	void Update() override;
	/// <summary>
	/// カメラのターゲットの更新
	/// </summary>
	/// <param name="引数無し"></param>
	/// <returns>なし</returns>
	void MoveTarget();
	// 切り替え可能か判定を返す
	const bool& IsCanChange(){ return is_canChangeFlag;}

};

#endif // SELECTCAMERA