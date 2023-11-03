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

class Logo;
class Stage;
class TitleSky;
class TitleUI;
class TitleScene final : public IScene
{
private:
	// カメラアングル
	const float CAMERA_ANGLE = 45.0f;

private:

	// UI
	std::unique_ptr<TitleUI> m_titleUI;

	// タイトルロゴ
	std::unique_ptr<Logo> m_logo;

	// ステージ
	std::unique_ptr<Stage> m_stage;

	// スカイドーム
	std::unique_ptr<TitleSky> m_titleSky;

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

};

#endif // TITLESCENE