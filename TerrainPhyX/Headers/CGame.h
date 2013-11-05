#ifndef C_GAME_H_
#define C_GAME_H_

//##### OGRE INCLUDES
#include <OgreSingleton.h>
#include <OgreFrameListener.h>
#include <OgreWindowEventUtilities.h>
#include <OgreTimer.h>
//##### OGRE INCLUDES

//##### GAME INCLUDES
#include "CGraphicsManager.h"
#include "CInputManager.h"
#include "CStatesManager.h"
//##### GAME INCLUDES

class CGame : public Ogre::Singleton<CGame>
			, public Ogre::FrameListener
			, public Ogre::WindowEventListener
{
	//##### METHODS
	public:
		
		CGame();
		~CGame();

		/**	Get|Set Singleton **/
		static CGame& getSingleton(void);
		static CGame* getSingletonPtr(void);

		/** Initialize */ 
		bool Initialize();

		/** Main loop */
		void Start();

		/** Finalize the game */
		void Finalize();

	//##### VARIABLES
	private:

		/** Graphics manager */
		CGraphicsManager* mCGraphicsManager;

		/** Input manager */
		CInputManager* mCInputManager;

		/** Input manager */
		CStatesManager* mCStatesManager;

		/** Camera manager */
		//CCameraManager* mCCameraManager;

		Ogre::Timer mLoopTimer;
		// Total seconds since first loop (absolute gaming time)
		double mTotalSeconds;
		// Total seconds since last loop (relative loop time)
		double mElapsedSeconds;
};

#endif //C_GAME_H_