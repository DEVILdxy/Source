#include "Headers.h"
enum ChangeMap {
	StartMap,
	NormalMap
};

Map::Map(GameScene* _gameScene)
{
	m_mat = DirectX::XMMatrixTranslation(0,0.5,0);
	//m_fname[2] = { "Data/Models/StartMap/StartScene.gltf","Data/Models/Map/Scene.gltf" };

	m_model=MODELMANAGER.GetModel("Data/Models/Map/Scene.gltf");
	
	//m_loadedMap = ChangeMap::StartMap;

	m_fence = new Fence();

	m_gameScene = _gameScene;
}

//void Map::Init(std::string _fname,int _mapNo)
//{
//	if (m_loadedMap != _mapNo)
//	{
//		m_mat = DirectX::XMMatrixTranslation(0, 0.5, 0);
//		m_model.Load(_fname);
//
//		m_loadedMap = _mapNo;
//	}
//	
//}

Map::~Map()
{
	delete m_fence;
}

void Map::Draw()
{
	SHADER.m_standardShader.SetUVTiling({ 4,4 });
	SHADER.m_standardShader.SetWorldMatrix(m_mat);
	SHADER.m_standardShader.DrawModel(m_model);
	SHADER.m_standardShader.SetUVTiling({ 1,1 });	

	m_fence->Draw();
}

void Map::Update()
{
	/*if (GetAsyncKeyState('1') & 0x8000)
	{
		Init("Data/Models/Map/Scene.gltf",ChangeMap::NormalMap);
	}*/
	m_fence->Update();

	Player& player = m_gameScene->GetPlayer();

	Math::Vector3 playerPos = player.GetPos();
	Math::Vector3 camPos = player.GetCamPos();

	// ���@����J�����ւ̕����x�N�g�����擾
	Math::Vector3 toCamVec = camPos - playerPos;
	toCamVec.Normalize();

	bool aimFlg = player.GetAimState();

	if (aimFlg == true)
	{
		float camDist = Math::Vector3(1.5f, 0.5f, -3).Length();// �Ə���ԁ@���@����J�����܂ł̋���


		CamMove(camDist, Math::Vector3(1.5f, 0.5f, -3), playerPos, toCamVec, player);
	}
	else
	{
		float camDist = Math::Vector3(0, 2, -5.5f).Length();// ���ʏ�� ���@����J�����܂ł̋���

		CamMove(camDist, Math::Vector3(0, 2, -5.5f),playerPos, toCamVec, player);
	}

	

	

}

void Map::CamMove(float _camDist, Math::Vector3& _camPos, Math::Vector3& _playerPos, Math::Vector3& _vec,Player& _player)
{

	// �J�����ƃ}�b�v�̓����蔻��
	float dist;
	if (ModelIntersects(*m_model, m_mat, _playerPos, _vec, dist) == true)
	{
		// ��������
		if (dist < _camDist) // ���@����J�����ւ̃x�N�g���͕ǂ܂ł̋��������@����J�����܂ł̋�����菬�����Ȃ�
		{
			// �J�����Ƃ̋�����␳
			Math::Vector3 newCamPos = _camPos;
			newCamPos.Normalize();

			newCamPos = newCamPos * dist * 0.9;

			// �ǂɓ�����ƃJ�������v���C���[�Ƌ߂�������
			// ������Ƃ��Ė{���̃J�����̋����Ƃ̍�����Y�ɉ��Z
		//	float tmpY = Math::Vector3(0, 5, -10).Length() - dist;
		//	newCamPos.y += tmpY;

			_player.SetCamPos(newCamPos);
		}
	}

}



bool Map::CheckBoard(const Math::Vector3& _rayPos, const Math::Vector3& _rayVec, float& _dis, Math::Vector3& _normalVec) const // �@������
{
	bool hit;
	hit = ModelIntersects_IncludeNormal(*m_model, m_mat, _rayPos, _rayVec, _dis, _normalVec);

	return hit;
}

bool Map::CheckBoard(const Math::Vector3& _rayPos, const Math::Vector3& _rayVec, float& _dis) const // �@���Ȃ�
{
	bool hit;
	hit = ModelIntersects(*m_model, m_mat, _rayPos, _rayVec, _dis);

	return hit;
}
