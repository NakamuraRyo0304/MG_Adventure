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
	Fade(float speed);
	~Fade();

	// 初期化処理
	void Initialize(DX::DeviceResources* pDR);

	// 更新処理
	void Update();

	// 描画処理
	void Draw();

	// フェードインを開始する関数
	void SetFadeIn();

	// フェードアウトを開始する関数
	void SetFadeOut();

	// フェードアウトの値を返す
	const float& GetMaxValue() { return MAX_NUM; }

	// 値のリセットをかける関数
	void Reset();

	// フェード終了を判定する関数
	const bool& GetEndFlag() { return is_endFlag; }

	// フェードの数値の取得関数
	const float& GetFadeNum() { return m_fadeNum; }

	// フェード速度セッター
	void SetFadeSpeed(float speed) { m_fadeSpeed = speed; }

};

#endif // FADE