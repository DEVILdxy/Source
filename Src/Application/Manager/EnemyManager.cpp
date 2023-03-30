#include"../Headers.h"


EnemyManager::EnemyManager()
{
	m_list.clear();
}

EnemyManager::~EnemyManager()
{
	
	//�@�v�f��delete����
	for (auto obj : m_list)
	{
		delete obj;
	}
	//�@�g������
	m_list.clear();

	
}

void EnemyManager::Update()
{
	// �X�V
	for (auto obj : m_list)
	{
		obj->Update();
	}
	// �폜
	auto it = m_list.begin();
	while (it != m_list.end())
	{
		if ((*it)->GetAliveFlg() == false)
		{
			delete* it;
			it = m_list.erase(it);
		}
		else
		{
			++it;
		}
	}
	
}

void EnemyManager::Draw()
{
	for (auto obj : m_list)
	{
		obj->Draw();
	}
}

void EnemyManager::Add(Enemy* _enemy)
{
	m_list.push_back(_enemy);
}
