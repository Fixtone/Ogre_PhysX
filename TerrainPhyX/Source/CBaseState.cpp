#include "../Headers/CBaseState.h"

CBaseState::CBaseState()
: mSceneManager(0)
, mRoot(0)
, mWindow(0)
, mIsActive(false)
, mIsDead(false)
, mIsPaused(false)
{
        this->mIsActive = false;
        this->mIsDead = false;
        this->mIsPaused = false;
}

CBaseState::CBaseState(CGraphicsManager* aCGraphicsManager, 
                       CInputManager* aCInputManager)
: mSceneManager(0)
, mRoot(0)
, mWindow(0)
, mIsActive(false)
, mIsDead(false)
, mIsPaused(false)
{
        this->mCGraphicsManager  = aCGraphicsManager;
        this->mCInputManager     = aCInputManager;    		
}

CBaseState::~CBaseState()
{
    //
    // TODO Distructor logic HERE
    //
}

void CBaseState::initialize()
{
    //
    // TODO Initialize
    //
    this->mNextGameStateId = this->getStateId();

	mRoot = mCGraphicsManager->GetRoot();

    mWindow = mCGraphicsManager->GetRenderWindow();

    mSceneManager = mCGraphicsManager->GetSceneManager();
}

void CBaseState::finalize()
{
        // TODO : Common finalize stuff
}

void CBaseState::enter()
{
        //
        // Initialize the state...
        //
        this->initialize();

        //
        // ...and load its resources
        //
        this->load();
}

void CBaseState::exit()
{
        // 
        // Unload resources for current state
        //
        this->unload();

        //
        // Destroy and finalize current state
        //
        this->finalize();
}
