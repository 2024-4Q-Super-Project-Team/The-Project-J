#pragma once

class SavePointScript : public MonoBehaviour
{
public:
    _MONOBEHAVIOUR_INIT(SavePointScript);

    void Start() override;

    virtual void OnTriggerEnter(Collider* _origin, Collider* _destination) override;

    Vector3 GetSavePointPosition() const { return gameObject->transform->GetWorldPosition(); }
    int GetIndex() const { return mIndex.val; }

private:
    SerializeField(INT, mIndex, 0);
    AudioSource* mAudioSource;
public:
    static std::unordered_map<INT, SavePointScript*> TotalSavePointTable;
    static SavePointScript* GetSavePoint(INT _index) {
        auto itr = TotalSavePointTable.find(_index);
        if (FIND_SUCCESS(itr, TotalSavePointTable)) {
            return itr->second;
        }
        else return nullptr;
    }
public:
    virtual json Serialize() override;
    virtual void Deserialize(json& j) override;
};