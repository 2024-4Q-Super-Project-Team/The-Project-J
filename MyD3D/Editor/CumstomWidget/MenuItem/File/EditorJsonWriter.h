#pragma once

namespace Editor
{
	// MenuItem
	class JsonWriter
		: public IWidget
	{
	public:
		explicit JsonWriter();
		virtual ~JsonWriter();
	public:
		virtual void Render() override;
	private:
		std::string mlabel;
	};
}


