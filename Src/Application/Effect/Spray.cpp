#include"../Headers.h"

Spray::Spray()
{
	m_tex = TEXTUREMANAGER.GetTexture("Data/Texture/Cloud.bmp");

	m_v[0].uv = { 0,0 };
	m_v[1].uv = { 1,0 };
	m_v[2].uv = { 0,1 };
	m_v[3].uv = { 1,1 };

	m_mat = DirectX::XMMatrixTranslation(0, 0, 0);

	m_pos = {};


	//加算半透明初期化
	m_blendAlpha = D3D.CreateBlendState(KdBlendMode::Alpha);
	m_blendAdd = D3D.CreateBlendState(KdBlendMode::Add);

	m_aliveFlg = true;

	//m_water =_water;
}

Spray::~Spray()
{
	m_vb.Release();

	m_blendAlpha->Release();
	m_blendAdd->Release();

}

void Spray::Update()
{
	//ビルボード作成
	//====================
	Math::Matrix tmpMat = SHADER.m_cb7_Camera.Work().mV; //カメラの行列を取得(View行列)

	//カメラの座標は不要なので無効にしたい
	tmpMat._41 = 0; //X
	tmpMat._42 = 0; //Y
	tmpMat._43 = 0; //Z

	//逆行列に変換
	tmpMat = tmpMat.Invert();

	//===================================
	//Math::Matrix waterMat = m_water->GeMat();
	Math::Matrix transMat = DirectX::XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z);
	m_mat =tmpMat* transMat;
	static float tmpPos = 0;
	static float tmpAlpha = 0;

	tmpPos += 0.01f;
	tmpAlpha -= 0.001f;
	const int size = 10; // 曇りのサイズ
	const float alpha = 0.6f;

	m_v[0].pos = { -size - tmpPos,size + tmpPos,0 };
	m_v[1].pos = { size + tmpPos,size + tmpPos,0 };
	m_v[2].pos = { -size - tmpPos,-size - tmpPos,0 };
	m_v[3].pos = { size + tmpPos,-size - tmpPos,0 };

	m_v[0].color = { 1,1,1,alpha + tmpAlpha };
	m_v[1].color = { 1,1,1,alpha + tmpAlpha };
	m_v[2].color = { 1,1,1,alpha + tmpAlpha };
	m_v[3].color = { 1,1,1,alpha + tmpAlpha };

	if ((alpha + tmpAlpha) <= 0.0f) // 曇りのalpha値は0の時
	{
		tmpAlpha = 0;
		tmpPos = 0;
		m_aliveFlg = false;
	}

	D3D11_SUBRESOURCE_DATA srd = {};//ゼロクリア(中身全てゼロ)
	srd.pSysMem = m_v;//&v[0]　頂点データが入っている先頭アドレス
	m_vb.Create(D3D11_BIND_VERTEX_BUFFER, sizeof(m_v), D3D11_USAGE_DEFAULT, &srd);

	
}

void Spray::Draw()
{	//zバッファへの書き込みをしない
	D3D.GetDevContext()->OMSetDepthStencilState(
		D3D.CreateDepthStencilState(true, false), 0);

	//加算半透明に変更
	D3D.GetDevContext()->OMSetBlendState(m_blendAdd, Math::Color(0, 0, 0, 0), 0xFFFFFFFF);


	//板ポリ============================================
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	//使ってほしい頂点バッファ
	D3D.GetDevContext()->IASetVertexBuffers(0, 1, m_vb.GetAddress(), &stride, &offset);
	//頂点の利用法(プリミティブトポロジー)をセット 原始拓?
	D3D.GetDevContext()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	//テクスチャをセット							　　
	D3D.GetDevContext()->PSSetShaderResources(0, 1, m_tex->GetSRViewAddress());
	//---------------------------
	//板ボリ表示
	SHADER.m_effectShader.SetWorldMatrix(m_mat);
	SHADER.m_effectShader.WriteToCB();//今までに設定した内容を反映
	//描画					  ↓何個使うか？
	D3D.GetDevContext()->Draw(4, 0);



	D3D.GetDevContext()->OMSetBlendState(m_blendAlpha, Math::Color(0, 0, 0, 0), 0xFFFFFFFF);

	//加算合成OFF

	//用がなくなったら元に戻す
	D3D.GetDevContext()->OMSetDepthStencilState(
		D3D.CreateDepthStencilState(true, true), 0);


	


}

void Spray::SetParameter(Math::Vector3 _pos)
{
	m_pos = _pos;
}
