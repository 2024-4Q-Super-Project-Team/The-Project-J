#pragma once

class UIManager;

// ���� ���� Ÿ��
enum class eAlignmentType
{
	CENTER,		// ��
	LEFT,		// ��
	RIGHT,		// ��
};

// ���� Ÿ��
enum class eAssetType
{
	NON,	// ����
	IMAGE,	// �̹���
	TEXT,	// �ؽ�Ʈ
	SHAPE,	// ����
};

// �ܰ��� ����
struct OutlineInfo
{
	Color color{};				// �ܰ��� �÷�
	FLOAT offset = 2.f;			// �ܰ��� ũ��
	FLOAT alpha = 100.f;		// �ܰ��� ������
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
	// �⺻ ���� ����
	void SetID(std::wstring _id)					{ mID = _id; }
	void SetFilepath(std::wstring _path)			{ mFilepath = _path; }
	void SetAssetType(eAssetType _type)				{ mAssetType = _type; }
	// Ʈ������ ���� ����
	void SetPosition(Vector2 _position)				{ mPosition = _position; }
	void SetRotation(float _rotate)					{ mRotate = _rotate; }
	void SetSize(Vector2 _size)						{ mSize = _size; }
	// �׷��� ���� ����
	void SetAlignment(eAlignmentType _type)			{ mAlignment = _type; }
	void SetAlpha(FLOAT _alpha)						{ mAlpha = Clamp(_alpha, 0.0f, 1.0f); }
	void SetColor(Color _color)						{ mColor = _color; }
	// ���� ���� ����
	void SetActive(bool _active)					{ bIsActive = _active; }
	void SetOutline(bool _useOutline)				{ bUseOutline = _useOutline; }
	// �ƿ����� ���� ����
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

	// ���� ���� Ÿ��
	eAlignmentType	mAlignment = eAlignmentType::CENTER;
	// �ܰ��� ����
	OutlineInfo		mOutlineInfo{};
	// ���̾ƿ� ���� (ũ��)

	float	mAlpha = 1.f;
	bool	bUseOutline = false;
	bool	bIsActive = true;
protected:
	// ���� �ڽ� �׸���� ���ҽ�
	Texture2DResource* m_pBox = nullptr;

	void DebugRectangleInit();
	void DebugRectangleRender(Color color, Vector2 _scale);
private:
};