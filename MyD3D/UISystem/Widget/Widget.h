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
	FLOAT Width = 10.0f;		// ����
	FLOAT Height = 10.0f;	// ����
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
	// ���� Ÿ��
	eAssetType mAssetType = eAssetType::NON;
	// ���� ������
	Vector2 mPosition{};
	// ���� ���� Ÿ��
	eAlignmentType mAlignment = eAlignmentType::CENTER;
	// �ܰ��� ����
	OutlineInfo mOutlineInfo{};
	
	// TODO :
	// ���� ���� �߰� ��׸�? ����?

	// ���̾ƿ� ���� (ũ��)
	// �������� ���� ������?
	Layout mLayout{};

	float mAlpha = 100.f;
	bool bUseOutline = false;
	bool bIsActive = true;
private:
};