#pragma once
#include "linkmaninfo.h"
/*#include <json/json.h>*/
class ProfileResponse
{
public:
	~ProfileResponse(void);
	MAP_STRING_PLINKMANINFO  m_linkmenMap;
	void FromJson(const Json::Value& val);
	void EraseAndRelease(MAP_STRING_PLINKMANINFO::iterator it);
	static ProfileResponse* GetInstance();
private:
	class CGarbo
	{
	public:	
		~CGarbo()
		{
			SAFE_DELETE(ProfileResponse::m_pInstance);
		}
	};  
	static ProfileResponse* m_pInstance;
	static CGarbo Garbo;  //清理资源
	ProfileResponse(void);
	

};

