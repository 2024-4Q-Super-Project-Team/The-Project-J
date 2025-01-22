#pragma once

class UISystem;

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
	FLOAT Width = 10.0f;		// 가로
	FLOAT Height = 10.0f;	// 세로
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
	FLOAT scale = 1.f;			// 외곽선 크기
	FLOAT alpha = 100.f;		// 외곽선 불투명도
};

class Widget
{
public:
	Widget();
	~Widget();
public:
	virtual void Start();
	virtual void FixedUpdate();
	virtual void PreUpdate();
	virtual void Update() = 0;
	virtual void PostUpdate();
	virtual void PreRender();
	virtual void Render() = 0;
	virtual void PostRender();
public:
	void SetEID(std::string _id)						{ mID = _id; };
	void SetOwner(UISystem* _owner)					{ m_pUISystem = _owner; };
	void SetAssetType(eAssetType _type)				{ mAssetType = _type; };
	void SetPosition(Vector2 _position)				{ mPosition = _position; };
	void SetAlignment(eAlignmentType _type)			{ mAlignment = _type; };
	void SetAlpha(FLOAT _alpha)						{ mAlpha = Clamp(_alpha, 0.0f, 1.0f); };
	void SetLayout(FLOAT _width, FLOAT _height)		{ mLayout.Width = _width; mLayout.Height = _height; };
	void SetActive(bool _active)					{ bIsActive = _active; };
	void SetOutline(bool _useOutline)				{ bUseOutline = _useOutline; };

	std::string		GetName()		{ return mID; };
	bool			IsActive()		{ return bIsActive; };
	bool			UseOutline()	{ return bUseOutline; };
public:
	UISystem* m_pUISystem = nullptr;
	std::string mID = "";
protected:
	// 위젯 타입
	eAssetType mAssetType = eAssetType::NON;
	// 위젯 포지션
	Vector2 mPosition{};
	// 위젯 정렬 타입
	eAlignmentType mAlignment = eAlignmentType::CENTER;
	// 외곽선 정보
	OutlineInfo mOutlineInfo{};
	
	// TODO :
	// 각도 변수 추가 디그리? 라디안?

	// 레이아웃 정보 (크기)
	// 포지션을 같이 넣을까?
	Layout mLayout{};

	float mAlpha = 100.f;
	bool bUseOutline = false;
	bool bIsActive = true;
private:
};