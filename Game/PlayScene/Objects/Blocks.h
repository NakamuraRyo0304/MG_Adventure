/*
 *	@File	Blocks.h
 *	@Brief	マップのブロック管理。
 *	@Date	2023-07-11
 *  @Author NakamuraRyo
 */

// マップの使用
#include <map>

class Blocks
{
private:

	// マップローダー
	std::unique_ptr<MapLoad> m_map;

	// ブロック
	std::vector<Object> m_mapObj;

	// 雲ステータス
	struct Clowd 
	{
		bool moveFlag = false;
		DirectX::SimpleMath::Vector3 endPosition = DirectX::SimpleMath::Vector3::Zero;
	};

	// フラグ管理
	std::map<int, Clowd> m_clowdState;

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

private:
	// マップサイズ
	const float COMMON_SIZE = 0.9f;

	// コインのサイズ
	const float COIN_SIZE = COMMON_SIZE / 3;

	// 雲のサイズ
	const float CLOWD_SIZE = COMMON_SIZE / 2;

	// 最低高度
	const float	COMMON_LOW = COMMON_SIZE / 2;


private:
	// モデル
	std::unique_ptr<DirectX::Model> m_grassModel;
	std::unique_ptr<DirectX::Model> m_coinModel;
	std::unique_ptr<DirectX::Model> m_moveModel;

public:
	// ブロックの種類
	enum { GRASS, COIN, CLOWD , LENGTH };
public:

	// コンストラクタ
	Blocks();

	// デストラクタ
	~Blocks();

	// 初期化
	void Initialize(int stageNum);

	// 更新
	void Update(float elapsedTime);

	// 描画
	void Render(ID3D11DeviceContext* context, DirectX::CommonStates& states,
		DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj, float timer);

	// 終了処理
	void Finalize();

	// モデル作成
	void CreateModels(std::unique_ptr<Model> model,int modelName);

	// コインのカウントアップ
	void CountUpCoin(int index);
	const int& GetCoinCount() { return m_coinCount; }
	const bool& IsCollectedFlag();

	// プレイヤー座標ゲッター
	const DirectX::SimpleMath::Vector3& GetPlayerPosition() { return m_playerPos; }

	// マップ全体のゲッター
	const std::vector<Object>& GetMapData() { return m_mapObj; }

	// ブロックの大きさゲッター
	const float& GetObjSize(const int& objName)
	{
		if (objName == MapLoad::BoxState::CoinBox)
		{
			// コインは小さめサイズ
			return COIN_SIZE;
		}
		else if (objName == MapLoad::BoxState::ClowdBox)
		{
			// 雲も小さめサイズ
			return CLOWD_SIZE;
		}
		else
		{
			// デフォルトサイズ
			return COMMON_SIZE;
		}
	}

	// マップの判定セッター
	void SetClowdHitFlag(int index,bool flag){ m_clowdState[index].moveFlag = flag;	}

private:

	// マップセレクト
	std::wstring MapSelect(int num);
};