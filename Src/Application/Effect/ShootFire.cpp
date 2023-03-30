#include "../Headers.h"

ShootFire::ShootFire(const Math::Vector3& _pos)
{
	m_tex=TEXTUREMANAGER.GetTexture("Data/Texture/Smoke.png");
	m_mat = Math::Matrix::Identity;
	m_pos = _pos;
	m_anime = 0;
	m_aliveFlg = true;

	
}

ShootFire::~ShootFire()
{
	delete m_tex;
	m_vb.Release();
}

void ShootFire::Update()
{
	Vertex v[4];
	v[0].pos = { -0.8,0.8,0 };
	v[1].pos = { 0.8,0.8,0 };
	v[2].pos = { -0.8,-0.8,0 };
	v[3].pos = { 0.8,-0.8,0 };

	v[0].color = { 1,1,1,1 };
	v[1].color = { 1,1,1,1 };
	v[2].color = { 1,1,1,1 };
	v[3].color = { 1,1,1,1 };

	m_anime++;
	if (m_anime >= 64) {
		m_anime = 0;
		m_aliveFlg = false;
	}

	//           ���\���������摜�ԍ�
	float W1 = (m_anime % 8) * 32.0f / m_tex->GetWidth();
	//                   �����ɕ���ł���摜�̐�
	float H1 = (m_anime / 8) * 32.0f / m_tex->GetHeight();
	float W2 = (m_anime % 8 + 1) * 32.0f / m_tex->GetWidth();
	float H2 = (m_anime / 8 + 1) * 32.0f / m_tex->GetHeight();

	v[0].uv = { W1,H1 };
	v[1].uv = { W2,H1 };
	v[2].uv = { W1,H2 };
	v[3].uv = { W2,H2 };

	D3D11_SUBRESOURCE_DATA srd = {};//�[���N���A(���g�S�ă[��)
	srd.pSysMem = v;//&v[0]�@���_�f�[�^�������Ă���擪�A�h���X

	m_vb.Create(D3D11_BIND_VERTEX_BUFFER, sizeof(v), D3D11_USAGE_DEFAULT, &srd);

	//�r���{�[�h�쐬
	//====================
	Math::Matrix tmpMat = SHADER.m_cb7_Camera.Work().mV; //�J�����̍s����擾(View�s��)���擾

	//�J�����̍��W�͕s�v�Ȃ̂Ŗ����ɂ�����
	tmpMat._41 = 0; //X
	tmpMat._42 = 0; //Y
	tmpMat._43 = 0; //Z

	//�t�s��ɕϊ�
	tmpMat = tmpMat.Invert();

	//===================================
	Math::Matrix transMat = DirectX::XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z);
	m_mat = tmpMat * transMat;
}


