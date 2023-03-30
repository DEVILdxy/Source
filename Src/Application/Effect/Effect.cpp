#include "../Headers.h"

void Effect::Update()
{
}

void Effect::Draw()
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	//使ってほしい頂点バッファ
	D3D.GetDevContext()->IASetVertexBuffers(0, 1, m_vb.GetAddress(), &stride, &offset);
	//頂点の利用法(プリミティブトポロジー)をセット 
	D3D.GetDevContext()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	//テクスチャをセット							　　
	D3D.GetDevContext()->PSSetShaderResources(0, 1, m_tex->GetSRViewAddress());
	//---------------------------
	//板ボリ表示
	SHADER.m_effectShader.SetWorldMatrix(m_mat);
	SHADER.m_effectShader.WriteToCB();//今までに設定した内容を反映
	//描画					  ↓何個使うか？
	D3D.GetDevContext()->Draw(4, 0);
}
