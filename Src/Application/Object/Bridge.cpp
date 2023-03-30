#include"../Headers.h"

Bridge::Bridge(GameScene* _gameScene,std::string _fileName,float _maxHeight,float _upSpeed)
{
	m_model = MODELMANAGER.GetModel(_fileName);
	m_mat = Math::Matrix::Identity;
	m_maxHeight = _maxHeight;
	m_upSpeed = _upSpeed;
	m_upHeight = 0;
	m_gameScene = _gameScene;
}

Bridge::~Bridge()
{
	
}

void Bridge::Update()
{
	const Chest& chest = m_gameScene->GetChest();
	if (chest.GetOpenFlg()==true)
	{
		m_upHeight += m_upSpeed;
		if(m_upHeight >= m_maxHeight)
		{
			m_upHeight = m_maxHeight;
		}
	}
	Math::Matrix transMat = DirectX::XMMatrixTranslation(0, m_upHeight, 0);
	m_mat = transMat;
}

bool Bridge::CheckBoard(const Math::Vector3& _rayPos, const Math::Vector3& _rayVec, float& _dis) const
{
	bool hit;
	hit = ModelIntersects(*m_model, m_mat, _rayPos, _rayVec, _dis);

	return hit;
}

