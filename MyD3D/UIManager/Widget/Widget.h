#pragma once

class UIManager;

// 위젯 정렬 타입
enum class eAlignmentType
{
	CENTER,		// 중
	LEFT,		// 좌
	RIGHT,		// 우
};

// 위젯 레이아웃 정보
struct Layout
{
	FLOAT Width		= 10.0f;		// 가로
	FLOAT Height	= 10.0f;		// 세로
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
	FLOAT scale = 2.f;			// 외곽선 크기
	FLOAT alpha = 100.f;		// 외곽선 불투명도
};

class Widget
{
public:
	Widget();
	~Widget();
public:
	virtual void Init();
	virtual void Update();
	virtual void Render() = 0;
	virtual void Release();
public:
	void SetID(std::wstring _id)					{ mID = _id; }
	void SetFilepath(std::wstring _path)			{ mFilepath = _path; }
	void SetAssetType(eAssetType _type)				{ mAssetType = _type; }
	void SetPosition(Vector2 _position)				{ mPosition = _position; }
	void SetRotation(float _rotate)					{ mRotate = _rotate; }
	void SetAlignment(eAlignmentType _type)			{ mAlignment = _type; }
	void SetAlpha(FLOAT _alpha)						{ mAlpha = Clamp(_alpha, 0.0f, 1.0f); }
	void SetColor(Color _color)						{ mColor = _color; }
	void SetLayout(FLOAT _width, FLOAT _height)		{ mLayout.Width = _width; mLayout.Height = _height; }
	void SetActive(bool _active)					{ bIsActive = _active; }
	void SetOutline(bool _useOutline)				{ bUseOutline = _useOutline; }

	std::wstring	GetName()		{ return mID; }
	bool			IsActive()		{ return bIsActive; }
	bool			UseOutline()	{ return bUseOutline; }
protected:
	std::wstring	mID = L"";

	SpriteBatch*	m_pSpriteBatch = nullptr;
	SpriteFont*		m_pSpriteFont = nullptr;

	std::wstring		mFilepath = L"";
	Texture2DResource*	m_pTexture = nullptr;

	eAssetType	mAssetType = eAssetType::NON;
	Vector2		mPosition{};
	float		mRotate = 0.f;
	Color		mColor{};

	// 위젯 정렬 타입
	eAlignmentType	mAlignment = eAlignmentType::CENTER;
	// 외곽선 정보
	OutlineInfo		mOutlineInfo{};
	// 레이아웃 정보 (크기)
	Layout			mLayout{};

	float	mAlpha = 100.f;
	bool	bUseOutline = false;
	bool	bIsActive = true;
private:
};