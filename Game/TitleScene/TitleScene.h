/*
 *	@File	TitleScene.h
 *	@Brief	タイトルシーン。
 *	@Date	2023-03-31
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef TITLESCENE
#define	TITLESCENE

#include "../IScene.h"

class TitleUI;
class TitleScene final : public IScene
{
private:
	// カメラアングル
	const float CAMERA_ANGLE = 45.0f;

	// タイトルロゴモデル
	std::unique_ptr<DirectX::Model> m_titleLogoModel;
	// タイトルステージモデル
	std::unique_ptr<DirectX::Model> m_miniatureModel;
	// スカイドーム
	std::unique_ptr<DirectX::Model> m_skyDomeModel;

	// カメラ演出
	float m_cameraMoveY;
	const float MAX_HEIGHT = 10.0f;
	float m_logoMoveScale;

	// 選択変更時の加速
	bool is_accelerateFlag;
	const float MAX_ACCELERATE_TIME = 6.0f;
	float m_accelerate;

	// ロゴの開始時の演出
	float m_logoMoveY;
	const float END_MOVE_POS = 1.0f;
	const float LOGO_CHANGE_SCALE = 1.05f;

private:

	// UI
	std::unique_ptr<TitleUI> m_titleUI;

	// 決定判定
	bool is_startFlag;

	// 選択フラグ
	bool is_menuFlag;

public:

	TitleScene();
	~TitleScene();

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="引数無し"></param>
	/// <returns>なし</returns>
	void Initialize() override;

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="引数無し"></param>
	/// <returns>なし</returns>
	void Update() override;

	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name="引数無し"></param>
	/// <returns>なし</returns>
	void Draw() override;

	/// <summary>
	/// 終了処理
	/// </summary>
	/// <param name="引数無し"></param>
	/// <returns>なし</returns>
	void Finalize() override;

	/// <summary>
	/// 画面依存、デバイス依存の初期化
	/// </summary>
	/// <param name="引数無し"></param>
	/// <returns>なし</returns>
	void CreateWindowDependentResources() override;

private:

	/// <summary>
	/// シーン内の変数初期化関数
	/// </summary>
	/// <param name="引数無し"></param>
	/// <returns>なし</returns>
	void SetSceneValues() override;

private:

	/// <summary>
	/// スタートを押したときの演出
	/// </summary>
	/// <param name="引数無し"></param>
	/// <returns>演出完了でTrueを返す</returns>
	bool FlyStartObjects();
};

#endif // TITLESCENE