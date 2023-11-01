/*
 *	@File	Fade.h
 *	@Brief	画面遷移時のフェード処理。
 *	@Date	2023-06-27
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef FADE
#define FADE

class Transition;
class Fade
{
private:
	// トランジション
	std::unique_ptr<Transition> m_transition;

private:

	// フェードインを開始するフラグ
	bool is_fadeInFlag;

	// フェードアウトを開始するフラグ
	bool is_fadeOutFlag;

	// フェード速度
	float m_fadeSpeed;

	// フェード終了判定フラグ
	bool is_endFlag;

	// フェードの数値0-1
	float m_fadeNum;

private:

	// 最大値/最小値
	const float MAX_NUM = 1.f;
	const float MIN_NUM = 0.f;
	const float CLAMP_NUM = 255.0f;

public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="speed">フェードの速度</param>
	/// <returns>なし</returns>
	Fade(float speed);
	~Fade();

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="引数無し"></param>
	/// <returns>なし</returns>
	void Initialize();

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="引数無し"></param>
	/// <returns>なし</returns>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name="引数無し"></param>
	/// <returns>なし</returns>
	void Draw();

	/// <summary>
	/// フェードインを開始する関数
	/// </summary>
	/// <param name="引数無し"></param>
	/// <returns>なし</returns>
	void SetFadeIn();

	/// <summary>
	/// フェードアウトを開始する関数
	/// </summary>
	/// <param name="引数無し"></param>
	/// <returns>なし</returns>
	void SetFadeOut();

	/// <summary>
	/// フェードアウトの値を返す
	/// </summary>
	/// <param name="引数無し"></param>
	/// <returns>フェードアウトの値</returns>
	const float& GetMaxValue() { return MAX_NUM; }

	/// <summary>
	/// フェード終了判定
	/// </summary>
	/// <param name="引数無し"></param>
	/// <returns>フェード終了でTrue</returns>
	const bool& GetEndFlag() { return is_endFlag; }

	/// <summary>
	/// フェードの数値の取得関数
	/// </summary>
	/// <param name="引数無し"></param>
	/// <returns>フェードの値</returns>
	const float& GetFadeNum() { return m_fadeNum; }

	/// <summary>
	/// フェード速度セッター
	/// </summary>
	/// <param name="speed">フェード速度</param>
	/// <returns>なし</returns>
	void SetFadeSpeed(float speed) { m_fadeSpeed = speed; }

private:

	/// <summary>
	/// フェード処理を行う前に呼び出す関数
	/// </summary>
	/// <param name="引数無し"></param>
	/// <returns>なし</returns>
	void Reset();
};

#endif // FADE