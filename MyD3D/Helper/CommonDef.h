#pragma once
#define _INTERFACE 
#define _ABSTRACT
#define _CALLBACK

#define _READ_ONLY

#define _IA
#define _VS
#define _PS

#define SAFE_RELEASE(ptr)           if( (ptr) ) { (ptr)->Release(); } (ptr) = nullptr;
#define SAFE_RELEASE_ARRAY(Arr)     for( auto& ptr : Arr ) { if( (ptr) ) { (ptr)->Release(); (ptr) = nullptr; } }
#define SAFE_RELEASE_VECTOR(Vec)     for( auto& ptr : Vec ) { if( (ptr) ) { (ptr)->Release(); (ptr) = nullptr; } } Vec.clear()
#define SAFE_RELEASE_MAP(Map)       for(  auto& [key, ptr] : Map ) { if( (ptr) ) { (ptr)->Release(); (ptr) = nullptr; } } Map.clear()
#define SAFE_DELETE(ptr)            if( (ptr) ) { delete (ptr); } (ptr) = nullptr;
#define SAFE_DELETE_ARRAY(Arr)      for( auto& ptr : Arr ) { if( (ptr) ) { delete (ptr); (ptr) = nullptr; } }
#define SAFE_DELETE_VECTOR(Vec)     for( auto& ptr : Vec ) { if(!Vec.empty()) { if( (ptr) ) { delete (ptr); (ptr) = nullptr; } } } Vec.clear()
#define SAFE_DELETE_MAP(Map)        for( auto& [key, ptr] : Map ) { if( (ptr) ) { delete (ptr); (ptr) = nullptr; } } Map.clear()

#define FOR_LOOP_MAP_ENTITY(mem, func)						\
for (auto itr = mem.begin(); itr != mem.end(); ++itr) {		\
if (itr->second->GetState() == EntityState::Active)			\
itr->second->func;											\
}								

#define FOR_LOOP_ARRAY_ENTITY(mem, func)					\
for (auto itr = mem.begin(); itr != mem.end(); ++itr) {		\
if ((*itr)->GetState() == EntityState::Active)				\
(*itr)->func;												\
}								

#define FOR_LOOP_ARRAY(mem, func)							\
for (auto itr = mem.begin(); itr != mem.end(); ++itr) {		\
(*itr)->func												\
}								

#define FOR_LOOP_MAP(mem, func)								\
for (auto itr = mem.begin(); itr != mem.end(); ++itr) {		\
itr->second->func											\
}	

#define UPDATE_ENTITY(mem, func)				\
if (mem->GetState() == EntityState::Active){	\
mem->func;										\
}				

// 컨테이너와 람다조건식으로 이터레이터 반환
#define FIND_CONTAINER(Container, Condition)				\
std::find_if(Container.begin(), Container.end(), Condition) \

#define FIND_MAP(Key, Map)                                  \
auto itr = Map.find(_key);                                  \

#define FIND_SUCCESS(itr, Arr) itr != Arr.end()
#define FIND_FAILED(itr, Arr) itr == Arr.end()

#define ENTITY_IS_ACTIVE(mem)	mem->GetState() == EntityState::Active
#define ENTITY_IS_CREATE(mem)	mem->GetState() == EntityState::Create
#define ENTITY_IS_DESTROY(mem)	mem->GetState() == EntityState::Destroy