#include "../Headers.h"

Fence::Fence()
{
	m_model=MODELMANAGER.GetModel("Data/Models/Map/Object/Fence.gltf");
	m_mat = Math::Matrix::Identity;
	m_downPow = 0;
	m_isClosed = false;
}

Fence::~Fence()
{
	
}

void Fence::Update()
{
	m_downPow -= 0.05f;
	if (m_downPow <= -6)
	{
		m_downPow = -6;
		m_isClosed = true;
	}
	m_mat = DirectX::XMMatrixTranslation(0, m_downPow, 0);
}

