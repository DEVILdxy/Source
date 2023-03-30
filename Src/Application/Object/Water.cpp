#include"../Headers.h"

Water::Water(GameScene* _gameScene)
{
	m_mat = DirectX::XMMatrixTranslation(0,-5,0);
	m_model=MODELMANAGER.GetModel("Data/Models/Map/Object/Ground.gltf");
	m_gameScene = _gameScene;

	//地面は水効果を付ける
	m_normalTex.Load("Data/Texture/water_normal.jpg");
	D3D.GetDevContext()->PSSetShaderResources(3, 1, m_normalTex.GetSRViewAddress());
	Math::Matrix rotMat = DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(90));
	SHADER.m_standardShader.SetNormalMat(rotMat);
}

Water::~Water()
{
	m_normalTex.Release();
}

void Water::Update()
{
	Math::Matrix transMat = DirectX::XMMatrixTranslation(0, 0.005f, 0);
	const Chest& chest = m_gameScene->GetChest();
	if (chest.GetOpenFlg() == true)
	{
		m_mat = transMat * m_mat;
	}

	//法線のUVを操作
	static Math::Vector2 offset = { 0,0 };
	offset.x += 0.0001f;
	offset.y += 0.0001f;
	if (offset.x > 1)
	{
		offset.x -= 1;
	}
	if (offset.y > 1)
	{
		offset.y -= 1;
	}

	SHADER.m_standardShader.SetNormalUVOffset(offset);

}

void Water::Draw()
{
	SHADER.m_standardShader.SetNormalEnable(true);
	SHADER.m_standardShader.SetWorldMatrix(m_mat);
	SHADER.m_standardShader.DrawModel(m_model,0.9f);
	SHADER.m_standardShader.SetNormalEnable(false);
}
