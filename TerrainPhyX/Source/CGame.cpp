#include "../Headers/CGame.h"

// BEGIN SINGLETON
template<> CGame* Ogre::Singleton<CGame>::msSingleton = 0;
CGame* CGame::getSingletonPtr(void)
{
    return msSingleton;
}
CGame& CGame::getSingleton(void)
{  
    assert( msSingleton );  return ( *msSingleton );  
}
// END SINGLETON

CGame::CGame()
: mElapsedSeconds(0.0f)
, mTotalSeconds(0.0f)
{
	mCGraphicsManager = new CGraphicsManager();
	mCInputManager = new CInputManager();
	mCStatesManager = new CStatesManager();
	//mCCameraManager = new CCameraManager();
}

CGame::~CGame()
{
	if(mCGraphicsManager)
		delete mCGraphicsManager;
	
	if(mCInputManager)
		delete mCInputManager;

	if(mCStatesManager)
		delete mCStatesManager;

	//delete mCCameraManager;
}

/** Initialize */ 
bool CGame::Initialize()
{
	// Initialize graphic manager
	bool carryOn = mCGraphicsManager->Initialize();

	if(carryOn)
	{                               
		// Load graphic resources
		mCGraphicsManager->LoadResources();          		

		// Initialize input manager. A render window is needed in order to setup mouse coords and boundaries.
		mCInputManager->initialize(mCGraphicsManager->GetRenderWindow(), true);

	//	mCCameraManager->initialize(mCGraphicsManager->GetSceneManager(), mCGraphicsManager->GetRenderWindow());		
	//		
		//Initialize Game states (FSM) manager
		mCStatesManager->initialize(mCGraphicsManager, mCInputManager);
	}

	return carryOn;
}

/** Main loop */
void CGame::Start()
{
	bool continueRunning;
	if(mCGraphicsManager->IsInitialized())
		continueRunning = true;
	else
		continueRunning = false;

	while ( continueRunning )
	{
		Ogre::WindowEventUtilities::messagePump();

		// Get elaspes microseconds
		mElapsedSeconds = mLoopTimer.getMicroseconds() * 1.0 / 1000000;

		// Updte total seconds
		mTotalSeconds += mElapsedSeconds;

		continueRunning = mCStatesManager->loop(mElapsedSeconds);

		bool windowClosed = mCGraphicsManager->GetRenderWindow()->isClosed();
		continueRunning &= ! windowClosed;

		mLoopTimer.reset();

		bool renderFrameSuccess = mCGraphicsManager->RenderOneFrame();
		continueRunning &= renderFrameSuccess;
	}
}

/** Finalize the game */
void CGame::Finalize()
{       
	// Unload all graphics data
	//mCGraphicsManager->UnloadResources();
	// Finalize all
	//mCGraphicsManager.reset();	
}