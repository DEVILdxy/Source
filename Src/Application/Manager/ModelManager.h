#pragma once

class ModelManager
{
public:
	KdModel* GetModel(std::string _fileName);
	//指定したファイル名の画像データが保存されているアドレスを返す

	void AllRelease();
	~ModelManager();

	static ModelManager& GetInstance()
	{
		static ModelManager instance;

		return instance;
	}

private:
	std::map<std::string, KdModel*>m_modelList;
	//検索キーの型、値の型

	ModelManager()
	{

	}


};


#define MODELMANAGER ModelManager::GetInstance()