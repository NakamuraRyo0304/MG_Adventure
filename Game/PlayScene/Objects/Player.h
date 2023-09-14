/*
 *	@File	Player.h
 *	@Brief	プレイヤの処理をするクラス。
 *	@Date	2023-06-14
 *  @Author NakamuraRyo
 */

#pragma once
#ifndef PLAYER
#define PLAYER

struct Parameter
{
	DirectX::SimpleMath::Vector3 velocity;		// 移動量
	DirectX::SimpleMath::Quaternion rotate;		// 向いている方向
	float gravity;								// 重力
	float accelerate;							// 加速度
	void reset()								// リセット関数
	{
		velocity = DirectX::SimpleMath::Vector3::Zero;
		gravity = 0.0f;
		accelerate = 0.0f;
		rotate = DirectX::SimpleMath::Quaternion::Identity;
	}
};

class SystemManager;
class Player
{
private:

	// 時間
	float m_timer;

	// 脚の動きに使う変数
	float m_footMove;

	// 座標
	DirectX::SimpleMath::Vector3 m_position;

	// プレイヤのパラメータ
	Parameter m_parameter;

	// 首の動き
	DirectX::SimpleMath::Quaternion m_neckQuaternion;
	DirectX::SimpleMath::Vector2 m_neckRotate;
	bool is_lookFlag;

	// 三人称の回転
	DirectX::SimpleMath::Quaternion m_thirdRotate;

	// モデルデータ
	std::unique_ptr<DirectX::Model> m_head, m_body, m_leftLeg, m_rightLeg;

	// システム
	std::weak_ptr<SystemManager> m_system;

	// 死亡判定
	bool is_deathFlag;

private:

	// プレイヤのサイズ
	const float SIZE = 0.85f;
	// 回転速度
	const float ROT_SPEED = 0.05f;
	const float NECK_ROT_SPEED = 0.30f;
	// 摩擦係数
	const float DECELERATION = 0.851f;
	// プレイヤの浮遊
	const float OFFSET_Y = 0.2f;
	// 死亡ライン
	const float DEATH_LINE = -10.0f;


public:
	// コンストラクタ（頭、身体、右足、左足）
	Player(std::unique_ptr<DirectX::Model> head,std::unique_ptr<DirectX::Model> body,
		std::unique_ptr<DirectX::Model> rightLeg, std::unique_ptr<DirectX::Model> leftLeg);
	~Player();

	// 初期化処理（システムマネージャポインタ）
	void Initialize(std::shared_ptr<SystemManager> system);

	// 更新処理（キーボード、タイマー、視点変更フラグ）
	void Update(DirectX::Keyboard::State& keyState,float timer, bool lookFlag);

	// 描画処理（コンテキスト、ステート、ビュー行列、射影行列）
	void Render(ID3D11DeviceContext* context, DirectX::CommonStates& states,
		        DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);

	// 終了処理
	void Finalize();

private:

	// 重力処理
	void UpdateGravity();

public:

	// アクセサ----------------------------------------------------------------------------//

	// ポジションアクセサ
	DirectX::SimpleMath::Vector3& GetPosition() { return m_position; }
	void SetPosition(const DirectX::SimpleMath::Vector3& position) { m_position = position; }

	// スポーン関数
	void Spawn(DirectX::SimpleMath::Vector3 spawnPosition);

	// 重力リセット
	void ResetGravity() { m_parameter.gravity = 0.0f; }
	// 重力アクセサ
	const float& GetGravity() { return m_parameter.gravity; }
	void SetGravity(const float& gravity) { m_parameter.gravity = gravity; }

	// サイズアクセサ
	const float& GetSize() { return SIZE; }

	// 死亡判定ゲッター
	const bool& GetDeathFlag() { return is_deathFlag; }

	// 回転量を取得
	const DirectX::SimpleMath::Quaternion& GetRotate() { return m_parameter.rotate; }

	// 首の回転量を取得
	const DirectX::SimpleMath::Quaternion& GetNeckRotate() { return m_thirdRotate; }
};

#endif // PLAYER