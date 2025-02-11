#pragma once
class RotatingPlateScript : public MonoBehaviour
{
public:
	_MONOBEHAVIOUR_INIT(RotatingPlateScript);

public:
	virtual void Start() override;
	void Rotate();
};

