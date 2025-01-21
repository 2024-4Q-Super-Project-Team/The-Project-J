#pragma once

namespace Editor
{
	// MenuItem
	class WorldChanger
		: public Editor::IWidget
	{
	public:
		explicit WorldChanger();
		virtual ~WorldChanger();
	public:
		virtual void Render() override;
	private:
		std::string mlabel;
	};

}

