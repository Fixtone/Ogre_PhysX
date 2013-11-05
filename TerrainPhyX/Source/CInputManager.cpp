#include "../Headers/CInputManager.h"

// BEGIN SINGLETON
template<> CInputManager* Ogre::Singleton<CInputManager>::msSingleton = 0;
CInputManager* CInputManager::getSingletonPtr(void)
{
    return msSingleton;
}
CInputManager& CInputManager::getSingleton(void)
{  
    assert( msSingleton );  return ( *msSingleton );  
}
// END SINGLETON

CInputManager::CInputManager()
: mInputManager( NULL )
, mKeyboard( NULL )
, mMouse( NULL )
, mJoyStick( NULL )
, mUseBufferedInputKeys(false)
, mUseBufferedInputMouse(false)
, mUseBufferedInputJoyStick(false)
, mInputTypeSwitchingOn(false)
, mInitialized(false)
, mEnabled(true)
{
        //
        // TODO Constructor
        //
}

CInputManager::~CInputManager()
{
        //
        // TODO Destructor
        //
        if(mInitialized)
        {
                finalize();
        }
}

/** Initialize the input manager */
bool CInputManager::initialize( Ogre::RenderWindow* window, bool showDefaultMousePointer )
{       
        size_t windowHandle = 0;
        window->getCustomAttribute( "WINDOW", &windowHandle );

        std::ostringstream windowHandleString;
        windowHandleString << windowHandle;

        OIS::ParamList parameters;
        parameters.insert( std::make_pair( std::string( "WINDOW" ), windowHandleString.str() ) );
        if ( showDefaultMousePointer )
        {
                parameters.insert( std::make_pair( std::string( "w32_mouse" ), std::string( "DISCL_FOREGROUND" ) ) );
                parameters.insert( std::make_pair( std::string( "w32_mouse" ), std::string( "DISCL_EXCLUSIVE" ) ) );
                parameters.insert( std::make_pair( std::string( "w32_keyboard" ), std::string( "DISCL_FOREGROUND" ) ) );
                parameters.insert( std::make_pair( std::string( "w32_keyboard" ), std::string( "DISCL_EXCLUSIVE" ) ) );
                parameters.insert( std::make_pair( std::string( "x11_mouse_grab" ), std::string( "false" ) ) );
                parameters.insert( std::make_pair( std::string( "x11_mouse_hide" ), std::string( "false" ) ) );
                parameters.insert( std::make_pair( std::string( "x11_keyboard_grab" ), std::string( "false" ) ) );
                parameters.insert( std::make_pair( std::string( "XAutoRepeatOn" ), std::string( "true" ) ) );
        }

        mInputManager = OIS::InputManager::createInputSystem( parameters );

        mKeyboard = static_cast< OIS::Keyboard* >( mInputManager->createInputObject( OIS::OISKeyboard, true ) );
        mMouse = static_cast< OIS::Mouse* >( mInputManager->createInputObject( OIS::OISMouse, true ) );
        //mJoyStick = static_cast< OIS::JoyStick* >( mInputManager->createInputObject( OIS::OISJoyStick, true ) );

        const OIS::MouseState& mouseState = mMouse->getMouseState();

        mouseState.width = window->getWidth();
        mouseState.height = window->getHeight();

        mMouse->setEventCallback( this );
        mKeyboard->setEventCallback( this );
        //mJoyStick->setEventCallback( this );

        mInitialized = true;

        return mInitialized;
}

void CInputManager::finalize()
{
        if ( mInputManager )
        {
                if ( mMouse )
                {
                        mInputManager->destroyInputObject( mMouse );
                        mMouse = 0;
                }

                if ( mKeyboard )
                {
                        mInputManager->destroyInputObject( mKeyboard );
                        mKeyboard = 0;
                }

                if( mJoyStick )
                {
                        mInputManager->destroyInputObject( mJoyStick );
                        mJoyStick = 0;
                }

                OIS::InputManager::destroyInputSystem( mInputManager );
                mInputManager = 0;
        }

        mInitialized = false;
}

/** Add input listener */
void CInputManager::addListener(CInputListener* listener)
{
        // TODO Check if listener is already present!
        this->mRegisteredListeners.push_back(listener);
}

/** Remove input listener */
void CInputManager::removeListener(CInputListener* listener)
{
        // TODO Remove listener (if present)!
        unsigned int i;
        for (i=0; i<mRegisteredListeners.size(); i++)
                if(mRegisteredListeners[i] == listener) 
                        break;

        if(i < mRegisteredListeners.size())
                mRegisteredListeners.erase(mRegisteredListeners.begin()+i);
}

/** Acquire all inputs */
void CInputManager::acquireAll()
{
        //
        // TBD Is this really needed?
        //
}

/** Relese all inputs */
void CInputManager::unacquireAll()
{
        //
        // TBD Is this really needed?
        //
}

