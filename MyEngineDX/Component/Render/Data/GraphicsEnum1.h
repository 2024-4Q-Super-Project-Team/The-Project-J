#pragma once

namespace Graphics
{
	enum class eCBufferType
	{
		Transform,
		Material,
		Camera,
		BonePalette,
		SIZE
	};

	enum class eMaterialMapType
	{
		DIFFUSE,
		SPECULAR,
		AMBIENT,
		EMISSIVE,
		NORMAL,
		SHININESS,
		OPACITY,
		METALNESS,
		SIZE
	};
}

