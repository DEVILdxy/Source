#pragma once


class GameScene;
class ShootFire :public Effect
{
public:
	ShootFire(const Math::Vector3& _pos);
	~ShootFire();
	void Update()override;
	

private:

	Math::Vector3 m_pos;
	

};