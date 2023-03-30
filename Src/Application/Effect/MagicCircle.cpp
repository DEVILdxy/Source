#include"../Headers.h"

MagicCircle::MagicCircle(Enemy* _enemy, Math::Vector3 _pos)
{
	m_tex = TEXTUREMANAGER.GetTexture("Data/Texture/magic1.png");
	

	Vertex v[4];
	v[0].pos = { -3,3,0 };
	v[1].pos = { 3,3,0 };
	v[2].pos = { -3,-3,0 };
	v[3].pos = { 3,-3,0 };

	v[0].color = { 1,1,1,1 };
	v[1].color = { 1,1,1,1 };
	v[2].color = { 1,1,1,1 };
	v[3].color = { 1,1,1,1 };

	v[0].uv = { 0,0 };
	v[1].uv = { 1,0 };
	v[2].uv = { 0,1 };
	v[3].uv = { 1,1 };

	D3D11_SUBRESOURCE_DATA srd = {};//�[���N���A(���g�S�ă[��)
	srd.pSysMem = v;//&v[0]�@���_�f�[�^�������Ă���擪�A�h���X

	m_vb.Create(D3D11_BIND_VERTEX_BUFFER, sizeof(v), D3D11_USAGE_DEFAULT, &srd);

	m_enemy = _enemy;

	m_ang = 0;

	m_size = { 1,1,1 };

	m_aliveFlg = true;

	const const float modelHeight = 1.0f;
	m_pos = { _pos.x,_pos.y - modelHeight,_pos.z };

	m_appearHeight = m_pos.y - 0.1f;
}

MagicCircle::~MagicCircle()
{
	delete m_tex;
	m_vb.Release();
}

void MagicCircle::Update()
{
	const int circleAngle = 360;             // �~����̊p�x
	const float heigthMin = m_pos.y - 0.1f;  // ���@�w�ł��Ⴂ����
	const float heigthMax = m_pos.y + 2.5f;	 // �ł���������
	const float moveSpeed = 0.05f;			 // ���@�w�ړ����x


	Math::Matrix rotMatX = DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(90));

	m_ang++;

	if (m_ang >= circleAngle)
	{
		m_ang -= circleAngle;
	}
	Math::Matrix rotMatY = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(m_ang));

	m_appearHeight += moveSpeed;
	if (m_appearHeight >= heigthMax)
	{
		m_appearHeight = heigthMax;
	}
	if (m_appearHeight > m_pos.y + 1.0f)
	{
		m_size -= {0.02f, 0, 0.02f};
	}
	if (m_size.x <= 0)
	{
		m_size = { 0,0,0 };
		m_aliveFlg = false;
	}
	Math::Matrix scalMat = DirectX::XMMatrixScaling(m_size.x, m_size.y, m_size.z);

	Math::Matrix transMat = DirectX::XMMatrixTranslation(m_pos.x, m_appearHeight, m_pos.z);
	m_mat = rotMatX * rotMatY * scalMat * transMat;
}


