#pragma once

#define _INTERFACE 
#define _ABSTRACT
#define _CALLBACK

#define SAFE_RELEASE(ptr)  if( (ptr) ) { (ptr)->Release(); (ptr) = nullptr; }
#define SAFE_DELETE(ptr)   if( (ptr) ) { delete (ptr); (ptr) = nullptr; }
#define SAFE_DELETE_ARRAY(Arr)   for( auto& ptr : Arr ) { if( (ptr) ) { delete (ptr); (ptr) = nullptr; } }
#define SAFE_DELETE_VECTOR(Vec)   for( auto& ptr : Vec ) { if( (ptr) ) { delete (ptr); (ptr) = nullptr; } } Vec.clear();
#define SAFE_DELETE_MAP(Map)   for( auto& [key, ptr] : Map ) { if( (ptr) ) { delete (ptr); (ptr) = nullptr; } } Map.clear();