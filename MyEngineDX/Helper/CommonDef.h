#pragma once

#define FOR_LOOP_ENTITY(mem, func)				\
for (auto& [first, second] : mem) {				\
if (second->GetState() == EntityState::Active)	\
second->func;									\
}								

#define UPDATE_ENTITY(mem, func)				\
if (mem->GetState() == EntityState::Active){	\
mem->func;										\
}								