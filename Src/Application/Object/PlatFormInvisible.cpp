#include"../Headers.h"

PlatFormInvisible::PlatFormInvisible()
{
	m_model = MODELMANAGER.GetModel("Data/Models/Map/Object/PlatFormInvisible.gltf");
	m_mat = Math::Matrix::Identity;
}

PlatFormInvisible::~PlatFormInvisible()
{
}

void PlatFormInvisible::Update()
{
}

void PlatFormInvisible::Draw()
{
	SHADER.m_standardShader.SetOpticalCamoEnable(true);
	SHADER.m_standardShader.SetWorldMatrix(m_mat);
	SHADER.m_standardShader.DrawModel(m_model);
	SHADER.m_standardShader.SetOpticalCamoEnable(false);
}

bool PlatFormInvisible::CheckBoard(const Math::Vector3& _rayPos, const Math::Vector3& _rayVec, float& _dis) const
{
	bool hit;
	hit = ModelIntersects(*m_model, m_mat, _rayPos, _rayVec, _dis);

	return hit;
}

