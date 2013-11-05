#ifndef C_GRAPHICSMANAGER_H_
#define C_GRAPHICSMANAGER_H_

//##### OGRE INCLUDES
#include <OgreSingleton.h>	
#include <Ogre.h>
#include <OgreRenderWindow.h>
#include <Windows.h>
//##### OGRE INCLUDES

//#include "CConfig.h"

//##### DEFINES
#if _DEBUG
#define PLUGINS_FILE    ".\\Config\\Debug\\plugins_d.cfg"
#define RESOURCES_FILE  ".\\Config\\Debug\\resources_d.cfg"
#else
#define PLUGINS_FILE    ".\\Config\\Release\\plugins.cfg"
#define RESOURCES_FILE  ".\\Config\\Release\\resources.cfg"
#endif

//#define CONFIG_FILE     ".\\Config\\config.cfg"
#define LOG_FILE        ".\\TerrainPhysXLog.log"

class CGraphicsManager : public Ogre::Singleton<CGraphicsManager>
{
	//##### METHODS
	public:
		
		CGraphicsManager();
		~CGraphicsManager();

		/**	Get|Set Singleton **/
		static CGraphicsManager& getSingleton(void);
		static CGraphicsManager* getSingletonPtr(void);

		/** Initialize graphics */
		bool Initialize();
		
		/** Finalize graphics */
		void Finalize();

		/** Load graphics resources */
		void LoadResources();
		
		/** Unload graphics resources */
		void UnloadResources();

		/** Render one frame */
		bool RenderOneFrame();
		void ClearScene();

		bool IsInitialized(){return mInitialized;}

		/** Retrieve the render window */
		Ogre::Root*           GetRoot(void);
		Ogre::RenderWindow*   GetRenderWindow(void);
		Ogre::SceneManager*   GetSceneManager(void);

		//CConfig*			  GetGameConfig(void);

	private: 

		bool Configure(void);
		void ChooseSceneManager(void);
		void CreateResourceListener(void);
		
		/** Setup resources (i.e. read and parse resources .cfg file) */
		void SetupResources();

	//##### METHODS
	//##### VARIABLES
	private:
		Ogre::Root*           mRoot;
		Ogre::RenderWindow*   mRenderWindow;
		Ogre::SceneManager*   mSceneManager;
		//CConfig*			  mGameConfig;
		bool mInitialized;
	//##### VARIABLES
};

#endif //C_GRAPHICSMANAGER_H_