void CInputManager::capture()
{
        mMouse->capture();
        this->mKeyboard->capture();
        //mJoyStick->capture(); // TODO Set up joystick properly
}

/** Get Keyboard */
OIS::Keyboard* CInputManager::getKeyboard()
{
        return mKeyboard;
}

/** Get Mouse */
OIS::Mouse* CInputManager::getMouse()
{
        return mMouse;
}

/** Get Joystick */
OIS::JoyStick* CInputManager::getJoyStick()
{
        return mJoyStick;
}

/** Switches mouse mode (buffered/unbuffered) */
void CInputManager::switchMouseMode()
{
        mUseBufferedInputMouse = !mUseBufferedInputMouse;
        mMouse->setBuffered(mUseBufferedInputMouse);
}

/** Switches keyboard mode (buffered/unbuffered) */
void CInputManager::switchKeyMode()
{
        mUseBufferedInputKeys = !mUseBufferedInputKeys;
        mKeyboard->setBuffered(mUseBufferedInputKeys);
}

/** Switches joystick mode (buffered/unbuffered) */
void CInputManager::switchJoyStickMode()
{
        mUseBufferedInputJoyStick = !mUseBufferedInputJoyStick;
        mJoyStick->setBuffered(mUseBufferedInputJoyStick);
}

/** Set mouse mode (true:buffered/false:unbuffered) */
void CInputManager::setMouseMode(const bool isBuffered)
{
        mUseBufferedInputMouse = isBuffered;
        mMouse->setBuffered(mUseBufferedInputMouse);
}

/** Set keyboard mode (true:buffered/false:unbuffered) */
void CInputManager::setKeyMode(const bool isBuffered)
{
        mUseBufferedInputKeys = isBuffered;
        mKeyboard->setBuffered(mUseBufferedInputKeys);
}

/** Set joystick mode (true:buffered/false:unbuffered) */
void CInputManager::setJoyStickMode(const bool isBuffered)
{
        mUseBufferedInputJoyStick = isBuffered;
        //mJoyStick->setBuffered(mUseBufferedInputJoyStick);
}

// ---------------
// Mouse Listeners
// ---------------
/** Buffered input - mouse moved */
bool CInputManager::mouseMoved( const OIS::MouseEvent& e ) 
{
        if(mEnabled)
        {
                WA_CALL_LISTENERS(mouseMoved(e))
                return true;
        }

        return false;
}

/** Buffered input - mouse button pressed */
bool CInputManager::mousePressed( const OIS::MouseEvent& e, OIS::MouseButtonID button )
{
        if(mEnabled)
        {
                WA_CALL_LISTENERS(mousePressed(e, button))
                return true;
        }

        return false;
}

/** Buffered input - mouse button released */
bool CInputManager::mouseReleased( const OIS::MouseEvent& e, OIS::MouseButtonID button )
{
        if(mEnabled)
        {
                WA_CALL_LISTENERS(mouseReleased(e, button))
                return true;
        }

        return false;
}

// ------------------
// Keyboard listeners
// ------------------
/** Buffered input - keyboard key clicked */
bool CInputManager::keyClicked(const OIS::KeyEvent& e)
{
        if(mEnabled)
        {
                WA_CALL_LISTENERS(keyClicked(e))
                return true;
        }

        return false;
}

/** Buffered input - keyboard key clicked */
bool CInputManager::keyPressed( const OIS::KeyEvent& e )
{
        if(mEnabled)
        {
                WA_CALL_LISTENERS(keyPressed(e))
                return true;
        }

        return false;
}

/** Buffered input - keyboard key clicked */
bool CInputManager::keyReleased( const OIS::KeyEvent& e )
{
        if(mEnabled)
        {
                WA_CALL_LISTENERS(keyReleased(e))
                return true;
        }

        return false;
}

// ----------------
// JoyStickListener
// ----------------
/** Buffered input - joystick button pressed */
bool CInputManager::buttonPressed(const OIS::JoyStickEvent &evt, int index)
{
        if(mEnabled)
        {
                WA_CALL_LISTENERS(buttonPressed(evt,index))
                return true;
        }

        return false;
}

/** Buffered input - joystick button released */
bool CInputManager::buttonReleased(const OIS::JoyStickEvent &evt, int index)
{
        if(mEnabled)
        {
                WA_CALL_LISTENERS(buttonReleased(evt,index))
                return true;
        }

        return false;
}

/** Buffered input - axis pad moved */
bool CInputManager::axisMoved(const OIS::JoyStickEvent &evt, int index)
{
        if(mEnabled)
        {
        WA_CALL_LISTENERS(axisMoved(evt,index))
        return true;
        }

        return false;
}

/** Buffered input - pov moved */
bool CInputManager::povMoved(const OIS::JoyStickEvent &evt, int index)
{
        if(mEnabled)
        {
        WA_CALL_LISTENERS(povMoved(evt,index))
        return true;
        }

        return false;
}

