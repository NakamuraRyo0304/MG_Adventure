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

// モデルファクトリー
#include "../../../Libraries/Factories/ModelFactory.h"

#include "Player.h"

 /// <summary>
 /// コンストラクタ
 /// </summary>
 /// <param name="head">頭部のモデルデータ</param>
 /// <param name="body">胴体のモデルデータ</param>
 /// <param name="right">右足モデルデータ</param>
 /// <param name="left">左足モデルデータ</param>
 /// <returns>なし</returns>
Player::Player(std::unique_ptr<Model> head, std::unique_ptr<Model> body,
			   std::unique_ptr<Model> right, std::unique_ptr<Model> left, std::unique_ptr<Model> wink)
	: m_timer{0.0f}					// タイマー
	, m_system{}					// システム
	, m_parameter{}					// パラメーター
	, m_coinNum{}					// 合計獲得コイン数
	, m_head{ std::move(head) }		// 頭のモデル
	, m_body{ std::move(body) }		// 身体のモデル
	, m_rightLeg{ std::move(right) }// 右足のモデル
	, m_leftLeg{ std::move(left) }	// 左足のモデル
	, m_wink{ std::move(wink)}		// ウインク差分
	, m_position{}					// ポジション
	, m_headMove{0.0f}				// 頭の動き
	, m_footMove{0.0f}				// 足の動き
	, m_neckQuaternion{}			// 首の回転行列
	, m_neckRotate{}				// 首の回転量
	, m_thirdRotate{}				// 三人称視点の回転量
	, m_lighting{}					// ライティング
	, is_lookFlag{}					// 視点フラグ
	, is_deathFlag{}				// 死亡フラグ
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
	m_parameter.accelerate = NORMAL_SPEED;

	// 死亡判定の初期化
	is_deathFlag = false;

	// 回転量の初期化
	m_neckRotate = SimpleMath::Vector2::Zero;
	m_neckQuaternion = SimpleMath::Quaternion::Identity;
	m_thirdRotate = SimpleMath::Quaternion::Identity;
	is_lookFlag = false;

	// ライティングリセット
	m_lighting = SimpleMath::Vector3::Zero;
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

	// 視点によって速度を変更する
	m_parameter.accelerate = is_lookFlag ? THIRD_SPEED : NORMAL_SPEED;

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
		SimpleMath::Vector3 _newVec(0.0f, 0.0f, m_parameter.accelerate);
		_newVec = SimpleMath::Vector3::Transform(_newVec, m_parameter.rotate);
		m_parameter.velocity -= _newVec;
	}
	else if (keyState.S)
	{
		SimpleMath::Vector3 _newVec(0.0f, 0.0f, m_parameter.accelerate / 2);
		_newVec = SimpleMath::Vector3::Transform(_newVec, m_parameter.rotate);
		m_parameter.velocity += _newVec;
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

	// 頭も動かす
	m_headMove = m_footMove;

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
/// <param name="lightDir">ライティング</param>
/// <returns>なし</returns>
void Player::Render(ID3D11DeviceContext* context, CommonStates& states,
	SimpleMath::Matrix view, SimpleMath::Matrix proj,const SimpleMath::Vector3& lightDir)
{
	// ワールド行列
	SimpleMath::Matrix _commonMat, _headMat, _legRMat, _legLMat;

	// 回転行列
	SimpleMath::Matrix _rotate = SimpleMath::Matrix::CreateFromQuaternion(m_parameter.rotate);

	// プレイヤーの移動行列
	SimpleMath::Matrix _trans =
		SimpleMath::Matrix::CreateTranslation(
			m_position.x,
			m_position.y + OFFSET_Y,
			m_position.z
		);

	// 右脚の動き
	SimpleMath::Matrix _rightTrans =
		SimpleMath::Matrix::CreateTranslation(
			0.0f,
			0.0f,
			sinf(m_footMove) * FOOT_SPEED
		);

	// 左脚の動き
	SimpleMath::Matrix _leftTrans =
		SimpleMath::Matrix::CreateTranslation(
			0.0f,
			0.0f,
			-sinf(m_footMove) * FOOT_SPEED
		);

	// 頭部の動き
	SimpleMath::Matrix _headTrans =
		SimpleMath::Matrix::CreateTranslation(
			0.0f,
			0.0f,
			sinf(m_headMove * HEAD_SPEED) * 0.05f
		);

	// 行列計算
	_commonMat = _rotate * _trans;

	// 移動してから回転させる
	_legRMat = _rightTrans * _rotate * _trans;
	_legLMat = _leftTrans  * _rotate * _trans;

	// 頭は前後に動く
	if (!is_lookFlag)
	{
		_headMat = _headTrans * _rotate * _trans;
	}
	else
	{
		_headMat = SimpleMath::Matrix::CreateFromQuaternion(m_neckQuaternion) * _headTrans * _rotate * _trans;
	}

	// ビュー行列からカメラの回転を取得
	SimpleMath::Matrix _cameraRot;

	// ビュー行列を逆変換
	_cameraRot = view.Invert();

	// 移動量をなくす
	_cameraRot._41 = 0.0f;
	_cameraRot._42 = 0.0f;
	_cameraRot._43 = 0.0f;

	// ライトの方向をカメラの回転に逆向きにする
	m_lighting = SimpleMath::Vector3::TransformNormal(lightDir, _cameraRot);
	SimpleMath::Color _lightColor(0.3f, 0.3f, 0.3f, 1.0f);

	auto _lightingEffects = [&](IEffect* effect)
	{
		auto _lights = dynamic_cast<IEffectLights*>(effect);
		if (_lights)
		{
			// ライトオン
			_lights->SetLightEnabled(0, true);
			_lights->SetLightEnabled(1, true);
			_lights->SetLightEnabled(2, true);

			// ライトの方向を設定
			_lights->SetLightDirection(0, m_lighting);
			_lights->SetLightDirection(1, m_lighting);
			_lights->SetLightDirection(2, m_lighting);

			// ライトの色を暗めのグレーに設定
			_lights->SetLightDiffuseColor(0, _lightColor);
			_lights->SetLightDiffuseColor(1, _lightColor);
			_lights->SetLightDiffuseColor(2, _lightColor);
		}
		auto _basicEffect = dynamic_cast<BasicEffect*>(effect);
		if (_basicEffect)
		{
			// アンビエントライトカラーの設定
			_basicEffect->SetAmbientLightColor(SimpleMath::Color(0.2f, 0.2f, 0.2f));
		}
	};


	// 右足の描画
	m_rightLeg->UpdateEffects(_lightingEffects);
	m_rightLeg->Draw(context, states, _legRMat, view, proj);

	// 左足の描画
	m_leftLeg->UpdateEffects(_lightingEffects);
	m_leftLeg->Draw(context, states, _legLMat, view, proj);

	// 頭部の描画
	if (sinf(m_timer) <= WINK_SPAN)
	{
		m_head->UpdateEffects(_lightingEffects);
		m_head->Draw(context, states, _headMat, view, proj);
	}
	else
	{
		m_wink->UpdateEffects(_lightingEffects);
		m_wink->Draw(context, states, _headMat, view, proj);
	}
	// 身体の描画
	m_body->UpdateEffects(_lightingEffects);
	m_body->Draw(context, states, _commonMat, view, proj);
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
