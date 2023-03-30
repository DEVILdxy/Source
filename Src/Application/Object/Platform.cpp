#include"../Headers.h"

Platform::Platform(GameScene* _gameScene)
{
	m_model=MODELMANAGER.GetModel("Data/Models/Map/Object/Platform.gltf");
	m_modelUpdate = MODELMANAGER.GetModel("Data/Models/Map/Object/PlatFormUpdate.gltf");
	m_pos = {};
	m_gameScene = _gameScene;
	m_dis = 0;
}

Platform::~Platform()
{
	
}

void Platform::Update()
{
	m_mat = DirectX::XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z);
}

void Platform::Draw()
{
	SHADER.m_standardShader.SetWorldMatrix(m_mat);
	SHADER.m_standardShader.DrawModel(m_model);
}

void Platform::SetPos(Math::Vector3 _pos)
{
	m_pos = _pos;
}


bool Platform::CheckBoard(const Math::Vector3& _rayPos, const Math::Vector3& _rayVec, float& _dis) const
{
	bool hit;
	hit = ModelIntersects(*m_modelUpdate, m_mat, _rayPos, _rayVec, _dis);

	return hit;
}