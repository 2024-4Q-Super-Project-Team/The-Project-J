#pragma once

class UISystem;

// ���� ���� Ÿ��
enum class eAlignmentType
{
	CENTER,		// ��
	LEFT,		// ��
	RIGHT,		// ��
};

// ���� ���̾ƿ� ����
struct Layout
{
	FLOAT Width		= 10.0f;		// ����
	FLOAT Height	= 10.0f;		// ����
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
	FLOAT scale = 1.f;			// �ܰ��� ũ��
	FLOAT alpha = 100.f;		// �ܰ��� ������
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
	virtual void Update();
	virtual void PostUpdate();
	virtual void PreRender();
	virtual void Render() = 0;
	virtual void PostRender();
public:
	void SetID(std::wstring _id)					{ mID = _id; };
	void SetFilepath(std::wstring _path)			{ mFilepath = _path; }
	void SetOwner(UISystem* _owner)					{ m_pUISystem = _owner; };
	void SetAssetType(eAssetType _type)				{ mAssetType = _type; };
	void SetPosition(Vector2 _position)				{ mPosition = _position; };
	void SetRotation(float _rotate)					{ mRotate = _rotate; };
	void SetAlignment(eAlignmentType _type)			{ mAlignment = _type; };
	void SetAlpha(FLOAT _alpha)						{ mAlpha = Clamp(_alpha, 0.0f, 1.0f); };
	void SetLayout(FLOAT _width, FLOAT _height)		{ mLayout.Width = _width; mLayout.Height = _height; };
	void SetActive(bool _active)					{ bIsActive = _active; };
	void SetOutline(bool _useOutline)				{ bUseOutline = _useOutline; };

	std::wstring	GetName()		{ return mID; };
	bool			IsActive()		{ return bIsActive; };
	bool			UseOutline()	{ return bUseOutline; };
public:
	UISystem* m_pUISystem = nullptr;
	std::wstring mID = L"";
protected:
	std::wstring mFilepath = L"";
	Texture2DResource* mTexture = nullptr;

	eAssetType mAssetType = eAssetType::NON;
	Vector2 mPosition{};
	float mRotate = 0.f;

	// ���� ���� Ÿ��
	eAlignmentType mAlignment = eAlignmentType::CENTER;
	// �ܰ��� ����
	OutlineInfo mOutlineInfo{};
	// ���̾ƿ� ���� (ũ��)
	// �������� ���� ������?
	Layout mLayout{};

	float mAlpha = 100.f;
	bool bUseOutline = false;
	bool bIsActive = true;
private:
};