/*
 *	@File	Player.cpp
 *	@Brief	プレイヤの処理をするクラス。
 *	@Date	2023-06-14
 *  @Author NakamuraRyo
 */

#include "pch.h"

// 使用するライブラリ
#include "Libraries/SystemManager/SystemManager.h"
#include "Libraries/UserUtility.h"
#include "Libraries/SystemDatas/Input.h"
#include "Libraries/Factories/ModelFactory.h"

#include "Player.h"

// コンストラクタ
Player::Player(std::unique_ptr<Model> head, std::unique_ptr<Model> body,
			   std::unique_ptr<Model> right, std::unique_ptr<Model> left, std::unique_ptr<Model> wink)
	: _timer{0.0f}					// タイマー
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

// デストラクタ
Player::~Player()
{
	Finalize();
	m_system.reset();
}

// 初期化処理
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

// 更新処理
void Player::Update(bool lookFlag)
{
	auto _key = Keyboard::Get().GetState();

	is_lookFlag = lookFlag;

	// 視点によって速度を変更する
	m_parameter.accelerate = is_lookFlag ? THIRD_SPEED : NORMAL_SPEED;

	// 重力処理
	UpdateGravity();

	// 首を回転する
	if (lookFlag)
	{
		// 上下回転
		if (_key.Up)
		{
			m_neckRotate.x += NECK_ROT_SPEED;
		}
		if (_key.Down)
		{
			m_neckRotate.x -= NECK_ROT_SPEED;
		}
		// クランプ
		m_neckRotate.x = UserUtility::Clamp(m_neckRotate.x, -10.0f, 15.0f);
		m_neckQuaternion.x = XMConvertToRadians(m_neckRotate.x);
		m_neckRotate.x = UserUtility::Lerp(m_neckRotate.x, 0.0f, NECK_ROT_SPEED * 0.1f);

		// 左右回転
		if (_key.Left)
		{
			m_neckRotate.y += NECK_ROT_SPEED;
		}
		if (_key.Right)
		{
			m_neckRotate.y -= NECK_ROT_SPEED;
		}
		// クランプ
		m_neckRotate.y = UserUtility::Clamp(m_neckRotate.y, -15.0f, 15.0f);
		m_neckQuaternion.y = XMConvertToRadians(m_neckRotate.y);
		m_neckRotate.y = UserUtility::Lerp(m_neckRotate.y, 0.0f, NECK_ROT_SPEED * 0.1f);
	}

	// 左右回転
	if (_key.A)
	{
		m_parameter.rotate *= SimpleMath::Quaternion::CreateFromAxisAngle(
			SimpleMath::Vector3::UnitY, ROT_SPEED);
	}
	if (_key.D)
	{
		m_parameter.rotate *= SimpleMath::Quaternion::CreateFromAxisAngle(
			SimpleMath::Vector3::UnitY, -ROT_SPEED);
	}
	// 前後移動
	if (_key.W)
	{
		SimpleMath::Vector3 _newVec(0.0f, 0.0f, m_parameter.accelerate);
		_newVec = SimpleMath::Vector3::Transform(_newVec, m_parameter.rotate);
		m_parameter.velocity -= _newVec;
	}
	else if (_key.S)
	{
		SimpleMath::Vector3 _newVec(0.0f, 0.0f, m_parameter.accelerate / 2);
		_newVec = SimpleMath::Vector3::Transform(_newVec, m_parameter.rotate);
		m_parameter.velocity += _newVec;
	}

	// 脚の動き
	if (_key.W || _key.A || _key.S || _key.D)
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

// 描画処理
void Player::Render(CommonStates& states, SimpleMath::Matrix view, SimpleMath::Matrix proj,
	const SimpleMath::Vector3& lightDir)
{
	auto _context = DX::DeviceResources::GetInstance()->GetD3DDeviceContext();

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
	m_rightLeg->Draw(_context, states, _legRMat, view, proj);

	// 左足の描画
	m_leftLeg->UpdateEffects(_lightingEffects);
	m_leftLeg->Draw(_context, states, _legLMat, view, proj);

	// 頭部の描画
	if (sinf(_timer) <= WINK_SPAN)
	{
		m_head->UpdateEffects(_lightingEffects);
		m_head->Draw(_context, states, _headMat, view, proj);
	}
	else
	{
		m_wink->UpdateEffects(_lightingEffects);
		m_wink->Draw(_context, states, _headMat, view, proj);
	}
	// 身体の描画
	m_body->UpdateEffects(_lightingEffects);
	m_body->Draw(_context, states, _commonMat, view, proj);
}

// 終了処理
void Player::Finalize()
{
	m_head.reset();
	m_body.reset();
	m_rightLeg.reset();
	m_leftLeg.reset();
	m_parameter.reset();
}

// 重力処理
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

// リスポーン関数
void Player::Spawn(SimpleMath::Vector3 spawnPosition)
{
	// パラメータの初期化
	m_parameter.reset();

	// 座標の設定
	m_position = spawnPosition;
}
