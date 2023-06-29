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
	// スプライトバッチへのポインタ
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;

	// 画像情報
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_SRV;

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

	// 初期化処理
	void Initialize(ID3D11DeviceContext1* context,ID3D11Device1* device);

	// 更新処理
	void Update(float speed);

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

};

#endif // FADE