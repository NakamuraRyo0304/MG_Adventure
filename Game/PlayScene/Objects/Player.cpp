/*
 *	@File	Player.cpp
 *	@Brief	プレイヤの処理をするクラス。
 *	@Date	2023-06-14
 *  @Author NakamuraRyo
 */

#include "pch.h"

#include "../../../Libraries/UserUtility.h"

#include "Player.h"

//--------------------------------------------------------//
//コンストラクタ                                          //
//--------------------------------------------------------//
Player::Player(std::unique_ptr<DirectX::Model> model):
	m_model{std::move(model)},
	m_parameter{}
{
}

//--------------------------------------------------------//
//デストラクタ                                            //
//--------------------------------------------------------//
Player::~Player()
{
	Finalize();
}

//--------------------------------------------------------//
//初期化処理                                              //
//--------------------------------------------------------//
void Player::Initialize()
{
	// パラメータのリセット
	m_parameter.reset();
}

//--------------------------------------------------------//
//更新処理                                                //
//--------------------------------------------------------//
void Player::Update(DirectX::Keyboard::State& keyState, float timer)
{
	m_timer = timer;

	// 移動処理
	if (keyState.W) m_parameter.position.z -= 0.05f;
	if (keyState.S) m_parameter.position.z += 0.05f;
	if (keyState.A) m_parameter.position.x -= 0.05f;
	if (keyState.D) m_parameter.position.x += 0.05f;

	m_parameter.gravity += 0.015f;

	m_parameter.position.y -= m_parameter.gravity;

	if (m_parameter.position.y < -7.0f) m_parameter.position.y = 10.0f;
}

//--------------------------------------------------------//
//描画処理                                                //
//--------------------------------------------------------//
void Player::Render(ID3D11DeviceContext* context, DirectX::DX11::CommonStates& states,
	DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	// ワールド行列
	DirectX::SimpleMath::Matrix world =
		DirectX::SimpleMath::Matrix::CreateTranslation(
			m_parameter.position.x,
			m_parameter.position.y + sinf(m_timer) / 10.0f + OFFSET_Y,
			m_parameter.position.z
		);

	// モデルの描画
	m_model->Draw(context, states, world, view, proj, false);
}

//--------------------------------------------------------//
//終了処理                                                //
//--------------------------------------------------------//
void Player::Finalize()
{
	m_model.reset();
	m_parameter.reset();
}

//--------------------------------------------------------//
//初期位置の設定とリセット                                //
//--------------------------------------------------------//
void Player::Spawn(DirectX::SimpleMath::Vector3 spawnPosition)
{
	// パラメータの初期化
	m_parameter.reset();

	// 座標の設定
	m_parameter.position = spawnPosition;
}
