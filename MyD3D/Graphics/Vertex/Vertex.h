#pragma once

struct Vertex
{
    Vertex() = default;
    Vertex(Vector4 _position, Vector4 _color, Vector3 _normal)
        : Position(_position), Color(_color), Normal(_normal){
    }
    Vertex(Vector4 _position, Vector4 _color, Vector3 _normal, Vector3 _tangent)
        : Position(_position), Color(_color), Normal(_normal), Tangent(_tangent) {}

    // 정점의 좌표
    Vector4 Position = { 0,0,0,1 };
    // 정점의 색상
    Vector4 Color = { 1.0f,1.0f,1.0f,1.0f };
    // 정점 노말 값
    Vector3 Normal = { 0,0,0 };
    // 정점 탄젠트
    Vector3 Tangent = { 0,0,0 };
    Vector3 BiTangent = { 0,0,0 };
    // 정점 텍스쳐 좌표
    Vector2 TexCoord = { 0,0 };
    // 정점의 소속 메쉬로부터의 본테이블 참조 인덱스 (-1은 적용받는 본이 없다는 뜻)
    INT		BoneIndices[4] = { 0,0,0,0 };
    // 정점의 본매트릭스 웨이트 가중치 (0.0 ~ 1.0)
    FLOAT	BoneWeights[4] = { 0.0f,0.0f,0.0f,0.0f };

    void AddBoneData(UINT _index, float _weight)
    {
        assert(BoneWeights[0] == 0.0f || BoneWeights[1] == 0.0f
            || BoneWeights[2] == 0.0f || BoneWeights[3] == 0.0f);
        for (int i = 0; i < 4; ++i)
        {
            if (BoneWeights[i] == 0.0f)
            {
                BoneIndices[i] = _index;
                BoneWeights[i] = _weight;
                return;
            }
        }
    }
};