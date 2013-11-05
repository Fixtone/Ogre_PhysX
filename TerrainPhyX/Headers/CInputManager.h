#ifndef __CINPUT_MANAGER_H_
#define __CINPUT_MANAGER_H_

//##### OGRE INCLUDES
#include <Ogre.h>
#include <OgreRenderWindow.h>
#include <ois/OIS.h>
//##### OGRE INCLUDES

//##### STD INCLUDES
#include <vector>
//##### STD INCLUDES

//##### GAME INCLUDES
#include "../Headers/CInputListener.h"
//##### GAME INCLUDES

#define WA_CALL_LISTENERS(method) for ( InputListenersIterator it=mRegisteredListeners.begin() ; it < mRegisteredListeners.end(); it++ ) {(*it)->method;}

/** The input listenrs list */
typedef std::vector<CInputListener*> InputListenersList;
typedef InputListenersList::iterator InputListenersIterator;

/**
        Class used to manager user input, from keyboard, mouse, joystick and so on...
*/
class CInputManager : public Ogre::Singleton<CInputManager>                                        
                    , public OIS::MouseListener
                    , public OIS::KeyListener
                    , public OIS::JoyStickListener                   
{
	public:

			CInputManager();
			~CInputManager();
			static CInputManager& getSingleton(void);
			static CInputManager* getSingletonPtr(void);

	public:
			/** Initialize the input manager */
			bool initialize( Ogre::RenderWindow* window, bool showDefaultMousePointer );
			/** Finalize the input manager */
			void finalize();
			/** Add input listener */
			void addListener(CInputListener* listener);
			/** Remove input listener */
			void removeListener(CInputListener* listener);

			/** Acquire all inputs */
			void acquireAll();
			/** Relese all inputs */
			void unacquireAll();
			/** Read all user inputs */
			void capture();

			/** Get Keyboard */
			OIS::Keyboard* getKeyboard();
			/** Get Mouse */
			OIS::Mouse* getMouse();
			/** Get Joystick */
			OIS::JoyStick* getJoyStick();
                
			/** Switches mouse mode (buffered/unbuffered) */
			virtual void switchMouseMode();
			/** Switches keyboard mode (buffered/unbuffered) */
			virtual void switchKeyMode();
			/** Switches joystick mode (buffered/unbuffered) */
			virtual void switchJoyStickMode();

			/** Set mouse mode (true:buffered/false:unbuffered) */
			virtual void setMouseMode(const bool isBuffered);
			/** Set keyboard mode (true:buffered/false:unbuffered) */
			virtual void setKeyMode(const bool isBuffered);
			/** Set joystick mode (true:buffered/false:unbuffered) */
			virtual void setJoyStickMode(const bool isBuffered);

			//
			// Keyboard listeners
			//
			/** Buffered input - keyboard key clicked */
			virtual bool keyClicked(const OIS::KeyEvent& e);
			/** Buffered input - keyboard key clicked */
			virtual bool keyPressed(const OIS::KeyEvent& e);
			/** Buffered input - keyboard key clicked */
			virtual bool keyReleased(const OIS::KeyEvent& e);

			//
			// MouseListener
			//
			/** Buffered input - mouse moved */
			bool mouseMoved(const OIS::MouseEvent &evt);
			/** Buffered input - mouse button pressed */
			bool mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID);
			/** Buffered input - mouse button released */
			bool mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID);
        
			//
			// JoyStickListener
			//
			/** Buffered input - joystick button pressed */
			bool buttonPressed(const OIS::JoyStickEvent &evt, int index);
			/** Buffered input - joystick button released */
			bool buttonReleased(const OIS::JoyStickEvent &evt, int index);
			/** Buffered input - axis pad moved */
			bool axisMoved(const OIS::JoyStickEvent &evt, int index);
			/** Buffered input - pov moved */
			bool povMoved(const OIS::JoyStickEvent &evt, int index);

			//
			// Enable/disable input
			//
			void disable(){mEnabled = false;}
			void enable(){mEnabled = true;}
			bool isEnabled(){return mEnabled;}

	protected:

			OIS::InputManager* mInputManager;
			OIS::Keyboard* mKeyboard;
			OIS::Mouse* mMouse;
			OIS::JoyStick* mJoyStick;
                
			bool mUseBufferedInputMouse;
			bool mUseBufferedInputKeys;
			bool mUseBufferedInputJoyStick;
			bool mInputTypeSwitchingOn;

			InputListenersList mRegisteredListeners;

			bool mInitialized;
			bool mEnabled;
};

#endif // __INPUT_MANAGER_H_