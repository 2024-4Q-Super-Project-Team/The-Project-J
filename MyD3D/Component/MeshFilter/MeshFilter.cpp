#include "pch.h"
#include "MeshFilter.h"
#include "Resource/Graphics/Mesh/Mesh.h"

MeshFilter::MeshFilter(Object* _owner)
    : Component(_owner)
{
   // mMesh = new Mesh();
}

MeshFilter::~MeshFilter()
{
}

json MeshFilter::Serialize()
{
    return json();
}

json MeshFilter::Deserialize()
{
    return json();
}
