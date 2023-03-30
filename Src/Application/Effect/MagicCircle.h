#pragma once

class Enemy;

class MagicCircle:public Effect
{
public:
	MagicCircle(Enemy* _enemy,Math::Vector3 _pos);
	~MagicCircle();
	void Update()override;




private:

	Enemy* m_enemy;

	float m_ang;

	Math::Vector3 m_size ;

	Math::Vector3 m_pos;

	float m_appearHeight;

};