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
 /// <param name="head">頭部のモデルデータ</param>
 /// <param name="body">胴体のモデルデータ</param>
 /// <param name="right">右足モデルデータ</param>
 /// <param name="left">左足モデルデータ</param>
 /// <returns>なし</returns>
Player::Player(std::unique_ptr<Model> head, std::unique_ptr<Model> body, std::unique_ptr<Model> right, std::unique_ptr<Model> left):
	m_head{std::move(head)},
	m_body{std::move(body)},
	m_rightLeg{std::move(right)},
	m_leftLeg{std::move(left)},
	m_position{},
	m_parameter{},
	m_system{},
	is_deathFlag{},
	m_footMove{0.0f},
	m_neckQuaternion{},
	m_neckRotate{},
	is_lookFlag{},
	m_thirdRotate{}
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

	// 回転量の初期化
	m_neckRotate = SimpleMath::Vector2::Zero;
	m_neckQuaternion = SimpleMath::Quaternion::Identity;
	m_thirdRotate = SimpleMath::Quaternion::Identity;
	is_lookFlag = false;
}

/// <summary>
/// 更新処理
/// </summary>
/// <param name="keyState">キーボード</param>
/// <param name="timer">派生シーンのStepTimer(TotalTime)</param>
/// <param name="lookFlag">視点フラグ</param>
/// <returns>なし</returns>
void Player::Update(Keyboard::State& keyState, float timer, bool lookFlag)
{
	m_timer = timer;

	is_lookFlag = lookFlag;

	// 重力処理
	UpdateGravity();

	// 首を回転する
	if (lookFlag)
	{
		// 上下回転
		if (keyState.Up)
		{
			m_neckRotate.x += NECK_ROT_SPEED;
		}
		if (keyState.Down)
		{
			m_neckRotate.x -= NECK_ROT_SPEED;
		}
		// クランプ
		m_neckRotate.x = UserUtility::Clamp(m_neckRotate.x, -10.0f, 15.0f);
		m_neckQuaternion.x = XMConvertToRadians(m_neckRotate.x);
		m_neckRotate.x = UserUtility::Lerp(m_neckRotate.x, 0.0f, NECK_ROT_SPEED * 0.1f);

		// 左右回転
		if (keyState.Left)
		{
			m_neckRotate.y += NECK_ROT_SPEED;
		}
		if (keyState.Right)
		{
			m_neckRotate.y -= NECK_ROT_SPEED;
		}
		// クランプ
		m_neckRotate.y = UserUtility::Clamp(m_neckRotate.y, -15.0f, 15.0f);
		m_neckQuaternion.y = XMConvertToRadians(m_neckRotate.y);
		m_neckRotate.y = UserUtility::Lerp(m_neckRotate.y, 0.0f, NECK_ROT_SPEED * 0.1f);
	}

	// 左右回転
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
	// 前後移動
	if (keyState.W)
	{
		SimpleMath::Vector3 vec(0.0f, 0.0f, m_parameter.accelerate);
		vec = SimpleMath::Vector3::Transform(vec, m_parameter.rotate);
		m_parameter.velocity -= vec;
	}
	else if (keyState.S)
	{
		SimpleMath::Vector3 vec(0.0f, 0.0f, m_parameter.accelerate / 2);
		vec = SimpleMath::Vector3::Transform(vec, m_parameter.rotate);
		m_parameter.velocity += vec;
	}

	// 脚の動き
	if (keyState.W || keyState.A || keyState.S || keyState.D)
	{
		m_footMove++;
	}
	else
	{
		m_footMove = 0.0f;
	}

	// 移動量の計算
	m_position += m_parameter.velocity;

	// 減速処理
	if (m_parameter.velocity != SimpleMath::Vector3::Zero)
	{
		m_parameter.velocity *= DECELERATION;
	}

	// 最終的な回転量
	m_thirdRotate = m_neckQuaternion * m_parameter.rotate;
}

