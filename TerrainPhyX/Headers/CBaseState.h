#ifndef __CBASE_STATE_H_
#define __CBASE_STATE_H_

//##### OGRE INCLUDES
#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>
#include <SdkTrays.h>
#include <SdkCameraMan.h>
//##### OGRE INCLUDES

//##### GAME INCLUDES
#include "CGraphicsManager.h"
#include "CInputManager.h"
#include "CInputListener.h"
//##### GAME INCLUDES

#pragma warning(push)
#pragma warning(disable : 4482)
/** List of game states of the game */
enum GameStateId
{
    Play    = 0, // Splash screen      
	Exit	= 1
};
#pragma warning(pop)

/**
Base game state class
*/
class CBaseState : public CInputListener
                 , public Ogre::FrameListener
				 , public OgreBites::SdkTrayListener
{
public:
        CBaseState();
        
		CBaseState(CGraphicsManager* aCGraphicsManager, 
                   CInputManager* aCInputManager);

        virtual ~CBaseState() = 0;

public:

        /** Initialize current state */
        virtual void initialize() = 0;
        /** Load resources */
        virtual void load() = 0;
        /** Manage input - INPUT */
        virtual void input() = 0;
        /** Update internal stuff - PROCESS */
        virtual void update(const float elapsedSeconds) = 0;
        /** Render - OUTPUT */
        virtual void render(const float elapsedSeconds) = 0;
        /** Unload resources */
        virtual void unload() = 0;
        /** Destroy the state */
        virtual void finalize() = 0;

        /** Called the first time we enter the state */
        void enter();
        /** Called when we exit from the state */
        void exit();
        /** Called when the state has to be paused */
        virtual void pause() = 0;
        /** Called when the state has to be resumed (from pause) */
        virtual void resume() = 0;

        /** Get state Id */
        virtual GameStateId getStateId() = 0;
        /** Get NEXT state Id */
        GameStateId getNextStateId() { return mNextGameStateId; };

        bool mIsPaused;

protected:

        /** State is paused */
        //bool mIsPaused;
        /** State is active */
        bool mIsActive;
        /** State is dead */
        bool mIsDead;

protected:

        /** Graphics manager */
        CGraphicsManager* mCGraphicsManager;
        
		/** Input manager */
        CInputManager* mCInputManager;

        /** Next Game State Id */
        GameStateId mNextGameStateId;       

protected:

        Ogre::Root*			mRoot;
        Ogre::RenderWindow* mWindow;       
        Ogre::SceneManager* mSceneManager;
};

#endif // __CSBASE_STATE_H_