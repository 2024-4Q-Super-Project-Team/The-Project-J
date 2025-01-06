#pragma once

struct Vertex
{
    Vertex() = default;
    Vertex(Vector4 _position, Vector4 _color, Vector3 _normal)
        : Position(_position), Color(_color), Normal(_normal){
    }
    Vertex(Vector4 _position, Vector4 _color, Vector3 _normal, Vector3 _tangent)
        : Position(_position), Color(_color), Normal(_normal), Tangent(_tangent) {}

    // ������ ��ǥ
    Vector4 Position = { 0,0,0,1 };
    // ������ ����
    Vector4 Color = { 1.0f,1.0f,1.0f,1.0f };
    // ���� �븻 ��
    Vector3 Normal = { 0,0,0 };
    // ���� ź��Ʈ
    Vector3 Tangent = { 0,0,0 };
    Vector3 BiTangent = { 0,0,0 };
    // ���� �ؽ��� ��ǥ
    Vector2 TexCoord = { 0,0 };
    // ������ �Ҽ� �޽��κ����� �����̺� ���� �ε��� (-1�� ����޴� ���� ���ٴ� ��)
    INT		BoneIndices[4] = { 0,0,0,0 };
    // ������ ����Ʈ���� ����Ʈ ����ġ (0.0 ~ 1.0)
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