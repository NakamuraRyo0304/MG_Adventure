/*
 *	@File	Blocks.cpp
 *	@Brief	マップのブロック管理。
 *	@Date	2023-07-11
 *  @Author NakamuraRyo
 */

#include "pch.h"

// CSV読み込み
#include "../../../Libraries/SystemDatas/MapLoad.h"

// モデルファクトリ
#include "../../../Libraries/Factories/ModelFactory.h"

// ユーザーユーティリティ
#include "../../../Libraries/UserUtility.h"

#include "Blocks.h"

// コンストラクタ
Blocks::Blocks()
	: m_coinCount{0}							// コインカウンタ
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
		if (m_mapObj[i].id == MAPSTATE::PLAYER)
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

	// コイン回収済みフラグをFalseにする
	is_collectedFlag = false;
	// 当たり判定フラグをFalseにする
	is_hitCoinFlag = false;

	// ライティングのリセット
	m_lighting = SimpleMath::Vector3::Zero;
}

// 更新処理
void Blocks::Update()
{
	// 雲は上下移動する
	for(int i = 0; i < m_mapObj.size();++i)
	{
		if (m_mapObj[i].id == MAPSTATE::CLOUD)
		{
			// 触れているときは終点まで
			if (m_cloudState[i].moveFlag)
			{
				// Y座標を終点まで動かす
				m_mapObj[i].position.y = UserUtility::Lerp
				(
					m_mapObj[i].position.y,
					m_cloudState[i].endPosition.y,
					CLOUD_SPEED
				);
			}
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
	SimpleMath::Matrix _world, _rotMat, _verticle;

	// ライティング設定
	std::function<void(IEffect* effect)> _lightSetting;

	// 雲以外のオブジェクトの描画
	for (int i = 0; i < m_mapObj.size(); i++)
	{
		if (m_mapObj[i].id == MAPSTATE::NONE) continue;

		// 移動行列
		_world = SimpleMath::Matrix::CreateTranslation(m_mapObj[i].position);

		// 回転行列
		_rotMat = SimpleMath::Matrix::CreateRotationY(timer);

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

		// 草ブロック
		if (m_mapObj[i].id == MAPSTATE::GRASS)
		{
			m_grassModel->UpdateEffects(_lightSetting);
			m_grassModel->Draw(_context, states, _world, view, proj);
		}

		// コインブロック
		if (m_mapObj[i].id == MAPSTATE::COIN)
		{
			m_coinModel->UpdateEffects(_lightSetting);
			m_coinModel->Draw(_context, states, _rotMat * _world, view, proj);
		}

		// 重力ブロック
		if (m_mapObj[i].id == MAPSTATE::GRAVITY)
		{
			m_gravityModel->UpdateEffects(_lightSetting);
			m_gravityModel->Draw(_context, states, _world, view, proj);
		}
	}

	// 雲ブロック
	for (int i = 0; i < m_mapObj.size(); i++)
	{
		if (m_mapObj[i].id != MAPSTATE::CLOUD) continue;

		// 移動行列
		_world = SimpleMath::Matrix::CreateTranslation(m_mapObj[i].position);

		m_cloudModel->Draw(_context, states, _rotMat * _world, view, proj, false, [&]()
			{
				_context->OMSetBlendState(states.NonPremultiplied(), nullptr, 0xffffffff);
				_context->OMSetDepthStencilState(states.DepthDefault(), 0);
				_context->PSSetShader(m_psCloud.Get(), nullptr, 0);
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
	if (!m_gravityModel) return;

	ModelFactory::DeleteModel(m_grassModel);
	ModelFactory::DeleteModel(m_coinModel);
	ModelFactory::DeleteModel(m_cloudModel);
	ModelFactory::DeleteModel(m_gravityModel);
}

// モデル作成関数
void Blocks::CreateModels(std::unique_ptr<Model> model,int modelNum)
{
	switch (modelNum)
	{
	case GRASS:									// 草ブロック
		m_grassModel = std::move(model);
		break;
	case COIN:									// コイン
		m_coinModel = std::move(model);
		break;
	case CLOWD:									// 雲ブロック
		m_cloudModel = std::move(model);
		break;
	case GRAVITY:								// 重力ブロック
		m_gravityModel = std::move(model);
		break;
	default:
		break;
	}
}

// シェーダー作成
void Blocks::CreateShader()
{
	auto _device = DX::DeviceResources::GetInstance()->GetD3DDevice();

	// ピクセルシェーダー
	std::vector<uint8_t> _psCloud = DX::ReadData(L"Resources/Shaders/PS_Cloud.cso");
	DX::ThrowIfFailed(
		_device->CreatePixelShader(_psCloud.data(), _psCloud.size(), nullptr,
			m_psCloud.ReleaseAndGetAddressOf()));
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
	for (auto& i : m_mapObj)
	{
		// 雲のみを対象とする
		if (i.id == MAPSTATE::CLOUD)
		{
			// Y座標を始点まで動かす
			i.position.y = UserUtility::Lerp
			(
				i.position.y,
				m_cloudState[i.index].initPosition.y,
				CLOUD_SPEED * 0.5f
			);
			m_cloudState[i.index].moveFlag = false;
		}
	}
}

// マップ選択
std::wstring Blocks::MapSelect(int num)
{
	std::wstring _filePath;

	// TODO: [ステージ番号]マップ追加はここから！
	// マップの変更
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