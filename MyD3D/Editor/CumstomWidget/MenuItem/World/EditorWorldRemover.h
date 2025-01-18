#pragma once

namespace Editor
{
	class WorldRemover
		: public Editor::IWidget
	{
	public:
		explicit WorldRemover();
		virtual ~WorldRemover();
	public:
		virtual void Render() override;
	private:
		std::string mlabel;
	};
}