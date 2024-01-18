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
	DirectX::SimpleMath::Vector3 position;		// 座標
	DirectX::SimpleMath::Vector3 velocity;		// 移動量
	DirectX::SimpleMath::Quaternion rotate;		// 向いている方向
	float gravity;								// 重力
	float accelerate;							// 加速度
	void reset()								// リセット関数
	{
		position = DirectX::SimpleMath::Vector3::Zero;
		velocity = DirectX::SimpleMath::Vector3::Zero;
		gravity = 0.0f;
		accelerate = 0.0f;
		rotate = DirectX::SimpleMath::Quaternion::Identity;
	}
};

class Head;
class SystemManager;
class Player
{
private:

	// 頭
	std::unique_ptr<Head> m_head;

	// プレイヤのパラメータ
	Parameter m_parameter;

	// セレクトから受け取ったコイン数
	int m_coinNum;

	// 脚の動きに使う変数
	float m_footMove;

	// モデルデータ
	std::unique_ptr<DirectX::Model> m_body, m_leftLeg, m_rightLeg;

	// システム
	std::shared_ptr<SystemManager> m_system;

	// 死亡判定
	bool is_deathFlag;

	// ライティング
	DirectX::SimpleMath::Vector3 m_lightDirection;

private:

	// プレイヤのサイズ
	const float SIZE = 0.85f;
	// 移動速度
	const float NORMAL_SPEED = 0.01f;
	const float FOOT_SPEED = 0.1f;
	// 回転速度
	const float ROT_SPEED = 0.05f;
	// 摩擦係数
	const float DECELERATION = 0.851f;
	// プレイヤの浮遊
	const float OFFSET_Y = 0.2f;
	// 死亡ライン
	const float DEATH_LINE = -10.0f;
	// ウインクスパン
	const float WINK_SPAN = 0.95f;
	// ライトの変換速度
	const float LIGHT_SPEED = 0.1f;


public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="head">頭部のモデルデータ</param>
	/// <param name="body">胴体のモデルデータ</param>
	/// <param name="right">右足モデルデータ</param>
	/// <param name="left">左足モデルデータ</param>
	/// <returns>なし</returns>
	Player(std::unique_ptr<DirectX::Model> head,std::unique_ptr<DirectX::Model> body,
		std::unique_ptr<DirectX::Model> rightLeg, std::unique_ptr<DirectX::Model> leftLeg);
	~Player();

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="system">システムデータ</param>
	/// <returns>なし</returns>
	void Initialize(std::shared_ptr<SystemManager> system);

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="引数無し"></param>
	/// <returns>なし</returns>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name="states">コモンステート</param>
	/// <param name="view">ビュー行列</param>
	/// <param name="proj">射影行列</param>
	/// <param name="lightDir">ライティング</param>
	/// <returns>なし</returns>
	void Render(DirectX::CommonStates& states,DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj,
				const DirectX::SimpleMath::Vector3& lightDir);

	/// <summary>
	/// 終了処理
	/// </summary>
	/// <param name="引数無し"></param>
	/// <returns>なし</returns>
	void Finalize();

private:

	/// <summary>
	/// 重力処理
	/// </summary>
	/// <param name="引数無し"></param>
	/// <returns>なし</returns>
	void UpdateGravity();

	/// <summary>
	/// ライティングを更新する
	/// </summary>
	/// <param name="dir">ライトの方向</param>
	/// <returns>ライトの計算結果</returns>
	std::function<void(IEffect* effect)> UpdateLighting(DirectX::SimpleMath::Vector3 dir);
public:

	/// <summary>
	/// リスポーンさせる関数
	/// </summary>
	/// <param name="spawnPosition">プレイヤーの座標を指定する</param>
	/// <returns>なし</returns>
	void Spawn(DirectX::SimpleMath::Vector3 spawnPosition);

	// アクセサ----------------------------------------------------------------------------//

	// ライティング設定
	void InitializeLighting(const DirectX::SimpleMath::Vector3& lightDir) { m_lightDirection = lightDir; }
	// ポジションを取得
	const DirectX::SimpleMath::Vector3& GetPosition() { return m_parameter.position; }
	// ポジションを設定
	void SetPosition(const DirectX::SimpleMath::Vector3& position) { m_parameter.position = position; }
	// 重力リセット
	void ResetGravity() { m_parameter.gravity = 0.0f; }
	// 重力を取得
	const float& GetGravity() { return m_parameter.gravity; }
	// 重力を設定
	void SetGravity(const float& gravity) { m_parameter.gravity = gravity; }
	// サイズを取得
	const float& GetSize() { return SIZE; }
	// 死亡判定を取得
	const bool& GetDeathFlag() { return is_deathFlag; }
	// 回転量を取得
	const DirectX::SimpleMath::Quaternion& GetRotate() { return m_parameter.rotate; }
	// 合計コイン数を設定
	void SetAllCoins(const int& allCoins) { m_coinNum = allCoins; }
};

#endif // PLAYER