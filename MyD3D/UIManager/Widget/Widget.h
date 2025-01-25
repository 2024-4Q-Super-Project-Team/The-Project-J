#pragma once

class UIManager;

// 위젯 정렬 타입
enum class eAlignmentType
{
	CENTER,		// 중
	LEFT,		// 좌
	RIGHT,		// 우
};

// 위젯 타입
enum class eAssetType
{
	NON,	// 없음
	IMAGE,	// 이미지
	TEXT,	// 텍스트
	SHAPE,	// 도형
};

// 외곽선 정보
struct OutlineInfo
{
	Color color{};				// 외곽선 컬러
	FLOAT offset = 2.f;			// 외곽선 크기
	FLOAT alpha = 100.f;		// 외곽선 불투명도
};

class Widget
{
public:
	Widget();
	~Widget();
public:
	virtual void Init();
	virtual void PreUpdate();
	virtual void Update();
	virtual void Render(Vector2 _scale) = 0;
	virtual void Release();
public:
	// 기본 정보 설정
	void SetID(std::wstring _id)					{ mID = _id; }
	void SetFilepath(std::wstring _path)			{ mFilepath = _path; }
	void SetAssetType(eAssetType _type)				{ mAssetType = _type; }
	// 트랜스폼 정보 설정
	void SetPosition(Vector2 _position)				{ mPosition = _position; }
	void SetRotation(float _rotate)					{ mRotate = _rotate; }
	void SetSize(Vector2 _size)						{ mSize = _size; }
	// 그래픽 정보 설정
	void SetAlignment(eAlignmentType _type)			{ mAlignment = _type; }
	void SetAlpha(FLOAT _alpha)						{ mAlpha = Clamp(_alpha, 0.0f, 1.0f); }
	void SetColor(Color _color)						{ mColor = _color; }
	// 상태 정보 설정
	void SetActive(bool _active)					{ bIsActive = _active; }
	void SetOutline(bool _useOutline)				{ bUseOutline = _useOutline; }
	// 아웃라인 관련 설정
	void SetOutlineColor(Color _color)				{ mOutlineInfo.color = _color; }
	void SetOutlineOffset(float _offset)			{ mOutlineInfo.offset = _offset; }
	void SetOutlineAlpha(float _alpha)				{ mOutlineInfo.alpha = _alpha; }

	std::wstring	GetName()		{ return mID; }
	Vector2			GetSize()		{ return mSize; }
	bool			IsActive()		{ return bIsActive; }
	bool			UseOutline()	{ return bUseOutline; }
protected:
	std::wstring	mID = L"";

	std::wstring		mFilepath = L"";
	Texture2DResource*	m_pTexture = nullptr;

	eAssetType	mAssetType = eAssetType::NON;
	Vector2		mPosition{};
	Vector2		mSize{};
	float		mRotate = 0.f;
	Color		mColor{1,1,1};

	// 위젯 정렬 타입
	eAlignmentType	mAlignment = eAlignmentType::CENTER;
	// 외곽선 정보
	OutlineInfo		mOutlineInfo{};
	// 레이아웃 정보 (크기)

	float	mAlpha = 1.f;
	bool	bUseOutline = false;
	bool	bIsActive = true;
protected:
	// 범위 박스 그리기용 리소스
	Texture2DResource* m_pBox = nullptr;

	void DebugRectangleInit();
	void DebugRectangleRender(Color color, Vector2 _scale);
private:
};