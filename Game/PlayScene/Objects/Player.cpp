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
	m_system{},
	is_deathFlag{}
{
}

//--------------------------------------------------------//
//デストラクタ                                            //
//--------------------------------------------------------//
Player::~Player()
{
	Finalize();
	m_system.reset();
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
	m_parameter.accelerate = 0.01f;

	// 死亡判定の初期化
	is_deathFlag = false;
}

//--------------------------------------------------------//
//更新処理                                                //
//--------------------------------------------------------//
void Player::Update(Keyboard::State& keyState, float timer)
{
	m_timer = timer;

	// 重力処理
	UpdateGravity();

	// 移動処理
	if (keyState.A)
	{
		m_parameter.rotate *= SimpleMath::Quaternion::CreateFromAxisAngle(
			SimpleMath::Vector3::UnitY, ROT_SPEED);
	}
	if (keyState.D)
	{
		m_parameter.rotate *= SimpleMath::Quaternion::CreateFromAxisAngle(
			SimpleMath::Vector3::UnitY, -ROT_SPEED);
	}
	if (keyState.S)
	{
		SimpleMath::Vector3 vec(0.0f, 0.0f, m_parameter.accelerate);
		vec = SimpleMath::Vector3::Transform(vec, m_parameter.rotate);
		m_parameter.velocity += vec;
	}

	if (keyState.W)
	{
		SimpleMath::Vector3 vec(0.0f, 0.0f, m_parameter.accelerate);
		vec = SimpleMath::Vector3::Transform(vec, m_parameter.rotate);
		m_parameter.velocity -= vec;
	}
	// 移動量の計算
	m_parameter.position += m_parameter.velocity;

	// 減速処理
	if (m_parameter.velocity != SimpleMath::Vector3::Zero)
	{
		m_parameter.velocity *= DECELERATION;
	}
}

//--------------------------------------------------------//
//描画処理                                                //
//--------------------------------------------------------//
void Player::Render(ID3D11DeviceContext* context, DX11::CommonStates& states,
	SimpleMath::Matrix view, SimpleMath::Matrix proj)
{
	// ワールド行列
	SimpleMath::Matrix world;

	// 回転行列
	SimpleMath::Matrix rotate = SimpleMath::Matrix::CreateFromQuaternion(m_parameter.rotate);

	// 移動行列
	SimpleMath::Matrix trans =
		SimpleMath::Matrix::CreateTranslation(
			m_parameter.position.x,
			m_parameter.position.y + sinf(m_timer) / 10.0f + OFFSET_Y,
			m_parameter.position.z
		);

	// 行列計算
	world = rotate * trans;

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
//重力処理                                                //
//--------------------------------------------------------//
void Player::UpdateGravity()
{
	// 重力の加算
	m_parameter.gravity += 0.015f;

	// 重力の反映
	m_parameter.position.y -= m_parameter.gravity;

	// 死亡判定
	if (m_parameter.position.y < DEATH_LINE)
	{
		is_deathFlag = true;
	}
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
