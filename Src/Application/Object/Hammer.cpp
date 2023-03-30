#include"../Headers.h"

Hammer::Hammer(GameScene* _gameScene,float _speed,int _towards,float _range,Math::Vector3 _pos)
{
	m_model=MODELMANAGER.GetModel("Data/Models/Map/Object/Hammer.gltf");
	m_mat = Math::Matrix::Identity;
	m_gameScene = _gameScene;
	m_ang = 0;
	m_pushPow = 0;
	m_speed = _speed;
	m_towards = _towards;
	m_range = _range;
	m_pos = _pos;

	const float shadowSize = 6.0f;
	m_shadow = std::make_shared<Shadow>(_gameScene, shadowSize);
	
}

Hammer::~Hammer()
{
	
}

void Hammer::Update()
{
	m_tmpAng += m_speed;
	if (m_tmpAng > 360)
	{
		m_tmpAng -= 360;
	}
	m_ang = m_towards*(sin(DirectX::XMConvertToRadians(m_tmpAng))); // 1~-1
	m_ang *= m_range;
	Math::Matrix transMat = DirectX::XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z); 
	Math::Matrix rotMatX = DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(m_ang));
	m_mat = rotMatX * transMat;

	//Hammer‚Ì’†S‚ð•Ï‚¦‚é 
	Math::Matrix hammerTransMat = DirectX::XMMatrixTranslation(0, -71, 0);
	Math::Matrix tmpMat = hammerTransMat * m_mat;
	Math::Vector3 hammerPos;
	hammerPos = tmpMat.Translation();

	Push(hammerPos);

	m_shadow->Update(hammerPos);

}

void Hammer::Draw()
{
	SHADER.m_standardShader.SetWorldMatrix(m_mat);
	SHADER.m_standardShader.DrawModel(m_model);

	SHADER.m_effectShader.SetToDevice();
	m_shadow->Draw();

	SHADER.m_standardShader.SetToDevice();
}

void Hammer::Push(Math::Vector3 _pos)
{
	//‰~”»’è
	Player& player = m_gameScene->GetPlayer();
	Math::Vector3 v = player.GetPos() - _pos;
	if (v.Length() < 11.0f)
	{	//“–‚½‚Á‚Ä‚é		
		m_pushPow = 3.0f;
	}
	else
	{
		m_pushPow -= 0.1f;
		if (m_pushPow <= 0)
		{
			m_pushPow = 0;
		}
	}
	v.y = 0;
	v.Normalize();
	v *= m_pushPow;
//	player.FromHammerPow(v);
	player.AddPos(v);
	
}
