#pragma once


class linkmaninfo
{
public:
	linkmaninfo(void);
	virtual ~linkmaninfo(void);
public:
	short  status ;
	std::string   temp;
	std::string   id;
	std::string   nickname;
	std::string   address;
	std::string   mobile;
 	std::string   contractor;
 	std::string   shopname;
 	bool	      mobilevalid;
 	bool		  realnamevalid;
	std::string   thumb; //用户对应的logo图片
	std::string   email;
	bool		  merchant;

};

class linkmaninfoEx:public linkmaninfo
{
public:
	linkmaninfoEx();
	~linkmaninfoEx();
public:
	UINT_PTR  m_pTag;
};

typedef linkmaninfo* PLINKMANINFO;

typedef std::map<std::string, PLINKMANINFO>  MAP_STRING_PLINKMANINFO ;