/*
 *	@File	PlayUI.h
 *	@Brief	プレイシーンのUI表示。
 *	@Date	2023-07-15
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef PLAYUI
#define PLAYUI

class SystemManager;
class PlayUI
{
private:
	// シェアポインタ
	std::shared_ptr<SystemManager> m_systemManager;

	// ウィンドウサイズ
	DirectX::SimpleMath::Vector2 m_windowSize;

	// フルスクリーンサイズ
	const DirectX::SimpleMath::Vector2 FULL_SCREEN_SIZE = { 1920.0f,1080.0f };

	// タイマーの座標
	DirectX::SimpleMath::Vector2 m_timerPos;


public:
	PlayUI(const DirectX::SimpleMath::Vector2& windowSize);
	~PlayUI();

	// 作成処理
	void Create(std::shared_ptr<SystemManager> system ,ID3D11DeviceContext1* context, ID3D11Device1* device);

	// 更新処理
	void Update(const float& timelimit);

	// 描画処理
	void Render();

	// 終了処理
	void Finalize();
};

#endif //PLAYUI
