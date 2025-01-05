#ifndef EMULATIONSTATION_ALL_SYSTEMCONF_H
#define EMULATIONSTATION_ALL_SYSTEMCONF_H


#include <string>
#include <map>
#include <set>

class SystemConf 
{
public:
	static SystemConf* getInstance();
	
	static bool getIncrementalSaveStates();
	static bool getIncrementalSaveStatesUseCurrentSlot();

    bool loadSystemConf();
    bool saveSystemConf();

    std::string get(const std::string &name);
    bool set(const std::string &name, const std::string &value);

	bool getBool(const std::string &name, bool defaultValue = false);
	bool setBool(const std::string &name, bool value);

private:
	SystemConf();
	static SystemConf* sInstance;

	std::map<std::string, std::string> confMap;
	std::set<std::string> changedConf;


	std::string mSystemConfFile;
	std::string mSystemConfFileTmp;
};


#endif //EMULATIONSTATION_ALL_SYSTEMCONF_H
