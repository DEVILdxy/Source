#include"../Headers.h"

AimCone::AimCone(Enemy* _enemy)
{
	m_model = MODELMANAGER.GetModel("Data/Models/Enemy/AimCone.gltf");
	m_mat = Math::Matrix::Identity;
	m_enemy = _enemy;
	m_height = 1.5f;
}

AimCone::~AimCone()
{
}

void AimCone::Update()
{
	const float minHeight = 1.5f;
	const float maxHeight = 3.0f;



	if (m_height <= minHeight)
	{
		m_plusSwitch = true;
	}
	if (m_height >= maxHeight)
	{
		m_plusSwitch = false;
	}

	if (m_plusSwitch == true)
	{
		m_height += 0.05f;
	}
	else if (m_plusSwitch == false)
	{
		m_height -= 0.05f;
	}


	Math::Matrix transMat = DirectX::XMMatrixTranslation(0, m_height, 0);
	m_mat = transMat * m_enemy->GetMat();


}

