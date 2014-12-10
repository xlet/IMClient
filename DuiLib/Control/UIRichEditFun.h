#if !defined(__UI_RICH_EDIT_FUNCTION_H_)
#define __UI_RICH_EDIT_FUNCTION_H_

typedef union _charPos
{
	struct
	{
		WORD	wIndex;
		WORD	wLine;
	};
	int	nVal;
}EDIT_CHARPOS,*PEDIT_CHARPOS;

class StreamOutResult
{
public:
	explicit StreamOutResult( const DuiLib::tString* szMemory ) : m_bClear(false)
	{
		m_nObjectNum = -1;
		m_Images	 = 0;
		m_memory = const_cast<DuiLib::tString*>(szMemory);
	}
	StreamOutResult( ) : m_bClear(true)
	{
		m_memory = new DuiLib::tString;
	}
	~StreamOutResult()
	{
		m_nObjectNum = -1;
		if( m_bClear && m_memory )
		{
			delete m_memory;
			m_memory = 0;
		}
	}
	inline void appendValue(LPCTSTR pBytes,unsigned long dwLen)
	{
		if( m_memory )
			m_memory->append(pBytes,dwLen);
	}
	std::map<char*,DuiLib::tString>*	m_Images;
private:
	int								m_nObjectNum;
	DuiLib::tString*					m_memory;
	bool							m_bClear;
};

class EnumOleObjectByRange
{
public:
	virtual bool OnOleObject(IOleObject* pObject,const CLSID& pIID,long lPos) = 0;	
};

LPSTORAGE	CreateStorage();

void		CreateNewGUID(LPTSTR *pOut);

bool		IsPictureGuid(LPTSTR pPic);

DWORD CALLBACK StreamOutRtf(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb);

bool	getDropFilePath(IDataObject *pDataObj,vector<DuiLib::tString>& v);


bool	OleDataFormatPicEtc(DuiLib::CRichEditUI *pRe,LPCTSTR lpFilePath,LPTSTR pictureInfo,
	long lPos,DuiLib::atlCString& outVal);

void	EnumOleObjectByPos(IRichEditOle* pRe,CHARRANGE& pRange,EnumOleObjectByRange *pEnum);

void	EnumAllOleObject(IRichEditOle* pRe,EnumOleObjectByRange *pEnum);

#endif /*__UI_RICH_EDIT_FUNCTION_H_*/