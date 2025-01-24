#pragma once
class SaveManager
{
public:
	static void Save();
	static void Load();

private:
	static std::string filePath;
};

