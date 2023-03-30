#pragma once
class Water;
class Spray :public Effect
{
public:
	Spray();
	~Spray();
	void Update()override;
	void Draw()override;

	void SetParameter(Math::Vector3 _pos);
private:

	Math::Vector3 m_pos;

	Vertex m_v[4]; 

	ID3D11BlendState* m_blendAlpha;
	ID3D11BlendState* m_blendAdd;  //‰ÁŽZ”¼“§–¾ 

	Water* m_water;
};
