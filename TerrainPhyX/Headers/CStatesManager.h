/*
-----------------------------------------------------------------------------
Wyverns' Assault 3D Videgame.
Copyright (C) 2010  Giorgio Tino, Javier Soto Huesa, Jordi Carreras Ribot, 
                                        Marc Serena, Elm Oliver Torres

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

-----------------------------------------------------------------------------
*/
#ifndef __CSTATES_MANAGER_H_
#define __CSTATES_MANAGER_H_

//##### OGRE INCLUDES
#include <Ogre.h>
#include "OISEvents.h"
#include "OISInputManager.h"
#include "OISMouse.h"
#include "OISKeyboard.h"
#include "OISJoyStick.h"
//##### OGRE INCLUDES

//##### STD INCLUDES
#include <map>
//##### STD INCLUDES

//##### GAME INCLUDES
#include "CBaseState.h"
#include "CPlayState.h"
#include "CExitState.h"
#include "CInputManager.h"
#include "CInputListener.h"
//##### GAME INCLUDES

        /** The games states map type */
        typedef std::map<GameStateId, CBaseState*> StatesMap;
        typedef StatesMap::iterator StatesMapIterator;

        /** The games states stack type */
        typedef std::vector<CBaseState*> StatesStack;
        typedef StatesStack::iterator StatesStackIterator;

        /** Game options */

        /** Game levels */

        /** Game score */

        /**
			Class used to manage the game states
        */
        class CStatesManager : public CInputListener
                             , public Ogre::Singleton<CStatesManager>                                                
        {
        public:
                
				CStatesManager();
                ~CStatesManager();

                static CStatesManager& getSingleton(void);
                static CStatesManager* getSingletonPtr(void);

        public:
                /** Initialize the manager */
			void initialize(CGraphicsManager* aCGraphicsManager, CInputManager* aCInputManager);
                /** Destroy and release all resources used by the manager */
                void finalize();
                /** Run the current state (main loop)*/
                bool loop(const float elapsedSeconds);

                //
                // Keyboard listeners
                //
                /** Buffered input - keyboard key clicked */
                bool keyClicked(const OIS::KeyEvent& e);
                /** Buffered input - keyboard key clicked */
                bool keyPressed(const OIS::KeyEvent& e);
                /** Buffered input - keyboard key clicked */
                bool keyReleased(const OIS::KeyEvent& e);

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

        private:
                /** Change state */
                void changeState(CBaseState* state);
                /** Push a state on the top of the stack */
                void pushState(CBaseState* state);
                /** Pop a state from the top of the stack */
                void popState();
                /** Retrive a game state reference by its ID */
                CBaseState* getGameStateById(const GameStateId id);

        private:
                /** Map of game states [ID - State]*/ 
                StatesMap mStates;
                /** States hitory stack (for push-pop info) */
                StatesStack mStatesStack;

                /** Current (active) game state */
                CBaseState* mCurrentState;
                /** Id of the current (active) game state */
                GameStateId mCurrentStateId;

                /** Graphics Manager */
                CGraphicsManager* mCGraphicsManager;
                /** Input Manager */
                CInputManager* mCInputManager; 

                bool mInitialized;
        };        


#endif // __CSTATES_MANAGER_H_