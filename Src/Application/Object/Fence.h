#pragma once
class Fence :public Object
{
public:
	Fence();
	~Fence();
	void Update() override;
	const bool GetCloseFlg()const { return m_isClosed; }

	void uuu();

private:
	bool m_isClosed;

	float m_downPow;
};