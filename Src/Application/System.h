#pragma once

class System
{
public:
	static System& GetInstance()
	{
		static System system;
		return system;
	}
	bool GetSpaceKeyFlg() { return m_spaceKeyFlg; }

	void SetSpaceKeyFlg(bool _flg) { m_spaceKeyFlg = _flg; }

	bool GetEscapeKeyFlg() { return m_escapeKeyFlg; }

	void SetEscapeKeyFlg(bool _flg) { m_escapeKeyFlg = _flg; }

private:
	System()
	{
	}
	bool m_spaceKeyFlg = false;
	bool m_escapeKeyFlg = false;
};

#define SYSTEM System::GetInstance()