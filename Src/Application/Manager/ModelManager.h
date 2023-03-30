#pragma once

class ModelManager
{
public:
	KdModel* GetModel(std::string _fileName);
	//�w�肵���t�@�C�����̉摜�f�[�^���ۑ�����Ă���A�h���X��Ԃ�

	void AllRelease();
	~ModelManager();

	static ModelManager& GetInstance()
	{
		static ModelManager instance;

		return instance;
	}

private:
	std::map<std::string, KdModel*>m_modelList;
	//�����L�[�̌^�A�l�̌^

	ModelManager()
	{

	}


};


#define MODELMANAGER ModelManager::GetInstance()