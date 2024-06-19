#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS

#include<iostream>
#include<fstream>
#include<string>
#include <filesystem>

#include"UTF8Converter.hpp"

#define CURENTVERSION_FILE "versionMods\\curentVersion"

#define MINEVERSIONS_PATH "versionMods\\"
#define MODS_PATH "mods\\"
#define CONFIGS_PATH "config\\"
#define VERSION_PATH "versions\\"

namespace fs = std::filesystem;

bool isEqual(const char* str1, const char* str2)
{
	return std::string(str1) == std::string(str2);
}

class MinecraftMeneger
{
public:
	MinecraftMeneger() :
		minecraftPath{ getMinecraftPath() }
	{
		initProg();
		modsVersion = getFileModsVersion();
	}

	const std::string& getMinePath() const
	{
		return minecraftPath;
	}

	const std::string getModsVersion() const
	{
		return modsVersion;
	}

	void setModsVersion(const std::string& version)
	{
		modsVersion = version;
		setFileModsVersion(modsVersion);
	}

	bool isMineVersion(const std::string& version) const
	{
		return fs::exists(minecraftPath + VERSION_PATH + version);
	}

	bool isSavedModeVersion(const std::string& version)
	{
		return fs::exists(minecraftPath + MINEVERSIONS_PATH + version);
	}

	std::string getMinecraftModsPath() const
	{
		return minecraftPath + MODS_PATH;
	}
	std::string getMinecraftConfigsPath() const
	{
		return minecraftPath + CONFIGS_PATH;
	}

	std::string getSavedVersionModsPath() const
	{
		return minecraftPath + MINEVERSIONS_PATH + modsVersion + "\\" + MODS_PATH;
	}
	std::string getSavedVersionConfigsPath() const
	{
		return minecraftPath + MINEVERSIONS_PATH + modsVersion + "\\" + CONFIGS_PATH;
	}

	void addNewVersionPath(const std::string& version)
	{
		if (!isMineVersion(version))
			return;

		std::string newDirectory = minecraftPath + MINEVERSIONS_PATH + version + "\\";

		fs::create_directory(newDirectory);
		fs::create_directory(newDirectory + MODS_PATH);
		fs::create_directory(newDirectory + CONFIGS_PATH);
	}

private:

	const std::string minecraftPath;

	std::string modsVersion;

private:


	static std::string getMinecraftPath()
	{
		char* appdata = nullptr;
		size_t len = 0;
	
		errno_t err = _dupenv_s(&appdata, &len, "APPDATA");
	
		if (err != 0 || appdata == nullptr)
			return "";
	
		std::string fName = appdata;
		fName += "\\.minecraft\\";
	
		free(appdata);
		return fName;
	}
	static std::string getFileModsVersion()
	{
		std::ifstream in{ getMinecraftPath() + CURENTVERSION_FILE};
		std::string version;
		std::getline(in, version);
		in.close();
		return version;
	}
	static void setFileModsVersion(const std::string& version)
	{
		std::ofstream out{ getMinecraftPath() + CURENTVERSION_FILE};
		out << version;
		out.close();
	}

	static void initProg()
	{
		const std::string minecraftPath = getMinecraftPath();
		if (fs::exists(minecraftPath + MINEVERSIONS_PATH))
			return;
		fs::create_directory(minecraftPath + MINEVERSIONS_PATH);
		std::ofstream prOut{ minecraftPath + CURENTVERSION_FILE };
		prOut.close();
	}

};

class PathMeneger
{
public:

	void clearPath(const std::string& path) const
	{
		if (!fs::is_directory(path))
			return;
		fs::remove_all(path);
		fs::create_directory(path);
	}

	void copyAll(const std::string& from, const std::string& to) const
	{
		fs::path sourceDir = from;
		fs::path targetDir = to;

		fs::create_directories(targetDir);

		for (const auto& entry : fs::directory_iterator(sourceDir)) 
		{
			const fs::path& path = entry.path();
			std::string relativePathStr = path.lexically_relative(sourceDir).string();
			fs::path targetPath = targetDir / relativePathStr;
			try 
			{
				if (fs::is_directory(entry.status())) 
				{
					fs::create_directories(targetPath);
					copyAll(path.string(), targetPath.string());
				}
				else if (fs::is_regular_file(entry.status()))
					fs::copy_file(path, targetPath, fs::copy_options::overwrite_existing);
				else
					std::cout << L"Пропущен файл \""_utf8 << path << "\"" << std::endl;
			}
			catch (fs::filesystem_error& e)
			{
				std::cout << L"Ошибка переноса \""_utf8 << path << L"\" в \""_utf8 << targetPath << "\": " << e.what() << std::endl;
			}
		}
	}

private:

};

class ComandMeneger
{
public:

