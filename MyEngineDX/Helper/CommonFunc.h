#pragma once

namespace Helper
{
	template <typename KEY, typename TYPE>
	TYPE* FindMap(const KEY& _key, std::unordered_map<KEY, TYPE>& _map)
	{
		auto itr = _map.find(_key);
		return (itr != _map.end()) ? &itr->second : nullptr;
	}
}