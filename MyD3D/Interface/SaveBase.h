#pragma once

namespace Engine
{
	class SaveBase
	{
		static unsigned int mIdCount;
		unsigned int mId;
	public:
		SaveBase() :mId(mIdCount++) {}
		unsigned int GetId() { return mId; }
		void SetId(unsigned int id) { mId = id; mMap[id] = this; }

		static std::unordered_map<unsigned int, SaveBase*> mMap;
	};
}