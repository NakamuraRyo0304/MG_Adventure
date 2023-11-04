/*
 *	@File	TitleSky.h
 *	@Brief	タイトルの空の描画。
 *	@Date	2023-11-03
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef TITLESKY
#define TITLESKY

#include "ITitleObject.h"

class TitleSky : public ITitleObject
{
private:

	std::unique_ptr<DirectX::Model> m_model;

	// スタートフラグ
	bool is_startFlag;

	// Y座標
	float m_posY;

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="factory">ファクトリマネージャ</param>
	/// <param name="path">タイトルロゴモデルのパス</param>
	/// <returns>なし</returns>
	TitleSky(std::shared_ptr<FactoryManager> factory, const wchar_t* path);
	~TitleSky();

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="引数無し"></param>
	/// <returns>なし</returns>
	void Update() override;

	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name="states">コモンステート</param>
	/// <param name="view">ビュー行列</param>
	/// <param name="proj">射影行列</param>
	/// <returns>なし</returns>
	void Render(DirectX::CommonStates& states,
		const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) override;

	/// <summary>
	/// 終了処理
	/// </summary>
	/// <param name="引数無し"></param>
	/// <returns>なし</returns>
	void Finalize() override;

	// スタートフラグを設定
	void SetStartFlag(const bool& flag) { is_startFlag = flag; }

	// Y座標を設定
	void SetPositionY(const float& y) { m_posY = y; }
};

#endif // TITLESKY