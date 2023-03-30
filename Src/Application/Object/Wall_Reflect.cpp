#include"../Headers.h"

Wall_Reflect::Wall_Reflect()
{
	m_model = MODELMANAGER.GetModel("Data/Models/Map/Object/Wall_Reflect.gltf");
}

Wall_Reflect::~Wall_Reflect()
{
}

void Wall_Reflect::Update()
{
}

bool Wall_Reflect::CheckBoard(const Math::Vector3& _rayPos, const Math::Vector3& _rayVec, float& _dis, Math::Vector3& _normalVec) const
{
	bool hit;
	hit = ModelIntersects_IncludeNormal(*m_model, m_mat, _rayPos, _rayVec, _dis, _normalVec);

	return hit;
}