	int printHelp()
	{
		std::cout << L"info                               - Текущая сохранённая версия модов."_utf8 << std::endl;
		std::cout << L"add    <Текущая версия майнкрафта> - Сохранить новую версию в папку сохранений."_utf8 << std::endl;
		std::cout << L"load   <Текущая версия майнкрафта> - Переключится на новую версию"_utf8 << std::endl;
		std::cout << L"save                               - Сохранение текущей версии"_utf8 << std::endl;
		std::cout << L"clear                              - Удаление модов"_utf8 << std::endl;
		return 0;
	}

	int printVersion()
	{
		std::cout << L"Текущая версия: "_utf8 << miner.getModsVersion() << std::endl;
		return 0;
	}

	/*Сохраняет текущую версию как новую*/
	int addVersion(const std::string& version)
	{
		if (!miner.isMineVersion(version))
		{
			std::cout << L"Данной версии нет в папке версий майнкрафта."_utf8 << std::endl;
			return 1;
		}
		if (miner.isSavedModeVersion(version))
		{
			std::cout << L"Дананя версия уже добавлена (Для сохранения воспользуйтесь save)."_utf8 << std::endl;
			return 1;
		}

		miner.addNewVersionPath(version);
		miner.setModsVersion(version);
		pather.copyAll(miner.getMinecraftModsPath(), miner.getSavedVersionModsPath());
		pather.copyAll(miner.getMinecraftConfigsPath(), miner.getSavedVersionConfigsPath());

		std::cout << L"Сохранена и поставлена текущей версия модов: "_utf8 << miner.getModsVersion() << std::endl;

		return 0;
	}

	int update(const std::string& version)
	{
		if (!miner.isMineVersion(version))
		{
			std::cout << L"Данной версии нет в папке версий майнкрафта."_utf8 << std::endl;
			return 1;
		}
		if (!miner.isSavedModeVersion(version))
		{
			std::cout << L"Данной версии нет в папке сохранений модов.(Для добавления используйте add)"_utf8 << std::endl;
			return 1;
		}

		std::string oldVers = miner.getModsVersion();
		saveCurentVersion();

		pather.clearPath(miner.getMinecraftModsPath());
		pather.clearPath(miner.getMinecraftConfigsPath());

		miner.setModsVersion(version);

		pather.copyAll(miner.getSavedVersionConfigsPath(), miner.getMinecraftConfigsPath());
		pather.copyAll(miner.getSavedVersionModsPath(), miner.getMinecraftModsPath());

		std::cout << L"Обновлена версия модов с "_utf8 << oldVers << L" на "_utf8 << miner.getModsVersion() << std::endl;

		return 0;
	}

	int saveCurentVersion() const
	{
		pather.clearPath(miner.getSavedVersionModsPath());
		pather.clearPath(miner.getSavedVersionConfigsPath());

		pather.copyAll(miner.getMinecraftModsPath(), miner.getSavedVersionModsPath());
		pather.copyAll(miner.getMinecraftConfigsPath(), miner.getSavedVersionConfigsPath());


		std::cout << L"Сохранена версия: "_utf8 << miner.getModsVersion() << std::endl;

		return 0;
	}

	int clear()
	{
		pather.clearPath(miner.getMinecraftConfigsPath());
		pather.clearPath(miner.getMinecraftModsPath());
		std::cout << L"Отчищены папки: "_utf8 << std::endl;
		std::cout << "1 - " << miner.getMinecraftConfigsPath() << std::endl;
		std::cout << "2 - " << miner.getMinecraftModsPath() << std::endl;
		return 0;
	}

private:

	MinecraftMeneger miner;
	PathMeneger pather;


private:

	std::string getInput(std::string str) const
	{
		std::cout << str;
		std::getline(std::cin, str);
		return str;
	}


};

int main(int argc, char* argv[])
{

#if defined(_WIN32) || defined(_WIN64)
	system("chcp 65001 > nul");
#endif // defined(_WIN32) || defined(_WIN64)

	if (argc == 1)
	{
		std::cout << L"Вы не ввели команду."_utf8 << std::endl;
		return 1;
	}

	ComandMeneger meneger;

	if (isEqual(argv[1], "help"))
		return meneger.printHelp();
	if (isEqual(argv[1], "info"))
		return meneger.printVersion();
	if (isEqual(argv[1], "clear"))
		return meneger.clear();
	if (isEqual(argv[1], "save"))
		return meneger.saveCurentVersion();
	if (isEqual(argv[1], "add") && argc > 2)
		return meneger.addVersion(argv[2]);
	if (isEqual(argv[1], "load") && argc > 2)
		return meneger.update(argv[2]);

	std::cout << L"Неизвестная команда."_utf8 << std::endl;

	return 0;
}