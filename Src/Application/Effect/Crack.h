#pragma once
class Crack :public Effect
{
public:
	Crack(const Math::Matrix& _rotMatZ,const Math::Matrix& _motherMat);
	~Crack();

	void Update()override;

private:


};