/*
 *	@File	PlaySky.h
 *	@Brief	プレイシーンのスカイドーム。
 *	@Date	2023-11-09
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef PLAYSKY
#define PLAYSKY

class FactoryManager;
class PlaySky
{
private:

	// モデル
	std::unique_ptr<DirectX::Model> m_model;

	// スカイドームの回転速度
	const float SKY_ROT_SPEED = 0.02f;

	// 空の色遷移
	struct Color
	{
		float red = 0.0f;
		float green = 0.0f;
		float blue = 0.0f;
		Color(float R = 1.0f, float G =  1.0f, float B = 1.0f)
		{
			this->red = R;
			this->green = G;
			this->blue = B;
		}
	};
	Color m_skyColor;

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="factory">ファクトリマネージャ</param>
	/// <param name="path">プレイのスカイパス</param>
	/// <returns>なし</returns>
	PlaySky(std::shared_ptr<FactoryManager> fm, const wchar_t* path);
	~PlaySky();

	/// <summary>
	/// 空の更新
	/// </summary>
	/// <param name="gameTimer">ゲーム内時間</param>
	/// <returns>なし</returns>
	void Update(const float& gameTimer);

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

#endif // PLAYSKY