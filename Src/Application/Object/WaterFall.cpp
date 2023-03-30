#include "../Headers.h"

WaterFall::WaterFall(GameScene* _gameScene)
{
	m_model=MODELMANAGER.GetModel("Data/Models/Map/Object/WaterFall.gltf");
	m_mat = Math::Matrix::Identity;

	m_gameScene = _gameScene;


	//==========
	m_sound = std::make_shared<KdSoundEffect>();
	m_sound->Load("Data/Sound/WaterFall.wav");
	m_soundInst = m_sound->CreateInstance(true);
	//                                             ��true�ɂ����3D�T�E���h
	//3D�T�E���h�̃p�����[�^���Z�b�g
	m_soundInst->SetCurveDistanceScaler(10.0f);//������
	//m_soundInst->Play3D(Math::Vector3(120,0,-115), true);
}

void WaterFall::InitSound()
{
	if (m_gameScene->GetChest().GetOpenFlg() == true)
	{
	}
}

WaterFall::~WaterFall()
{

}

void WaterFall::Update()
{
	m_uvOffset.x += 0.002f;

	if (GetAsyncKeyState('T') & 0x8000)
	{
		Spray* sp = new Spray();
		sp->SetParameter(Math::Vector3{ (float)(120+rand()%200),5,0});
		m_gameScene->GetEffectManager().Add(sp);
	}



	//���X�i�[�̐ݒ�i��{�I�Ƀv���C���[�̈ʒu�j
	//�ʒu�ݒ�
	AUDIO.GetListener().SetPosition(m_gameScene->GetPlayer().GetPos());
	//�����ݒ�
	AUDIO.GetListener().SetOrientation(
		SHADER.m_cb7_Camera.Work().mV.Backward(), //�J�����̕����x�N�g���i���ʃx�N�g��)
		SHADER.m_cb7_Camera.Work().mV.Up()		//�J�����̓��̕����x�N�g��
	);
	//�X�V
	m_soundInst->Apply3D();

	
}

void WaterFall::Draw()
{
	SHADER.m_standardShader.SetUVTiling({ 3,3 });

	SHADER.m_standardShader.SetUVOffset(m_uvOffset);

	SHADER.m_standardShader.SetWorldMatrix(m_mat);
	SHADER.m_standardShader.DrawModel(m_model);

	SHADER.m_standardShader.SetUVOffset({0,0});

	SHADER.m_standardShader.SetUVTiling({ 1,1 });
}
