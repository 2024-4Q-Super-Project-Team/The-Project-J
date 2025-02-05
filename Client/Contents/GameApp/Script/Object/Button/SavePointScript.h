#pragma once

class SavePointScript : public MonoBehaviour
{
public:
    _MONOBEHAVIOUR_INIT(SavePointScript);

    void Start() override;

    virtual void OnTriggerEnter(Collider* _origin, Collider* _destination) override;

    Vector3 GetSavePointPosition() const { return gameObject->transform->position; }
    int GetIndex() const { return mIndex; }
    void SetIndex(int index) { mIndex = index; }

private:
    int mIndex;
    bool mIsSaved = false; // 세이브가 된적 있는지?


};