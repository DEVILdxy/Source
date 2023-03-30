#include"../Headers.h"

Crack::Crack(const Math::Matrix& _rotMatZ,const Math::Matrix& _motherMat)
{
	m_tex = TEXTUREMANAGER.GetTexture("Data/Texture/Crack.png");
	m_aliveFlg = true;
	Math::Matrix rotMatY = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(90));
	Math::Matrix transMat= DirectX::XMMatrixTranslation(-6.1f, 0, 0);

	m_mat = rotMatY * _rotMatZ * transMat * _motherMat;

	const float size = 6;
	Vertex v[4];
	v[0].pos = { -size,size,0 };
	v[1].pos = { size,size,0 };
	v[2].pos = { -size,-size,0 };
	v[3].pos = { size,-size,0 };

	v[0].color = { 1,1,1,1 };
	v[1].color = { 1,1,1,1 };
	v[2].color = { 1,1,1,1 };
	v[3].color = { 1,1,1,1 };

	v[0].uv = { 0,0 };
	v[1].uv = { 1,0 };
	v[2].uv = { 0,1 };
	v[3].uv = { 1,1 };

	D3D11_SUBRESOURCE_DATA srd = {};//ゼロクリア(中身全てゼロ)
	srd.pSysMem = v;//&v[0]　頂点データが入っている先頭アドレス

	m_vb.Create(D3D11_BIND_VERTEX_BUFFER, sizeof(v), D3D11_USAGE_DEFAULT, &srd);
}

Crack::~Crack()
{
	delete m_tex;
	m_vb.Release();
}

void Crack::Update()
{
	if (GetAsyncKeyState('3') & 0x8000)
	{
		m_aliveFlg = false;
	}
}
