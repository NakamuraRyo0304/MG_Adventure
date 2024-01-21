/*
 *	@File	Blocks.cpp
 *	@Brief	マップのブロック管理。
 *	@Date	2023-07-11
 *  @Author NakamuraRyo
 */

#include "pch.h"
#include "Libraries/SystemDatas/MapLoad.h"
#include "Libraries/FactoryManager/FactoryManager.h"
#include "Libraries/UserUtility.h"
#include "Blocks.h"

// コンストラクタ
Blocks::Blocks(std::shared_ptr<FactoryManager> factory)
	: m_factory{factory}						// ファクトリマネージャ
	, m_coinCount{0}							// コインカウンタ
	, m_maxCoins{0}								// コイン最大値
	, m_playerPos{ SimpleMath::Vector3::Zero }	// プレイヤーポジション
	, m_grassModel{ nullptr }					// 草ブロックのモデル
	, m_coinModel{ nullptr }					// コインブロックのモデル
	, m_cloudModel{ nullptr }					// 雲ブロックのモデル
	, m_gravityModel{ nullptr }					// 重力ブロックのモデル
	, m_lighting{}								// ライティング
	, is_collectedFlag{ false }					// コイン回収済み判定フラグ
	, is_hitCoinFlag{ false }					// 判定フラグ
{
	m_mapLoad = std::make_unique<MapLoad>();
}

// デストラクタ
Blocks::~Blocks()
{

}

// 初期化処理
void Blocks::Initialize(int stageNum)
{
	// ファイル名の宣言
	std::wstring _filePath = L"";

	// ステージセレクト
	_filePath = MapSelect(stageNum);

	// マップの読み込み
	m_mapLoad->LoadMap(_filePath);

	// マップの格納
	m_mapObj = m_mapLoad->GetMapData();

	// メモリの解放
	m_mapLoad->ReleaseMemory();

	// 座標を補正する
	MapSwipe();

	// コイン回収済みフラグをFalseにする
	is_collectedFlag = false;
	// 当たり判定フラグをFalseにする
	is_hitCoinFlag = false;

	// ライティングのリセット
	m_lighting = SimpleMath::Vector3::Zero;

	// コンスタントバッファの作成
	CreateConstBuffer();

}

// 更新処理
void Blocks::Update()
{
	auto _timer = static_cast<float>(DX::StepTimer::GetInstance().GetTotalSeconds());

	// 雲は上下移動する
	for (int i = 0; i < m_mapObj.size(); ++i)
	{
		if (m_mapObj[i].id == MAPSTATE::CLOUD)
		{
			// 触れているときは終点まで
			if (not m_cloudState[i].moveFlag) continue;

			// Y座標を終点まで動かす
			m_mapObj[i].position.y = UserUtility::Lerp
			(
				m_mapObj[i].position.y,
				m_cloudState[i].endPosition.y,
				CLOUD_SPEED
			);
		}
		// 重力ブロックの位置を上下させる
		if (m_mapObj[i].id == MAPSTATE::GRAVITY)
		{
			m_mapObj[i].position.y += sinf(_timer) * 0.5f * GRAVITY_MOVE_Y;
		}
	}

	// コインを回収したらTrueにする
	if (m_coinCount == m_maxCoins)
	{
		is_collectedFlag = true;
	}
}

