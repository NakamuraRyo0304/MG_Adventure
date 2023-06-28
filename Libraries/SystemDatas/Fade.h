/*
 *	@File	Fade.h
 *	@Brief	画面遷移時のフェード処理。
 *	@Date	2023-06-27
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef FADE
#define FADE

class Fade
{
private:
	// フェードインを開始するフラグ
	bool is_fadeInFlag;

	// フェードアウトを開始するフラグ
	bool is_fadeOutFlag;

	// フェード終了判定フラグ
	bool is_endFlag;

	// フェードの数値0-255
	float m_fadeNum;

	// 最大値/最小値
	const float MAX_NUM = 255.0f;
	const float MIN_NUM = 0.0f;

public:
	Fade();
	~Fade();

	// 更新処理
	void Update(float speed);

	// フェードインを開始する関数
	void SetFadeIn();

	// フェードアウトを開始する関数
	void SetFadeOut();

	// フェード終了を判定する関数
	const bool& GetEndFlag() { return is_endFlag; }
};

#endif // FADE