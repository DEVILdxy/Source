#pragma once
class TextureManager
{
public:
	KdTexture* GetTexture(std::string _fileName);

	void AllRelease();
	~TextureManager();

	static TextureManager& GetInstance()
	{
		static TextureManager instance;

		return instance;
	}

private:
	std::map<std::string, KdTexture*>m_textureList;

	TextureManager()
	{

	}

};
#define TEXTUREMANAGER TextureManager::GetInstance()