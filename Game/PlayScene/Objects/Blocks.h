/*
 *	@File	Blocks.h
 *	@Brief	マップのブロック管理。
 *	@Date	2023-07-11
 *  @Author NakamuraRyo
 */

// エイリアス
using MAPSTATE = MapLoad::BOXSTATE;

class Blocks
{
private:

	// マップローダー
	std::unique_ptr<MapLoad> m_mapLoad;

	// ブロック
	std::vector<Object> m_mapObj;

	// 雲ステータス
	struct Cloud
	{
		bool moveFlag = false;
		float timer = 0.0f;
		DirectX::SimpleMath::Vector3 initPosition = DirectX::SimpleMath::Vector3::Zero;
		DirectX::SimpleMath::Vector3 endPosition = DirectX::SimpleMath::Vector3::Zero;
	};

	// フラグ管理
	std::map<int, Cloud> m_cloudState;

	// プレイヤー座標
	DirectX::SimpleMath::Vector3 m_playerPos;

	// コインカウンタ
	int m_coinCount;
	// コインの最大値
	int m_maxCoins;
	// コイン回収済みフラグ
	bool is_collectedFlag;

	// 当たったかどうかの判定
	bool is_hitCoinFlag;

	// 雲のシェーダー
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_psCloud;

	// ライティング
	DirectX::SimpleMath::Vector3 m_lighting;

private:
	// マップサイズ
	const float COMMON_SIZE = 0.9f;

	// コインのサイズ
	const float COIN_SIZE = COMMON_SIZE / 3;

	// 雲のサイズ
	const float CLOUD_SIZE = COMMON_SIZE / 1.7f;
	const float CLOUD_SPEED = 0.1f;

	// 雲リセットエリアサイズ
	const float GRAVITY_SIZE = 0.85f;

	// 最低高度
	const float	COMMON_LOW = COMMON_SIZE / 2;


private:
	// モデル
	std::unique_ptr<DirectX::Model> m_grassModel;
	std::unique_ptr<DirectX::Model> m_coinModel;
	std::unique_ptr<DirectX::Model> m_cloudModel;
	std::unique_ptr<DirectX::Model> m_gravityModel;

public:
	// ブロックの種類
	enum { GRASS, COIN, CLOWD, GRAVITY, LENGTH };

public:

	Blocks();
	~Blocks();

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="stageNum">ステージ番号</param>
	/// <returns>なし</returns>
	void Initialize(int stageNum);

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
	/// <param name="timer">経過時間</param>
	/// <param name="lightDir">ライティング</param>
	/// <returns>なし</returns>
	void Render(DirectX::CommonStates& states,
		DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj, float timer,
		const DirectX::SimpleMath::Vector3& lightDir);

	/// <summary>
	/// 終了処理
	/// </summary>
	/// <param name="引数無し"></param>
	/// <returns>なし</returns>
	void Finalize();

	/// <summary>
	/// モデル作成
	/// </summary>
	/// <param name="model">モデルデータ</param>
	/// <param name="modelNum">対応したモデル番号</param>
	/// <returns>なし</returns>
	void CreateModels(std::unique_ptr<Model> model, int modelName);

public:

	/// <summary>
	/// シェーダーの作成
	/// </summary>
	/// <param name="引数無し"></param>
	/// <returns>なし</returns>
	void CreateShader();

	/// <summary>
	/// コイン取得処理
	/// </summary>
	/// <param name="index">当たったコインの番号</param>
	/// <returns>なし</returns>
	void CountUpCoin(int index);

	/// <summary>
	/// 雲の位置をもとに戻す処理
	/// </summary>
	/// <param name="引数無し"></param>
	/// <returns>なし</returns>
	void CallGravity();

	// ライティングのセッター
	void InitializeLighting(const DirectX::SimpleMath::Vector3& lightDir) { m_lighting = lightDir; }
public:

	// 取得コイン数を取得
	const int& GetCoinCount() { return m_coinCount; }
	// 最大コイン数を取得
	const int& GetMaxCoinCount() { return m_maxCoins; }
	// コインの回収判定
	const bool& IsCollectedFlag() { return is_collectedFlag; }
	// プレイヤー座標を取得
	const DirectX::SimpleMath::Vector3& GetPlayerPosition() { return m_playerPos; }

	// マップ全体を取得
	const std::vector<Object>& GetMapData() { return m_mapObj; }

	// ブロック単体の座標を取得
	DirectX::SimpleMath::Vector3& GetBlockPosition(const int& index)
	{
		if (index >= m_mapObj.max_size()) throw std::out_of_range("Index out of range");
		return m_mapObj[index].position;
	}
	void SetBlockPosition(const DirectX::SimpleMath::Vector3& newPos, const int& index) { m_mapObj[index].position = newPos; }

	/// <summary>
	/// サイズゲッター
	/// </summary>
	/// <param name="objName">オブジェクトの名前(構造体)</param>
	/// <returns>なし</returns>
	const float& GetObjSize(const int& objName);
	// 雲の判定アクセサ
	void SetCloudHitFlag(const int& index, bool flag) { m_cloudState[index].moveFlag = flag; }

private:

	/// <summary>
	/// マップ選択
	/// </summary>
	/// <param name="num">ステージ番号</param>
	/// <returns>ファイルパス</returns>
	std::wstring MapSelect(int num);
};