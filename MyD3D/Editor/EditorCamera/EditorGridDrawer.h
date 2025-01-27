#pragma once

struct GridVertex
{
	GridVertex(Vector4 _pos, Vector4 _color)
		: pos(_pos), color(_color) {}
	Vector4 pos = Vector4(0.0f,0.0f,0.0f,1.0f);
	Vector4 color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
};

class EditorGridDrawer
{
public:
	static BOOL Initialize();
	static void Finalization();
public:
	static void SetGrid(UINT _size, UINT _unit);
public:
	static void DrawGrid();
public:
	static std::vector<GridVertex> mVertexies;
	static D3DGraphicsVertexBuffer* mVsBuffer;
};