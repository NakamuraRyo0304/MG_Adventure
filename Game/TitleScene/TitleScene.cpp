/*
 *	@File	TitleScene.cpp
 *	@Brief	タイトルシーン。
 *	@Date	2023-03-31
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include "TitleScene.h"

 //--------------------------------------------------------//
 //コンストラクタ                                          //
 //--------------------------------------------------------//
TitleScene::TitleScene():
	IScene(),
	m_titleLogo{},
	m_timer{0.0f}
{
}

//--------------------------------------------------------//
//デストラクタ                                            //
//--------------------------------------------------------//
TitleScene::~TitleScene()
{
	Finalize();
}

//--------------------------------------------------------//
//初期化処理                                              //
//--------------------------------------------------------//
void TitleScene::Initialize()
{
	// 画面依存の初期化
	CreateWindowDependentResources();

	// カメラ視点移動
	GetSystemManager()->GetCamera()->SetEagleMode(false);
	// カメラ座標設定
	GetSystemManager()->GetCamera()->SetEyePosition(SimpleMath::Vector3(0.0f, -20.0f, -20.0f));
}

//--------------------------------------------------------//
//更新処理                                                //
//--------------------------------------------------------//
// 第１引数：時間(60FPS = 1sec) / 第２引数：キーボードのポインタ / 第３引数：マウスのポインタ
void TitleScene::Update(const float& elapsedTime,Keyboard::State& keyState,
	Mouse::State& mouseState)
{
	m_timer = elapsedTime;

	// キー入力情報を取得する
	GetSystemManager()->GetStateTrack()->Update(keyState);

	// マウス情報を取得する
	GetSystemManager()->GetMouseTrack()->Update(mouseState);

	// カメラの更新
	GetSystemManager()->GetCamera()->Update();

	// Spaceキーでシーン切り替え
	if (GetSystemManager()->GetStateTrack()->IsKeyReleased(Keyboard::Space))
	{
		GoNextScene(SCENE::SELECT);
	}
}

//--------------------------------------------------------//
//描画処理                                                //
//--------------------------------------------------------//
void TitleScene::Draw()
{
	// デバッグフォント
	GetSystemManager()->GetString()->ChangeFontColor(Colors::Black);
	GetSystemManager()->GetString()->DrawFormatString(
		GetSystemManager()->GetCommonStates().get(), 
		{ 0,0 }, 
		L"TitleScene"
	);

	// 描画関連
	auto context = GetSystemManager()->GetDeviceResources()->GetD3DDeviceContext();
	auto& states = *GetSystemManager()->GetCommonStates();

	// カメラ用行列
	SimpleMath::Matrix world, view, proj;

	// 移動、回転行列
	SimpleMath::Matrix trans, rotate;

	// ワールド行列
	world = SimpleMath::Matrix::Identity;

	// 回転行列
	rotate = SimpleMath::Matrix::CreateRotationX(sinf(m_timer) * 0.5f);

	// 移動行列
	trans = SimpleMath::Matrix::CreateTranslation(0.0f, 2.0f, cosf(m_timer) * 0.5f);

	world *= rotate * trans;

	// ビュー行列
	SimpleMath::Vector3    eye(0.0f, 0.1f, 8.0f);
	SimpleMath::Vector3     up(0.0f, 1.0f, 0.0f);
	SimpleMath::Vector3 target(0.0f, 0.0f, 0.0f);
	view = SimpleMath::Matrix::CreateLookAt(eye, target, up);

	// プロジェクション行列
	proj = GetSystemManager()->GetCamera()->GetProjection();

	// モデル描画
	m_titleLogo->Draw(context, states, world, view, proj);
}

//--------------------------------------------------------//
//終了処理                                                //
//--------------------------------------------------------//
void TitleScene::Finalize()
{
	ModelFactory::DeleteModel(m_titleLogo);
}

//--------------------------------------------------------//
//画面依存の初期化                                        //
//--------------------------------------------------------//
void TitleScene::CreateWindowDependentResources()
{
	// デバイスとデバイスコンテキストの取得
	auto device  = GetSystemManager()->GetDeviceResources()->GetD3DDevice();
	auto context = GetSystemManager()->GetDeviceResources()->GetD3DDeviceContext();

	// メイクユニーク
	GetSystemManager()->CreateUnique(device, context);
	GetSystemManager()->GetString()->CreateString(device, context);

	// 画面サイズの格納
	float width  = 
		static_cast<float>(GetSystemManager()->GetDeviceResources()->GetOutputSize().right);
	float height = 
		static_cast<float>(GetSystemManager()->GetDeviceResources()->GetOutputSize().bottom);

	// カメラの設定
	GetSystemManager()->GetCamera()->CreateProjection(width, height, 45.0f);

	// モデルの作成
	m_titleLogo = ModelFactory::GetModel(device, L"Resources/Models/TitleLogo.cmo");
}