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

 /// <summary>
 /// コンストラクタ
 /// </summary>
 /// <param name="model">プレイシーンで作成したモデルデータ</param>
 /// <returns>なし</returns>
Player::Player(std::unique_ptr<Model> model):
	m_model{std::move(model)},
	m_position{},
	m_parameter{},
	m_system{},
	is_deathFlag{}
{
}

/// <summary>
/// デストラクタ
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
Player::~Player()
{
	Finalize();
	m_system.reset();
}

/// <summary>
/// 初期化処理
/// </summary>
/// <param name="system">システムデータ</param>
/// <returns>なし</returns>
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

/// <summary>
/// 更新処理
/// </summary>
/// <param name="keyState">キーボード</param>
/// <param name="timer">派生シーンのStepTimer(TotalTime)</param>
/// <returns>なし</returns>
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
	m_position += m_parameter.velocity;

	// 減速処理
	if (m_parameter.velocity != SimpleMath::Vector3::Zero)
	{
		m_parameter.velocity *= DECELERATION;
	}
}

/// <summary>
/// 描画処理
/// </summary>
/// <param name="context">ID3D11DeviceContextポインタ</param>
/// <param name="states">コモンステート</param>
/// <param name="view">ビュー行列</param>
/// <param name="proj">射影行列</param>
/// <returns>なし</returns>
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
			m_position.x,
			m_position.y + sinf(m_timer) / 10.0f + OFFSET_Y,
			m_position.z
		);

	// 行列計算
	world = rotate * trans;

	// モデルの描画
	m_model->Draw(context, states, world, view, proj, false);
}

/// <summary>
/// 終了処理
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
void Player::Finalize()
{
	m_model.reset();
	m_parameter.reset();
}

/// <summary>
/// 重力処理
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
void Player::UpdateGravity()
{
	// 重力の加算
	m_parameter.gravity += 0.015f;

	// 重力の反映
	m_position.y -= m_parameter.gravity;

	// 死亡判定
	if (m_position.y < DEATH_LINE)
	{
		is_deathFlag = true;
	}
}

/// <summary>
/// リスポーンさせる関数
/// </summary>
/// <param name="spawnPosition">プレイヤーの座標を指定する</param>
/// <returns>なし</returns>
void Player::Spawn(SimpleMath::Vector3 spawnPosition)
{
	// パラメータの初期化
	m_parameter.reset();

	// 座標の設定
	m_position = spawnPosition;
}
