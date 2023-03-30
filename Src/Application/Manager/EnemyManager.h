#pragma once

class Enemy;

class EnemyManager
{
public:
	EnemyManager();
	~EnemyManager();
	void Update();
	void Draw();
	void Add(Enemy* _enemy);

	std::list<Enemy*>& GetEnemyList()
	{
		return m_list;
	}

private:

	std::list<Enemy*> m_list;

};