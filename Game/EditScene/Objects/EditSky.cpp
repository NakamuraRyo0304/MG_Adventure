/*
 *	@File	EditSky.cpp
 *	@Brief	エディタシーンのスカイドーム。
 *	@Date	2023-11-09
 *  @Author NakamuraRyo
 */

#include "pch.h"
#include "Libraries/FactoryManager/FactoryManager.h"
#include "EditSky.h"

// コンストラクタ
EditSky::EditSky(std::shared_ptr<FactoryManager> fm, const wchar_t* path)
{
	fm->BuildModelFactory();
	m_model = fm->VisitModelFactory()->GetCreateModel(path);
	fm->LeaveModelFactory();

	m_model->UpdateEffects([](IEffect* effect)
		{
			auto _lights = dynamic_cast<IEffectLights*>(effect);
			if (_lights)
			{
				// ライトの数分回す
				for (int i = 0; i < 3; ++i)
				{
					_lights->SetLightEnabled(i, false);
				}
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
EditSky::~EditSky()
{
}

// 描画関数
void EditSky::Draw(CommonStates& states,
	const SimpleMath::Matrix& view, const SimpleMath::Matrix& proj, const float& timer)
{
	auto _context = DX::DeviceResources::GetInstance()->GetD3DDeviceContext();

	SimpleMath::Matrix world = SimpleMath::Matrix::CreateRotationY(timer * ROT_SPEED);
	m_model->Draw(_context, states, world, view, proj);
}
