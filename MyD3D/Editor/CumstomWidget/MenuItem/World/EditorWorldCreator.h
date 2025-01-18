#pragma once

namespace Editor
{
	class WorldCreator
		: public Editor::IWidget
	{
	public:
		explicit WorldCreator();
		virtual ~WorldCreator();
	public:
		virtual void Render() override;
	private:
		std::string mlabel;
	};
}