#pragma once
class WidgetCheckBox 
	: public Widget
{
public:
	WidgetCheckBox();
	virtual ~WidgetCheckBox(); // 버츄얼로 돌려야되는데 왜 안됨?

public:
	virtual void Update()	override;
	virtual void Update()	override;
	virtual void Render()	override;
public:
	bool IsCheck() { return bIsCheck; }
	bool OnClick() { return bIsCheck && Input::IsMouseDown(Mouse::LEFT); }
	bool OnExit() { return !bIsCheck && Input::IsMouseDown(Mouse::LEFT); }
private:
	bool bIsCheck = false;
};

