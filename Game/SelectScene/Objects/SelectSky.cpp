/*
 *	@File	SelectSky.cpp
 *	@Brief	セレクトシーンのスカイドーム。
 *	@Date	2023-11-06
 *  @Author NakamuraRyo
 */

#include "pch.h"

// ファクトリマネージャ
#include "Libraries/FactoryManager/FactoryManager.h"

#include "SelectSky.h"

// コンストラクタ
SelectSky::SelectSky(std::shared_ptr<FactoryManager> fm, const wchar_t* path)
{
	fm->BuildModelFactory();
	m_model = fm->VisitModelFactory()->GetCreateModel(path);
	fm->LeaveModelFactory();

	m_model->UpdateEffects([](IEffect* effect)
		{
			auto _lights = dynamic_cast<IEffectLights*>(effect);
			if (_lights)
			{
				_lights->SetLightEnabled(0, false);
				_lights->SetLightEnabled(1, false);
				_lights->SetLightEnabled(2, false);
			}
			// 自己発光する
			auto _basicEffect = dynamic_cast<BasicEffect*>(effect);
			if (_basicEffect)
			{
				_basicEffect->SetEmissiveColor(Colors::White);
			}
		}
	);
}

// デストラクタ
SelectSky::~SelectSky()
{
}

// 描画関数
void SelectSky::Draw(CommonStates& states,
	const SimpleMath::Matrix& view, const SimpleMath::Matrix& proj, const float& timer)
{
	// 回転させる
	auto _context = DX::DeviceResources::GetInstance()->GetD3DDeviceContext();
	SimpleMath::Matrix skyMat = SimpleMath::Matrix::CreateRotationY(timer * ROT_SPEED);
	m_model->Draw(_context, states, skyMat, view, proj);
}