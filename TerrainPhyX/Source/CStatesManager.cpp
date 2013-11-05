#include "../Headers/CStatesManager.h"

// BEGIN SINGLETON
template<> CStatesManager* Ogre::Singleton<CStatesManager>::msSingleton = 0;
CStatesManager* CStatesManager::getSingletonPtr(void)
{
    return msSingleton;
}
CStatesManager& CStatesManager::getSingleton(void)
{  
    assert( msSingleton );  return ( *msSingleton );  
}
// END SINGLETON

CStatesManager::CStatesManager()
: mCurrentStateId ( GameStateId::Play )
, mInitialized(false)
{
        //
        // TODO Constructor
        //
}

CStatesManager::~CStatesManager()
{
        //
        // TODO Destructor
        //
        if(mInitialized)
        {
                finalize();
        }
}

void CStatesManager::initialize(CGraphicsManager* aCGraphicsManager, CInputManager* aCInputManager)
{
        //
        // Keep a reference to the managers
        //
        this->mCGraphicsManager = aCGraphicsManager;
        this->mCInputManager = aCInputManager;

        //
        // Register this class as listerer for the input manager (buffered input!)
        // Buffered events will call buffered input methods of this class
        // Fo unbuffered input we will have to access the mInputManager directly
        // in the game loop
        this->mCInputManager->addListener(this);

        //
        // NOTE Here we simply create the game states list, hard coded.
        //              Maybe it is better to provide some kind of way to load it
        //              'on the fly', reading it from a .cfg file?
        //
		this->mStates[GameStateId::Play]        = new CPlayState(mCGraphicsManager, mCInputManager);        
		this->mStates[GameStateId::Exit]        = new CExitState(mCGraphicsManager, mCInputManager);              

        //
        // Let's start with the first state!
        //
        changeState(getGameStateById(mCurrentStateId));

        mInitialized = true;
}

void CStatesManager::finalize()
{
        StatesMapIterator it;
        for(it = mStates.begin(); it != mStates.end(); it++)
        {
                delete it->second;
                it->second = 0;
        }

        mStates.clear();

        this->mCInputManager->removeListener(this);

        mInitialized = false;
}

bool CStatesManager::loop(const float elapsedSeconds)
{
                //
                // Read (UNBUFFERED) input
                //
                this->mCurrentState->input();

                //
                // Read next state. If it is not the current one, perform a state change!
                // 
                GameStateId nextStateId = mCurrentState->getNextStateId();


                //
                // Special case, we just exit the application
                //
                if(nextStateId == GameStateId::Exit)
                        return false;

                if(mCurrentStateId != nextStateId)
                {
                        //
                        // Retrieve the state corrisponding to the given state id
                        //
                        CBaseState* newState = this->getGameStateById(nextStateId);                        
                        
                        this->changeState(newState);                      
                }

                //
                // Process game logic/physics/state
                //
                this->mCurrentState->update(elapsedSeconds);

                //
                // Render output
                //
                this->mCurrentState->render(elapsedSeconds);

                //
                // Keep going 
                //
                return true;
}

// ------------ PRIVATE METHODS -------------------

void CStatesManager::changeState(CBaseState* state)
{
        // cleanup the current state
        if ( !mStatesStack.empty() ) {
                //
                // Exit from current state
                //
                mStatesStack.back()->exit();
                mStatesStack.pop_back();
        }

        //
        // Set new state as current state
        //
        this->mCurrentState = state;

        //
        // Save current state Id
        //
        this->mCurrentStateId = this->mCurrentState->getStateId();

        //
        // Store and init the new state
        //
        mStatesStack.push_back(state);
        mStatesStack.back()->enter();
}


void CStatesManager::pushState(CBaseState* state)
{
        //
        // Pause current state
        //
        if ( !mStatesStack.empty() ) {
                mStatesStack.back()->pause();
        }

        //
        // Set new state as current state
        //
        this->mCurrentState = state;

        //
        // Save current state Id
        //
        this->mCurrentStateId = this->mCurrentState->getStateId();

        //
        // Store and init the new state
        //
        mStatesStack.push_back(state);
        mStatesStack.back()->enter();
}

void CStatesManager::popState()
{
        //
        // Cleanup the current state
        //
        if ( !mStatesStack.empty() ) {
                mStatesStack.back()->exit();
                mStatesStack.pop_back();
        }

        if( mStatesStack.empty() )
        {
                //
                // Set new state as current state
                //
			this->mCurrentState = this->getGameStateById(GameStateId::Play);

                //
                // Save current state Id
                //
                this->mCurrentStateId = GameStateId::Play;

                //
                // Store and init the new state
                //
                mStatesStack.push_back(this->mCurrentState);
                mStatesStack.back()->enter();

        }
        else
        {

                //
                // Set new state as current state
                //
                this->mCurrentState = mStatesStack.back();

                //
                // Save current state Id
                //
                this->mCurrentStateId = this->mCurrentState->getStateId();

                //
                // Resume previous state
                //
                if ( !mStatesStack.empty() ) {
                        mStatesStack.back()->resume();
                }
        }
}

CBaseState* CStatesManager::getGameStateById(const GameStateId gameStateId)
{
        return this->mStates[gameStateId];
}

// ------------------
// Keyboard listeners
// ------------------
/** Buffered input - keyboard key clicked */
bool CStatesManager::keyClicked(const OIS::KeyEvent& e)
{
        // call keyClicked of current state
        return this->mCurrentState->keyClicked(e);
}

/** Buffered input - keyboard key clicked */
bool CStatesManager::keyPressed(const OIS::KeyEvent& e)
{
        // call keyPressed of current state
        return this->mCurrentState->keyPressed(e);
}

/** Buffered input - keyboard key clicked */
bool CStatesManager::keyReleased(const OIS::KeyEvent& e)
{
        // call keyReleased of current state
        return this->mCurrentState->keyReleased(e);
}

//
// MouseListener
//
/** Buffered input - mouse moved */
bool CStatesManager::mouseMoved(const OIS::MouseEvent &evt)
{
        return this->mCurrentState->mouseMoved(evt);
}

/** Buffered input - mouse button pressed */
bool CStatesManager::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID buttonId)
{
        return this->mCurrentState->mousePressed(evt,buttonId);
}

/** Buffered input - mouse button released */
bool CStatesManager::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID buttonId)
{
        return this->mCurrentState->mouseReleased(evt,buttonId);
}

//
// JoyStickListener
//
/** Buffered input - joystick button pressed */
bool CStatesManager::buttonPressed(const OIS::JoyStickEvent &evt, int index)
{
        return this->mCurrentState->buttonPressed(evt,index);
}

/** Buffered input - joystick button released */
bool CStatesManager::buttonReleased(const OIS::JoyStickEvent &evt, int index)
{
        return this->mCurrentState->buttonReleased(evt,index);
}

/** Buffered input - axis pad moved */
bool CStatesManager::axisMoved(const OIS::JoyStickEvent &evt, int index)
{
        return this->mCurrentState->axisMoved(evt,index);
}

/** Buffered input - pov moved */
bool CStatesManager::povMoved(const OIS::JoyStickEvent &evt, int index)
{
        return this->mCurrentState->povMoved(evt,index);
}