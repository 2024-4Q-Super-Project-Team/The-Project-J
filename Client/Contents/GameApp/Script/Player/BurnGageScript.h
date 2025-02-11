#pragma once

enum class eGaugeState
{
    NONE,
    PASSIVE,
    ACTIVE,
};

class BurnGageScript
	: public MonoBehaviour
{
public:
    _MONOBEHAVIOUR_INIT(BurnGageScript);

    void Start() override;
    void Update() override;
    void ProcessGauge();
public:
    eGaugeState mState = eGaugeState::NONE;
    bool bIsBurn = false;
private:
    Object* m_pPlayer = nullptr;
    Object* m_pBackground = nullptr;
    Object* m_pGauge = nullptr;
    Object* m_pText = nullptr;
};