// 描画関数
void Blocks::Render(CommonStates& states,SimpleMath::Matrix view, SimpleMath::Matrix proj,
	float timer, const SimpleMath::Vector3& lightDir)
{
	auto _context = DX::DeviceResources::GetInstance()->GetD3DDeviceContext();

	// ワールド座標
	SimpleMath::Matrix _world, _rotate, _verticle;

	// ライティング設定
	std::function<void(IEffect* effect)> _lightSetting;

	// ブロックの描画
	for (int i = 0; i < m_mapObj.size(); i++)
	{
		if (m_mapObj[i].id == MAPSTATE::NONE) continue;

		// 移動行列
		_world = SimpleMath::Matrix::CreateTranslation(m_mapObj[i].position);

		// 回転行列
		_rotate = SimpleMath::Matrix::CreateRotationY(timer) * CLOUD_ROT_SPEED;

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
		SimpleMath::Color lightColor(0.3f, 0.3f, 0.3f, 1.0f);

		// 上下移動する
		_verticle *= SimpleMath::Matrix::CreateTranslation(
			SimpleMath::Vector3(0.0f, ((sinf(timer) + 1) * 0.5f) * 2.f, 0.0f));

		_lightSetting = [&](IEffect* effect)
		{
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
				// ライトオン
				_lights->SetLightEnabled(0, true);
				_lights->SetLightEnabled(1, true);
				_lights->SetLightEnabled(2, true);

				// ライトの方向を設定
				_lights->SetLightDirection(0, m_lighting);
				_lights->SetLightDirection(1, m_lighting);
				_lights->SetLightDirection(2, m_lighting);

				// ライトの色を暗めのグレーに設定
				_lights->SetLightDiffuseColor(0, lightColor);
				_lights->SetLightDiffuseColor(1, lightColor);
				_lights->SetLightDiffuseColor(2, lightColor);
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

		if (m_mapObj[i].id == MAPSTATE::GRASS)
		{
			m_grassModel->UpdateEffects(_lightSetting);
			m_grassModel->Draw(_context, states, _world, view, proj);
		}
	}
	// コインの描画
	for (int i = 0; i < m_mapObj.size(); i++)
	{
		if (m_mapObj[i].id != MAPSTATE::COIN) continue;

		_world = SimpleMath::Matrix::CreateTranslation(m_mapObj[i].position);
		m_coinModel->Draw(_context, states, _rotate * _world, view, proj);
	}
	// 雲ブロックの描画
	for (int i = 0; i < m_mapObj.size(); i++)
	{
		if (m_mapObj[i].id != MAPSTATE::CLOUD) continue;

		_world = SimpleMath::Matrix::CreateTranslation(m_mapObj[i].position);
		m_cloudModel->Draw(_context, states, _rotate * _world, view, proj);
	}
	// 重力ブロックの描画
	for (int i = 0; i < m_mapObj.size(); i++)
	{
		if (m_mapObj[i].id != MAPSTATE::GRAVITY) continue;

		_world = SimpleMath::Matrix::CreateTranslation(m_mapObj[i].position);
		m_gravityModel->Draw(_context, states, (_rotate * 2.0f) * _world, view, proj, false, [&]()
			{
				_context->OMSetBlendState(states.NonPremultiplied(), nullptr, 0xffffffff);
				_context->OMSetDepthStencilState(states.DepthDefault(), 0);
				_context->PSSetShader(m_psGravity.Get(), nullptr, 0);
			}
		);
	}
}

// 終了処理
void Blocks::Finalize()
{
	// マップの解放
	m_mapObj.clear();

	// 最後までリセットされていたらスキップ
	if (not m_gravityModel) return;

	m_factory->BuildModelFactory();

	m_factory->VisitModelFactory()->DeleteModel(m_grassModel);
	m_factory->VisitModelFactory()->DeleteModel(m_coinModel);
	m_factory->VisitModelFactory()->DeleteModel(m_cloudModel);
	m_factory->VisitModelFactory()->DeleteModel(m_gravityModel);

	m_factory->LeaveModelFactory();
	m_factory.reset();
}

// モデル作成関数
void Blocks::CreateModels(std::unique_ptr<Model> model,int modelNum)
{
	switch (modelNum)
	{
	case GRASS:		m_grassModel = std::move(model);	break;
	case COIN:		m_coinModel = std::move(model);		break;
	case CLOWD:		m_cloudModel = std::move(model);	break;
	case GRAVITY:	m_gravityModel = std::move(model);	break;
	default: break;
	}
}

// シェーダー作成
void Blocks::CreateShader()
{
	// ファクトリでシェーダーを生成
	m_factory->BuildShaderFactory();

	m_factory->VisitShaderFactory()->CreatePixelShader(L"Resources/Shaders/PS_Gravity.cso", &m_psGravity);

	m_factory->LeaveShaderFactory();
}

// コインのカウントアップ
void Blocks::CountUpCoin(int index)
{
	m_coinCount++;
	m_mapObj[index].id = MAPSTATE::NONE;
}

// オブジェクトの大きさを取得
const float& Blocks::GetObjSize(const int& objName)

{
	if (objName == MAPSTATE::COIN)
	{
		// コインは小さめサイズ
		return COIN_SIZE;
	}
	else if (objName == MAPSTATE::CLOUD)
	{
		// 雲も小さめサイズ
		return CLOUD_SIZE;
	}
	else if (objName == MAPSTATE::GRAVITY)
	{
		// 重力は少し大きめサイズ
		return GRAVITY_SIZE;
	}
	else
	{
		// デフォルトサイズ
		return COMMON_SIZE;
	}
}

// 雲を元に戻す
void Blocks::CallGravity()
{
	for (auto& obj : m_mapObj)
	{
		// 雲のみを対象とする
		if (obj.id == MAPSTATE::CLOUD)
		{
			// Y座標を始点まで動かす
			obj.position.y = UserUtility::Lerp
			(
				obj.position.y,
				m_cloudState[obj.index].initPosition.y,
				CLOUD_SPEED * 0.5f
			);
			m_cloudState[obj.index].moveFlag = false;
		}
	}
}

// マップ選択
std::wstring Blocks::MapSelect(int num)
{
	std::wstring _filePath;

	// TODO: [ステージ番号]マップ追加はここから！
	// マップの変更　case ステージ番号
	switch (num)
	{
	case 0:
		_filePath = L"Datas/Maps/StageEdit.csv";
		break;
	case 1:
		_filePath = L"Datas/Maps/Stage1.csv";
		break;
	case 2:
		_filePath = L"Datas/Maps/Stage2.csv";
		break;
	case 3:
		_filePath = L"Datas/Maps/Stage3.csv";
		break;
	case 4:
		_filePath = L"Datas/Maps/Stage4.csv";
		break;
	case 5:
		_filePath = L"Datas/Maps/Stage5.csv";
		break;
	default:
		_filePath = L"NoStage";
		break;
	}

	return _filePath;
}

// マップの座標補正
void Blocks::MapSwipe()
{
	// 座標補正
	for (int i = 0; i < m_mapObj.size(); i++)
	{
		// インデックス番号の格納
		m_mapObj[i].index = i;

		// ブロックがないときは処理しない
		if (m_mapObj[i].id == MAPSTATE::NONE) continue;

		// マップの座標設定
		m_mapObj[i].position.x -= static_cast<float>(m_mapLoad->MAP_COLUMN) / 2 * COMMON_SIZE;
		m_mapObj[i].position.y += static_cast<float>(COMMON_LOW);
		m_mapObj[i].position.z -= static_cast<float>(m_mapLoad->MAP_COLUMN) / 2 * COMMON_SIZE;

		// コインの枚数のカウント
		if (m_mapObj[i].id == MAPSTATE::COIN)
		{
			m_maxCoins++;
		}
		// 雲フラグ登録
		if (m_mapObj[i].id == MAPSTATE::CLOUD)
		{
			// ステータス登録
			m_cloudState[i].moveFlag = false;

			// 始発点を保存
			m_cloudState[i].initPosition = m_mapObj[i].position;

			// 終着点を保存
			m_cloudState[i].endPosition = SimpleMath::Vector3
			{
				m_mapObj[i].position.x,
				m_mapObj[i].position.y + COMMON_SIZE + CLOUD_SIZE,
				m_mapObj[i].position.z
			};
		}
		// プレイヤの座標を代入
		if (m_mapObj[i].id == MAPSTATE::BIRD)
		{
			m_playerPos = SimpleMath::Vector3
			{
				m_mapObj[i].position.x,
				m_mapObj[i].position.y + COMMON_SIZE / 2,
				m_mapObj[i].position.z
			};

			// 代入後に該当マスをなしに変える(判定除去)
			m_mapObj[i].id = MAPSTATE::NONE;
		}
	}
}

// コンスタントバッファの作成
void Blocks::CreateConstBuffer()
{
	auto _device = DX::DeviceResources::GetInstance()->GetD3DDevice();
	// 定数バッファの作成
	D3D11_BUFFER_DESC _desc = {};
	// 注意：コンスタントバッファは１６の倍数であること
	_desc.ByteWidth = static_cast<UINT>(sizeof(CoinBuffer));
	// GPU (読み取り専用) と CPU (書き込み専用) の両方からアクセスできるリソース
	_desc.Usage = D3D11_USAGE_DYNAMIC;
	_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;	// 定数バッファとして扱う
	_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;	// CPUが内容を変更できるようにする
	DX::ThrowIfFailed(_device->CreateBuffer(&_desc, nullptr, m_coinBuffer.ReleaseAndGetAddressOf()));
}
