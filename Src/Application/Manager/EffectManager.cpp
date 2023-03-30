#include "../Headers.h"

EffectManager::EffectManager()
{
	m_list.clear();
}

EffectManager::~EffectManager()
{
	//‡@@—v‘f‚ðdelete‚·‚é
	for (auto obj:m_list)
	{
		delete obj;
	}
	//‡A@˜g‚ðÁ‚·
	m_list.clear();
	
}

void EffectManager::Update()
{
	// XV
	for (auto obj : m_list)
	{
		obj->Update();		
	}
	
	// íœ
	auto it = m_list.begin();
	while(it != m_list.end())
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

void EffectManager::Draw()
{
	for (auto obj : m_list)
	{
		obj->Draw();		
	}
}

void EffectManager::Add(Effect* _obj)
{
	m_list.push_back(_obj);
}



