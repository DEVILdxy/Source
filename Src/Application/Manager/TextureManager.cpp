#include"../Headers.h"

KdTexture* TextureManager::GetTexture(std::string _fileName)
{
	KdTexture* tmpTex;

	if (m_textureList.find(_fileName) == m_textureList.end())
	{
		tmpTex = new KdTexture;
		tmpTex->Load(_fileName);

		m_textureList[_fileName] = tmpTex;
	}
	else
	{
		tmpTex = m_textureList[_fileName];
	}

	return tmpTex;
}

void TextureManager::AllRelease()
{
	for (auto itr = m_textureList.begin(); itr != m_textureList.end(); itr++)
	{
		(itr->second)->Release();
		delete (itr->second);
	}
	m_textureList.clear();
}

TextureManager::~TextureManager()
{
	AllRelease();
}
