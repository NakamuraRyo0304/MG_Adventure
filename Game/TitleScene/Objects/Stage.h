/*
 *	@File	Stage.h
 *	@Brief	タイトルのステージ。
 *	@Date	2023-11-03
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef STAGE
#define STAGE

#include "ITitleObject.h"

class Stage : public ITitleObject
{
private:

	std::unique_ptr<DirectX::Model> m_model;

	// 選択変更時の加速
	float m_accelerate;
	bool is_accelerateFlag;
	const float MAX_ACCELERATE_TIME = 6.0f;

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="factory">ファクトリマネージャ</param>
	/// <param name="path">タイトルロゴモデルのパス</param>
	/// <returns>なし</returns>
	Stage(std::shared_ptr<FactoryManager> factory, const wchar_t* path);
	~Stage();

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
public:

	// 加速させる
	void AddAccelerate() { is_accelerateFlag = true; }
};

#endif // STAGE