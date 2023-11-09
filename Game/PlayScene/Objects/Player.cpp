/*
 *	@File	Player.cpp
 *	@Brief	プレイヤの処理をするクラス。
 *	@Date	2023-06-14
 *  @Author NakamuraRyo
 */

#include "pch.h"
#include "Libraries/SystemManager/SystemManager.h"
#include "Libraries/UserUtility.h"
#include "Libraries/SystemDatas/Input.h"
#include "Libraries/Factories/ModelFactory.h"
#include "Player.h"

// コンストラクタ
Player::Player(std::unique_ptr<Model> head, std::unique_ptr<Model> body,
			   std::unique_ptr<Model> right, std::unique_ptr<Model> left, std::unique_ptr<Model> wink)
	: m_system{}					// システム
	, m_parameter{}					// パラメーター
	, m_coinNum{}					// 合計獲得コイン数
	, m_head{ std::move(head) }		// 頭のモデル
	, m_body{ std::move(body) }		// 身体のモデル
	, m_rightLeg{ std::move(right) }// 右足のモデル
	, m_leftLeg{ std::move(left) }	// 左足のモデル
	, m_wink{ std::move(wink)}		// ウインク差分
	, m_headMove{0.0f}				// 頭の動き
	, m_footMove{0.0f}				// 足の動き
	, m_neckQuaternion{}			// 首の回転行列
	, m_neckRotate{}				// 首の回転量
	, m_thirdRotate{}				// 三人称視点の回転量
	, m_lightDirection{}			// ライティングの向き
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
	m_lightDirection = SimpleMath::Vector3(1.0f, -1.0f, -1.0f);
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

	// 左右旋回をする
	float _rotationAngle = 0.0f;
	_rotationAngle = _key.A ? ROT_SPEED : _key.D ? -ROT_SPEED : 0.0f;
	SimpleMath::Quaternion _q = SimpleMath::Quaternion::CreateFromAxisAngle(SimpleMath::Vector3::UnitY, _rotationAngle);
	m_parameter.rotate *= _rotationAngle != 0.0f ? _q : SimpleMath::Quaternion::Identity;

	// 前後移動をする
	SimpleMath::Vector3 _moveDirection(0.0f, 0.0f, 0.0f);
	_moveDirection.z = _key.W ? -m_parameter.accelerate : _key.S ? m_parameter.accelerate * 0.5f: 0.0f;
	_moveDirection = SimpleMath::Vector3::Transform(_moveDirection, m_parameter.rotate);
	m_parameter.velocity += _moveDirection;

	// 脚の動き
	(_key.W || _key.A || _key.S || _key.D) ? m_footMove++ : m_footMove = 0.0f;

	// 頭も動かす
	m_headMove = m_footMove;

	// 移動量の計算
	m_parameter.position += m_parameter.velocity;

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
	auto _timer = static_cast<float>(DX::StepTimer::GetInstance().GetTotalSeconds());

	// ワールド行列
	SimpleMath::Matrix _headMat, _legRMat, _legLMat;

	// 回転行列
	SimpleMath::Matrix _rotate = SimpleMath::Matrix::CreateFromQuaternion(m_parameter.rotate);

	// 移動行列
	SimpleMath::Matrix _trans =
		SimpleMath::Matrix::CreateTranslation(
			m_parameter.position.x, m_parameter.position.y + OFFSET_Y, m_parameter.position.z);
	// 共通行列計算
	SimpleMath::Matrix _commonMat = _rotate * _trans;

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

	// 移動してから回転させる
	_legRMat = _rightTrans * _commonMat;
	_legLMat = _leftTrans  * _commonMat;

	// 頭は前後に動く
	if (!is_lookFlag)
	{
		_headMat = _headTrans * _commonMat;
	}
	else
	{
		_headMat = SimpleMath::Matrix::CreateFromQuaternion(m_neckQuaternion) * _headTrans * _commonMat;
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
	m_lightDirection = SimpleMath::Vector3::Lerp(m_lightDirection,
		SimpleMath::Vector3::TransformNormal(lightDir, _cameraRot), LIGHT_SPEED);

	// 右足の描画
	m_rightLeg->UpdateEffects(UpdateLighting(m_lightDirection));
	m_rightLeg->Draw(_context, states, _legRMat, view, proj);

	// 左足の描画
	m_leftLeg->UpdateEffects(UpdateLighting(m_lightDirection));
	m_leftLeg->Draw(_context, states, _legLMat, view, proj);

	// 頭部の描画
	if (sinf(_timer) <= WINK_SPAN)
	{
		m_head->UpdateEffects(UpdateLighting(m_lightDirection));
		m_head->Draw(_context, states, _headMat, view, proj);
	}
	else
	{
		m_wink->UpdateEffects(UpdateLighting(m_lightDirection));
		m_wink->Draw(_context, states, _headMat, view, proj);
	}
	// 身体の描画
	m_body->UpdateEffects(UpdateLighting(m_lightDirection));
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
	m_parameter.position.y -= m_parameter.gravity;

	// 死亡判定
	if (m_parameter.position.y < DEATH_LINE)
	{
		is_deathFlag = true;
	}
}

// ライティングの更新
std::function<void(IEffect* effect)> Player::UpdateLighting(SimpleMath::Vector3 dir)
{
	auto _lightingEffects = [&](IEffect* effect)
		{
			SimpleMath::Color lightColor(0.3f, 0.3f, 0.3f, 1.0f);
			// ベーシックエフェクト
			auto _basicEffect = dynamic_cast<BasicEffect*>(effect);
			if (_basicEffect)
			{
				// アンビエントライトカラーを設定
				_basicEffect->SetAmbientLightColor(SimpleMath::Color(0.2f, 0.2f, 0.2f));
			}

			// ライト
			auto _lights = dynamic_cast<IEffectLights*>(effect);
			if (_lights)
			{
				for (int i = 0; i < 3; ++i)
				{
					_lights->SetLightEnabled(i, true);
					_lights->SetLightDirection(i, dir);
					_lights->SetLightDiffuseColor(i, lightColor);
				}
			}

			// フォグ
			auto _fog = dynamic_cast<IEffectFog*>(effect);
			if (_fog)
			{
				// 霧を使うシェーダーに切り替える
				_fog->SetFogEnabled(true);

				// フォグの色を決める
				_fog->SetFogColor(Colors::LightGray);

				// スタート
				_fog->SetFogStart(-50.0f);

				// エンド
				_fog->SetFogEnd(150.0f);
			}
		};

	return _lightingEffects;
}

// リスポーン関数
void Player::Spawn(SimpleMath::Vector3 spawnPosition)
{
	// パラメータの初期化
	m_parameter.reset();

	// 座標の設定
	m_parameter.position = spawnPosition;
}
