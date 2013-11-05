#ifndef __CINPUT_LISTENER_H__
#define __CINPUT_LISTENER_H__

//##### OGRE INCLUDES
#include <ois/OIS.h>
//##### OGRE INCLUDES

/**
Class used to listen to user input, from keyboard, mouse, joystick and so on...
*/

class CInputListener
{
	public:
			CInputListener();
			~CInputListener();

	public:
			//
			// Keyboard listeners
			//
			/** Buffered input - keyboard key clicked */
			virtual bool keyClicked(const OIS::KeyEvent& e){return true;}
			/** Buffered input - keyboard key clicked */
			virtual bool keyPressed(const OIS::KeyEvent& e){return true;}
			/** Buffered input - keyboard key clicked */
			virtual bool keyReleased(const OIS::KeyEvent& e){return true;}

			//
			// MouseListener
			//
			/** Buffered input - mouse moved */
			virtual bool mouseMoved(const OIS::MouseEvent &evt){return true;}
			/** Buffered input - mouse button pressed */
			virtual bool mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID){return true;}
			/** Buffered input - mouse button released */
			virtual bool mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID){return true;}
        
			//
			// JoyStickListener
			//
			/** Buffered input - joystick button pressed */
			virtual bool buttonPressed(const OIS::JoyStickEvent &evt, int index){return true;}
			/** Buffered input - joystick button released */
			virtual bool buttonReleased(const OIS::JoyStickEvent &evt, int index){return true;}
			/** Buffered input - axis pad moved */
			virtual bool axisMoved(const OIS::JoyStickEvent &evt, int index){return true;}
			/** Buffered input - pov moved */
			virtual bool povMoved(const OIS::JoyStickEvent &evt, int index){return true;}
};

#endif //__CINPUT_LISTENER_H__