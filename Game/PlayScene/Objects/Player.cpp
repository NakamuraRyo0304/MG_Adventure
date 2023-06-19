/*
 *	@File	Player.cpp
 *	@Brief	プレイヤの処理をするクラス。
 *	@Date	2023-06-14
 *  @Author NakamuraRyo
 */

#include "pch.h"

// システム
#include "../../../Libraries/SystemManager/SystemManager.h"

// インライン関数群
#include "../../../Libraries/UserUtility.h"

#include "Player.h"

//--------------------------------------------------------//
//コンストラクタ                                          //
//--------------------------------------------------------//
Player::Player(std::unique_ptr<Model> model):
	m_model{std::move(model)},
	m_parameter{},
	m_system{nullptr}
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
void Player::Initialize(std::shared_ptr<SystemManager> system)
{
	// システムの取得
	m_system = system;

	// パラメータのリセット
	m_parameter.reset();

	// 加速度の設定
	m_parameter.accelerate = 0.05f;
}

//--------------------------------------------------------//
//更新処理                                                //
//--------------------------------------------------------//
void Player::Update(Keyboard::State& keyState, float timer)
{
	m_timer = timer;

	// 移動処理
	if (keyState.W)
	{
		m_parameter.velocity.z -= m_parameter.accelerate;
	}
	if (keyState.S)
	{
		m_parameter.velocity.z += m_parameter.accelerate;
	}
	if (keyState.A)
	{
		m_parameter.velocity.x -= m_parameter.accelerate;
	}
	if (keyState.D)
	{
		m_parameter.velocity.x += m_parameter.accelerate;
	}

	// 移動量の計算
	m_parameter.position += m_parameter.velocity;

	// 減速処理
	if (m_parameter.velocity != SimpleMath::Vector3::Zero)
	{
		m_parameter.velocity *= DECELERATION;
	}
	m_parameter.gravity += 0.015f;

	m_parameter.position.y -= m_parameter.gravity;

	// デバッグ用
	if (m_parameter.position.y < -7.0f) m_parameter.position.y = 10.0f;
}

//--------------------------------------------------------//
//描画処理                                                //
//--------------------------------------------------------//
void Player::Render(ID3D11DeviceContext* context, DX11::CommonStates& states,
	SimpleMath::Matrix view, SimpleMath::Matrix proj)
{
	// ワールド行列
	SimpleMath::Matrix world =
		SimpleMath::Matrix::CreateTranslation(
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
void Player::Spawn(SimpleMath::Vector3 spawnPosition)
{
	// パラメータの初期化
	m_parameter.reset();

	// 座標の設定
	m_parameter.position = spawnPosition;
}
