#pragma once

namespace Editor
{
	// MenuItem
	class JsonLoader
		: public IWidget
	{
	public:
		explicit JsonLoader();
		virtual ~JsonLoader();
	public:
		virtual void Render() override;
	private:
		std::string mlabel;
	};
}


