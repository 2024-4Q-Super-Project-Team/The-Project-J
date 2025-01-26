#pragma once

namespace Editor
{
	// MenuItem
	class StartWorldSetter
		: public IWidget
	{
	public:
		explicit StartWorldSetter();
		virtual ~StartWorldSetter();
	public:
		virtual void Render() override;
	private:
		std::string mlabel;
	};

}

