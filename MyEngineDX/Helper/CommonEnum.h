#pragma once

enum class EntityState
{
	Create,
	Active,
	Passive,
	Destroy,
};

enum class ResourceType
{
	NONE,
	Texture,
	Sprite,
	Animation,
	AudioClip,
	Font,
	SIZE,
};