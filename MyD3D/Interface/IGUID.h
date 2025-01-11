#pragma once
namespace Engine
{
	class IGUID
	{
		static int mIdSize;
	public:
		IGUID() :mId(mIdSize++) {}
		unsigned int mId;
	};

	int IGUID::mIdSize = 0;
}