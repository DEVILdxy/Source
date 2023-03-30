#include "../Headers.h"

Door_Left::Door_Left(GameScene* _gameScene)
{
	m_model=MODELMANAGER.GetModel("Data/Models/Map/Object/Door_left.gltf");
	m_mat = Math::Matrix::Identity;
	m_ang = 0;
	m_gameScene = _gameScene;
}

Door_Left::~Door_Left()
{
	
}

void Door_Left::Update()
{
	const float toPlayerDis = 30;
	const float minAng = -90;
	Math::Vector3 vec = m_gameScene->GetPlayer().GetPos() - m_mat.Translation();
	if (vec.Length() <= toPlayerDis)
	{
		m_ang -= 1.0f;
		if (m_ang <= minAng)
		{
			m_ang = minAng;
		}
		Math::Matrix rotMat;
		rotMat = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(m_ang));
		Math::Matrix transMat = DirectX::XMMatrixTranslation(-7.1f, 0, -172);
		m_mat = rotMat * transMat;
	}
	else
	{
		m_ang += 1.0f;
		if (m_ang >= 0)
		{
			m_ang = 0;
		}
		Math::Matrix rotMat;
		rotMat = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(m_ang));
		Math::Matrix transMat = DirectX::XMMatrixTranslation(-7.1f, 0, -172);
		m_mat = rotMat * transMat;
	}
}
