#pragma once

class Enemy;
class AimCone :public Object
{
public:
	AimCone(Enemy* _enemy);
	~AimCone();
	void Update()override;
private:
	Enemy* m_enemy;

	float m_height;
	bool m_plusSwitch = true;
};