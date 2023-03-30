#include "../Headers.h"

ObjectManager::~ObjectManager()
{
	//➀　要素をdeleteする
	for (UINT i = 0; i < m_List.size(); i++)
	{
		delete m_List[i];
	}
	//②　枠を消す
	m_List.clear();
}

void ObjectManager::Update()
{
	for (UINT i = 0; i < m_List.size(); i++)
	{
		m_List[i]->Update();
	}
}

void ObjectManager::Draw()
{
	for (UINT i = 0; i < m_List.size(); i++)
	{
		m_List[i]->Draw();
	}
}

void ObjectManager::Add(Object* _obj)
{
	m_List.push_back(_obj);
}
