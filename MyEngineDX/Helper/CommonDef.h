#pragma once




#define UPDATE_ENTITY(mem, func)				\
if (mem->GetState() == EntityState::Active){	\
mem->func;										\
}								