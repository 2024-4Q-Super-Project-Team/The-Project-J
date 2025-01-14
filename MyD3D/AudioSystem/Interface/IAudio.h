#pragma once

class IAudio
{
};

class IAudioResource
{
public:
	virtual HRESULT Create() = 0;
	virtual void	Release() = 0;
};

