#pragma once

namespace Engine
{
	class SaveBase
	{
	public:
		static inline void Reset() { mIdCount = 0; }
	public:
		unsigned int GetId() { return mId;  }
		unsigned int GiveId() { mId = mIdCount++; return mId;  }
		void SetId(unsigned int id) { mId = id; mMap[id] = this; }
	public:
		virtual json Serialize() = 0;
		virtual void Deserialize(json& j) = 0;
	public:
		static std::unordered_map<unsigned int, SaveBase*> mMap;
	private:
		static unsigned int mIdCount;
		unsigned int mId;
	};
}