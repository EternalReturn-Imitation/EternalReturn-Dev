#pragma once
#include "CComponent.h"

class CText :
    public CComponent
{
private:
	// Text 렌더용 구조체
	union uniReferenceData
	{
		int		i;
		float	f;
	}typedef PrevRefData;

	enum class eRefDataType
	{
		INT,		// 정수형
		FLOAT_DP1,	// 실수형 소수점 1번째자리
		FLOAT_DP2,	// 실수형 소수점 2번째자리
	};

	struct tTextInfo
	{
		wstring				str;			// 출력할 문자열
		Vec2				vFontPos;		// 렌더위치
		Vec2				vOffsetPos;		// 오프셋위치
		wstring				Font;			// 폰트패밀리
		float				fSize;			// 글자크기
		UINT				FontColor;		// 글자색상
		UINT				Flags;			// 렌더플래그
		bool				bReference;		// 참조여부
		UINT				RefDataType;	// 참조하는 데이터 타입
		uniReferenceData	PrevRefData;	// 참조를 사용하고 있을 때 이전 입력한 데이터값
		
		DWORD_PTR			RefPtr;		// 참조데이터 포인터

		tTextInfo()
			: vFontPos{}
			, vOffsetPos{}
			, fSize(100.f)
			, FontColor(FONT_RGBA(255, 255, 255, 255))
			, Flags(1 | 4)	// FW1_CENTER | FW1_VCENTER
			, bReference(false)
			, RefDataType(0)
			, RefPtr(0)
			, PrevRefData{}
		{
			str = L"NODATA";
			Font = L"넥슨Lv2고딕";
		}
	};

public:
	enum class eTextRefType
	{
		WSTRING,	// 기본값 문자열
		INT,		// 정수형
		FLOAT_DP1,	// 소수점 첫재자리
		FLOAT_DP2,	// 소수점 둘째자리
	};

private:
	wstring								m_PrefabKey;		// Text 프리팹 사용한경우 
	vector<tTextInfo*>					m_vecTextInfo;		// 렌더용 텍스트 정보 벡터

public:
	virtual void finaltick() override;

	virtual void begin() override;
	virtual void tick() override;
	
	int AddText(Vec2 _vOffsetPos = Vec2(0.f, 0.f), float _fSize = 100.f, UINT _Color = FONT_RGBA(255, 255, 255, 255));
	bool SetReference(int _Textidx, UINT _RefDataType, DWORD_PTR _RefPtr);

	vector<tTextInfo*> GetTexts() { return m_vecTextInfo; }

	bool SetText(int _Textidx, const wchar_t* _str);
	bool SetOffset(int _Textidx, Vec2 _vOffset);
	bool SetSize(int _Textidx, float _size);
	bool SetColor(int _Textidx, UINT _Color);

	void render();

public:
	void SavePrefab(const wstring& _key);
	void LoadPrefab(const wstring& _key);

	virtual void SaveToLevelFile(FILE* _File);
	virtual void LoadFromLevelFile(FILE* _FILE);

    CLONE(CText);

public:
    CText();
    CText(const CText& _Origin);
    ~CText();

	friend class TextUI;
};

