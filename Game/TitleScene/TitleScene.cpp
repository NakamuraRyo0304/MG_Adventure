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
	m_timer{0.0f},
	m_titleLogoModel{},
	m_miniatureModel{}
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
	// 描画関連
	auto context = GetSystemManager()->GetDeviceResources()->GetD3DDeviceContext();
	auto& states = *GetSystemManager()->GetCommonStates();

	// カメラ用行列
	SimpleMath::Matrix logoMat, stageMat, skyMat, view, proj;

	// 移動、回転行列
	SimpleMath::Matrix logoTrans, logoRot;
	SimpleMath::Matrix stageTrans, stageRotX,stageRotY;
	SimpleMath::Matrix skyRotY;

	// ワールド行列
	logoMat = SimpleMath::Matrix::Identity;
	stageMat = SimpleMath::Matrix::Identity;
	skyMat = SimpleMath::Matrix::Identity;

	// 回転行列
	logoRot = SimpleMath::Matrix::CreateRotationX(sinf(m_timer) * 0.5f);
	stageRotX = SimpleMath::Matrix::CreateRotationX(0.3f);
	stageRotY = SimpleMath::Matrix::CreateRotationY(m_timer) * 0.5f;
	skyRotY = SimpleMath::Matrix::CreateRotationX(m_timer) * 0.2f;

	// 移動行列
	logoTrans = SimpleMath::Matrix::CreateTranslation(0.0f, 2.0f, cosf(m_timer) * 0.5f);
	stageTrans = SimpleMath::Matrix::CreateTranslation(0.0f, -1.0f, -10.0f);

	// ロゴ
	logoMat *= logoRot * logoTrans;
	// ステージ
	stageMat *= stageRotY * stageRotX * stageTrans;
	// スカイドーム
	skyMat *= skyRotY;

	// ビュー行列
	SimpleMath::Vector3    eye(0.0f, 0.1f, 8.0f);
	SimpleMath::Vector3     up(0.0f, 1.0f, 0.0f);
	SimpleMath::Vector3 target(0.0f, 0.0f, 0.0f);
	view = SimpleMath::Matrix::CreateLookAt(eye, target, up);

	// プロジェクション行列
	proj = GetSystemManager()->GetCamera()->GetProjection();

	// モデル描画
	m_miniatureModel->Draw(context, states, stageMat, view, proj);	// ステージ
	m_titleLogoModel->Draw(context, states, logoMat, view, proj);	// ロゴ
	m_skydomeModel->Draw(context, states, skyMat, view, proj);  	// スカイドーム
}

//--------------------------------------------------------//
//終了処理                                                //
//--------------------------------------------------------//
void TitleScene::Finalize()
{
	// モデルの解放
	ModelFactory::DeleteModel(m_titleLogoModel);
	ModelFactory::DeleteModel(m_miniatureModel);
	ModelFactory::DeleteModel(m_skydomeModel);
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
	m_titleLogoModel = ModelFactory::GetModel(device, L"Resources/Models/TitleLogo.cmo");
	m_miniatureModel = ModelFactory::GetModel(device, L"Resources/Models/TitleStage.cmo");
	m_skydomeModel   = ModelFactory::GetModel(device, L"Resources/Models/ShineSky.cmo");
}