/// <summary>
/// 描画処理
/// </summary>
/// <param name="context">ID3D11DeviceContextポインタ</param>
/// <param name="states">コモンステート</param>
/// <param name="view">ビュー行列</param>
/// <param name="proj">射影行列</param>
/// <returns>なし</returns>
void Player::Render(ID3D11DeviceContext* context, CommonStates& states,
	SimpleMath::Matrix view, SimpleMath::Matrix proj)
{
	// ワールド行列
	SimpleMath::Matrix world, headWorld, legRWorld, legLWorld;

	// 回転行列
	SimpleMath::Matrix rotate = SimpleMath::Matrix::CreateFromQuaternion(m_parameter.rotate);

	// プレイヤーの移動行列
	SimpleMath::Matrix trans =
		SimpleMath::Matrix::CreateTranslation(
			m_position.x,
			m_position.y + OFFSET_Y,
			m_position.z
		);

	// 右脚の動き
	SimpleMath::Matrix rightTrans =
		SimpleMath::Matrix::CreateTranslation(
			0.0f,
			0.0f,
			sinf(m_footMove) * 0.1f
		);

	// 左脚の動き
	SimpleMath::Matrix leftTrans =
		SimpleMath::Matrix::CreateTranslation(
			0.0f,
			0.0f,
			-sinf(m_footMove) * 0.1f
		);

	// 頭部の動き
	SimpleMath::Matrix headTrans =
		SimpleMath::Matrix::CreateTranslation(
			0.0f,
			0.0f,
			sinf(m_footMove * 0.25f) * 0.1f
		);

	// 行列計算
	world = rotate * trans;

	// 移動してから回転させる
	legRWorld = rightTrans * rotate * trans;
	legLWorld = leftTrans  * rotate * trans;

	// 頭は前後に動く
	if (!is_lookFlag)
	{
		headWorld = headTrans * rotate * trans;
	}
	else
	{
		headWorld = SimpleMath::Matrix::CreateFromQuaternion(m_neckQuaternion) * headTrans * rotate * trans;
	}

	// ライトの設定
	SimpleMath::Vector3 lightDirection(1.0f, -1.0f, -1.0f);

	// ビュー行列からカメラの回転を取得
	SimpleMath::Matrix cameraRotation;

	// ビュー行列を逆変換
	cameraRotation = view.Invert();

	// 移動量をなくす
	cameraRotation._41 = 0.0f;
	cameraRotation._42 = 0.0f;
	cameraRotation._43 = 0.0f;

	// ライトの方向をカメラの回転に逆向きにする
	lightDirection = SimpleMath::Vector3::TransformNormal(lightDirection, cameraRotation);
	SimpleMath::Color lightColor(0.3f, 0.3f, 0.3f, 1.0f);

	auto setLightForModel = [&](IEffect* effect)
	{
		auto lights = dynamic_cast<IEffectLights*>(effect);
		if (lights)
		{
			// ライトオン
			lights->SetLightEnabled(0, true);
			lights->SetLightEnabled(1, true);
			lights->SetLightEnabled(2, true);

			// ライトの方向を設定
			lights->SetLightDirection(0, lightDirection);
			lights->SetLightDirection(1, lightDirection);
			lights->SetLightDirection(2, lightDirection);

			// ライトの色を暗めのグレーに設定
			lights->SetLightDiffuseColor(0, lightColor);
			lights->SetLightDiffuseColor(1, lightColor);
			lights->SetLightDiffuseColor(2, lightColor);
		}
	};

	// モデルの描画
	// 右足の描画
	m_rightLeg->UpdateEffects(setLightForModel);
	m_rightLeg->Draw(context, states, legRWorld, view, proj);

	// 左足の描画
	m_leftLeg->UpdateEffects(setLightForModel);
	m_leftLeg->Draw(context, states, legLWorld, view, proj);

	// 頭部の描画
	m_head->UpdateEffects(setLightForModel);
	m_head->Draw(context, states, headWorld, view, proj);

	// 身体の描画
	m_body->UpdateEffects(setLightForModel);
	m_body->Draw(context, states, world, view, proj);
}

/// <summary>
/// 終了処理
/// </summary>
/// <param name="引数無し"></param>
/// <returns>なし</returns>
void Player::Finalize()
{
	m_head.reset();
	m_body.reset();
	m_rightLeg.reset();
	m_leftLeg.reset();
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
