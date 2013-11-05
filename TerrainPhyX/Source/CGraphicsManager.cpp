#include "..\Headers\CGraphicsManager.h"

// BEGIN SINGLETON
template<> CGraphicsManager* Ogre::Singleton<CGraphicsManager>::msSingleton = 0;
CGraphicsManager* CGraphicsManager::getSingletonPtr(void)
{
    return msSingleton;
}
CGraphicsManager& CGraphicsManager::getSingleton(void)
{  
    assert( msSingleton );  return ( *msSingleton );  
}
// END SINGLETON

CGraphicsManager::CGraphicsManager() 
: mRenderWindow(0)
, mRoot(0)
, mSceneManager(0)
, mInitialized(false)
{
}

CGraphicsManager::~CGraphicsManager()
{
	if(mInitialized)
		Finalize();        
}

bool CGraphicsManager::Initialize()
{
	// create an instance of LogManager prior to using LogManager::getSingleton()
	Ogre::LogManager* logMgr = new Ogre::LogManager;
	Ogre::Log *log = Ogre::LogManager::getSingleton().createLog(LOG_FILE, true, false, false);	
	log->setDebugOutputEnabled(true);

	// Create the root
	mRoot = new Ogre::Root(PLUGINS_FILE);

	// Setup resource locations
	SetupResources();	

	//mGameConfig = new CConfig();
	
	// Configure (configuration window)
	bool carryOn = Configure();
	if (!carryOn) return false;

	// Pick a scene manager
	ChooseSceneManager();

	// Set default mipmap level (NB some APIs ignore this)
	Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);

	// Create any resource listeners (for loading screens)
	CreateResourceListener();

	// Load resources
	//LoadResources();	

	mInitialized = true;

	return mInitialized;
}

void CGraphicsManager::SetupResources()
{
        // Load resource paths from config file
        Ogre::ConfigFile cf;
        cf.load(RESOURCES_FILE);

        // Go through all sections & settings in the file
        Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();

        Ogre::String secName, typeName, archName;
        while (seci.hasMoreElements())
        {
                secName = seci.peekNextKey();
                Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
                Ogre::ConfigFile::SettingsMultiMap::iterator i;
                for (i = settings->begin(); i != settings->end(); ++i)
                {
                        typeName = i->first;
                        archName = i->second;
                         Ogre::ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName);
                }
        }
}

bool CGraphicsManager::Configure()
{
    // Show the configuration dialog and initialise the system
	// You can skip this and use root.restoreConfig() to load configuration
	// settings if you were sure there are valid ones saved in ogre.cfg

	bool configOk;	
	configOk = mRoot->showConfigDialog();
	
	if(configOk)
	{
		// If returned true, user clicked OK so initialise
		// Here we choose to let the system create a default rendering window by passing 'true'
		mRenderWindow = mRoot->initialise(true, "TerrainPhysX Render Window");

		// Let's add a nice window icon
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
		HWND hwnd;
		mRenderWindow->getCustomAttribute("WINDOW", (void*)&hwnd);
		LONG iconID   = 0;// (LONG)LoadIcon( GetModuleHandle(0), MAKEINTRESOURCE(IDI_APPICON) );
		SetClassLong( hwnd, -14, iconID );
#endif
		return true;
	}
	else
	{
		mInitialized = false;
		return false;
	}
}

void CGraphicsManager::CreateResourceListener(void)
{
        //
        // TODO Create a resource listener
        //
}

void CGraphicsManager::Finalize()
{
        if(mSceneManager)
        {
                mSceneManager->clearScene();

                delete mSceneManager;
                mSceneManager = NULL;
        }

        mInitialized = false;
}

void CGraphicsManager::LoadResources()
{
        //mLoadingBar = new LoadingBar();

        //mLoadingBar->start(mWindow, 1, 1, 0.75);

        // Turn off rendering of everything except overlays
        mSceneManager->clearSpecialCaseRenderQueues();
        mSceneManager->addSpecialCaseRenderQueue(Ogre::RENDER_QUEUE_OVERLAY);
        mSceneManager->setSpecialCaseRenderQueueMode(Ogre::SceneManager::SCRQM_INCLUDE);

        // Initialise the rest of the resource groups, parse scripts etc
        Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
        Ogre::ResourceGroupManager::getSingleton().loadResourceGroup(
                Ogre::ResourceGroupManager::getSingleton().getWorldResourceGroupName(),
                false, true);

        // Back to full rendering
        mSceneManager->clearSpecialCaseRenderQueues();
        mSceneManager->setSpecialCaseRenderQueueMode(Ogre::SceneManager::SCRQM_EXCLUDE);        
}

void CGraphicsManager::UnloadResources()
{
        //
        // TODO Unload resources
        //
}

void CGraphicsManager::ChooseSceneManager(void)
{
        // Get the SceneManager, in this case a generic one
        mSceneManager = mRoot->createSceneManager(Ogre::ST_GENERIC, "Default");
}

Ogre::Root* CGraphicsManager::GetRoot()
{
	return this->mRoot;
}

Ogre::RenderWindow* CGraphicsManager::GetRenderWindow()
{
	return this->mRenderWindow;
}

Ogre::SceneManager* CGraphicsManager::GetSceneManager()
{
	return this->mSceneManager;
}

//CConfig* CGraphicsManager::GetGameConfig()
//{
//	return this->mGameConfig;
//}

/** Render one frame */
bool CGraphicsManager::RenderOneFrame()
{
    bool result = false;

    try
    {
    result =  mRoot->renderOneFrame();
    }
    catch (Ogre::Exception& e)
    {
        Ogre::LogManager::getSingleton().logMessage(e.getFullDescription());
    }

    return result;
}

void CGraphicsManager::ClearScene()
{
    if(mSceneManager)
    {
        mSceneManager->clearScene();
    }
}
