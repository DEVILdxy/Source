#include"../Headers.h"

KdModel* ModelManager::GetModel(std::string _fileName)
{
	KdModel* tmpModel;

	if (m_modelList.find(_fileName) == m_modelList.end())
	{
		tmpModel = new KdModel;
		tmpModel->Load(_fileName);
		m_modelList[_fileName] = tmpModel;		
	}
	else
	{
		tmpModel = m_modelList[_fileName];
	}

	return tmpModel;
}

void ModelManager::AllRelease()
{
	for (auto itr = m_modelList.begin(); itr != m_modelList.end(); itr++)
	{
		(itr->second)->Release();
		delete (itr->second);
	}
	m_modelList.clear();
}

ModelManager::~ModelManager()
{
	AllRelease();
}
