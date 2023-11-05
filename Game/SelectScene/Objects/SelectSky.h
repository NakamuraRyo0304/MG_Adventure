/*
 *	@File	SelectSky.h
 *	@Brief	セレクトシーンのスカイドーム。
 *	@Date	2023-11-06
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef SELECTSKY
#define SELECTSKY

class FactoryManager;
class SelectSky
{
private:

	// モデル
	std::unique_ptr<DirectX::Model> m_model;

	// 回転速度
	const float ROT_SPEED = 2.0f;

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="factory">ファクトリマネージャ</param>
	/// <param name="path">タイトルロゴモデルのパス</param>
	/// <returns>なし</returns>
	SelectSky(std::shared_ptr<FactoryManager> fm, const wchar_t* path);
	~SelectSky();

	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name="states">コモンステート</param>
	/// <param name="view">ビュー行列</param>
	/// <param name="proj">射影行列</param>
	/// <param name="timer">タイマー</param>
	/// <returns>なし</returns>
	void Draw(DirectX::CommonStates& states, const DirectX::SimpleMath::Matrix& view,
		const DirectX::SimpleMath::Matrix& proj, const float& timer);
};

#endif // SELECTSKY