#include "GlobalFunc.h"
#include "TSingleton.h"

class GlobalManager
{
public:
	NetMediaManager m_mediaManager;
};

typedef comn::Singleton<GlobalManager> SingletonGlobalManager;

GlobalManager& getGlobalManager()
{
	return SingletonGlobalManager::instance();
}

NetMediaManager &getMediaManager()
{
	return getGlobalManager().m_mediaManager;
}


