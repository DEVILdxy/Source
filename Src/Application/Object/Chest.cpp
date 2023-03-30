#include"../Headers.h"

Chest::Chest(GameScene* _gameScene)
{
	m_model = MODELMANAGER.GetModel("Data/Models/Map/Object/chest.gltf");
	m_mat = DirectX::XMMatrixTranslation(0, 0.5f, 0.5f);
	m_modelTop=MODELMANAGER.GetModel("Data/Models/Map/Object/chestTop.gltf");
	m_matTop = Math::Matrix::Identity;
	m_modelUpdate = MODELMANAGER.GetModel("Data/Models/Map/Object/Chest_Update.gltf");
	m_matUpdate= DirectX::XMMatrixTranslation(0, 0.5f, 0.5f);
	m_ang = 0;
	m_isOpen = false;
	m_gameScene = _gameScene;
}

Chest::~Chest()
{
	
}

void Chest::Update()
{
	if (m_gameScene->GetUI().GetOpenChest()==true) {
		m_ang += 1.0f;
		if (m_ang >= 60)
		{
			m_ang = 60;
			m_isOpen = true;
		}		
	}
	Math::Matrix transMat = DirectX::XMMatrixTranslation(0, 1, 1);
	Math::Matrix rotMat = DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(m_ang));
	m_matTop = rotMat * transMat;
	
}

void Chest::Draw()
{
	SHADER.m_standardShader.SetWorldMatrix(m_mat);
	SHADER.m_standardShader.DrawModel(m_model);

	SHADER.m_standardShader.SetWorldMatrix(m_matTop);
	SHADER.m_standardShader.DrawModel(m_modelTop);
}

bool Chest::CheckBoard(const Math::Vector3& _rayPos, const Math::Vector3& _rayVec, float& _dis, Math::Vector3& _normalVec) const
{
	bool hit;
	hit = ModelIntersects_IncludeNormal(*m_modelUpdate, m_matUpdate, _rayPos, _rayVec, _dis, _normalVec);

	return hit;
}