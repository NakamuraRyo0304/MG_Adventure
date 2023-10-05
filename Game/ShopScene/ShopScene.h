/*
 *	@File	ShopScene.h
 *	@Brief	スキンショップのシーン。
 *	@Date	2023-10-04
 *  @Author NakamuraRyo
 */

#ifndef SHOPSCENE
#define SHOPSCENE

#include "../IScene.h"

class ShopScene final : public IScene
{
private:

	// 合計コイン数
	int m_allCoins;

private:

	// カメラアングル
	const float CAMERA_ANGLE = 44.5f;

public:
	// コンストラクタ
	ShopScene();

	// デストラクタ
	~ShopScene();

	// 初期化
	void Initialize() override;

	// 更新
	void Update(const float& elapsedTime, DirectX::Keyboard::State& keyState, DirectX::Mouse::State& mouseState) override;

	// 描画
	void Draw() override;

	// 終了処理
	void Finalize() override;

	// 画面依存の初期化
	void CreateWindowDependentResources() override;

private:

	// シーン内の変数初期化関数
	void SetSceneValues() override;

public:

	// コインアクセサ
	const int& GetAllCoins() { return m_allCoins; }
	void SetAllCoins(const int& allCoins) { m_allCoins = allCoins; }


};

#endif // SHOPSCENE