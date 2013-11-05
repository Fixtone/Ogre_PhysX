#include "../Headers/CExitState.h"

CExitState::CExitState(CGraphicsManager* aCGraphicsManager, 
                       CInputManager* aCInputManager)
: CBaseState(aCGraphicsManager,aCInputManager)
, mRootSceneNode(0)
, mElapsedSeconds(0.0f)
, mDebugEnabled(false)
{
    //
    // TODO Constructor logic HERE
    //        
}

CExitState::~CExitState()
{
        //
        // TODO Distructor logic HERE
        //
        finalize();
}

/** Initialize current state */
void CExitState::initialize()
{
        CBaseState::initialize();        

        mDebugEnabled = false;

        mCInputManager->setKeyMode(true);
        mCInputManager->setMouseMode(true);
        mCInputManager->setJoyStickMode(true);

        this->mNextGameStateId = this->getStateId();

        // Add Playstate as frame listener
        mRoot->addFrameListener(this);
        
        buttonTimer = 0.0;
        lastKey = OIS::KeyCode::KC_UNASSIGNED;
}

/** Load resources */
void CExitState::load()
{
        //
        // TODO: Load?
        //
}

/** Manage input */
void CExitState::input()
{
        //
        // TODO Read input
        //
        mCInputManager->capture();
}

/** Rendering queue */
bool CExitState::frameRenderingQueued(const Ogre::FrameEvent& evt)
{	

    return true;
}

/** Update internal stuff */
void CExitState::update(const float elapsedSeconds)
{
	mElapsedSeconds = elapsedSeconds;       
}

/** Render */
void CExitState::render(const float elapsedSeconds)
{
        //
        // TODO
        //
}

/** Unload resources */
void CExitState::unload() 
{  
    if(mRoot)
		mRoot->removeFrameListener(this);    
}

/** Destroy the state */
void CExitState::finalize()
{
    CBaseState::finalize();
    //Debug::Out("**************************");
}

/** Get state Id */
GameStateId CExitState::getStateId()
{
    //
    // BEWARE! Make sure this is the correspondent state or we fuck up everything!
    //
    return GameStateId::Play;
}

/** Called when the state has to be paused */
void CExitState::pause()
{    

    // Hide axes if visible
    //if(mDebugEnabled)
}

/** Called when the state has to be resumed (from pause) */
void CExitState::resume()
{
    //
    // Set next state to this state (Play)  
    //
    this->mNextGameStateId = this->getStateId();		
   

    // Show axes if visible
    //if(mDebugEnabled)        
}

/** Buffered input - keyboard key clicked */
bool CExitState::keyPressed(const OIS::KeyEvent& e)
{
	return true;
}

bool CExitState::keyReleased(const OIS::KeyEvent& e)
{
    return true;
}

/** Mouse input */
bool CExitState::mouseMoved(const OIS::MouseEvent& e)
{      
   return true;
}