#ifndef __CEXIT_STATE_H_
#define __CEXIT_STATE_H_

//##### GAME INCLUDES
#include "CBaseState.h"
//##### GAME INCLUDES

/**
        Play game state class
*/
class CExitState : public CBaseState
{
	public:
			CExitState(CGraphicsManager* aCGraphicsManager, 
					   CInputManager* aCInputManager);

			~CExitState();
	
			/** Initialize current state */
			void initialize();
			/** Load resources */	
			void load();
			/** Manage input - INPUT */
			void input();
			/**  Renderin queue */
			bool frameRenderingQueued(const Ogre::FrameEvent& evt);
			/** Update internal stuff - PROCESS */
			void update(const float elapsedSeconds);
			/** Render - OUTPUT */
			void render(const float elapsedSeconds);
			/** Unload resources */
			void unload();
			/** Destroy the state */
			void finalize();
			/** Called when the state has to be paused */
			void pause();
			/** Called when the state has to be resumed (from pause) */
			void resume();

			/** Get state Id */
			GameStateId getStateId();

			/** Buffered input - keyboard key clicked */
			bool keyPressed(const OIS::KeyEvent& e);
			bool keyReleased(const OIS::KeyEvent& e);

			/** Mouse input */
			bool mouseMoved(const OIS::MouseEvent& e);

	private:			

			Ogre::SceneNode* mRootSceneNode;
			//
			// Performances timers
			//
			float mElapsedSeconds;			

	protected:

			float                   buttonTimer;
			OIS::KeyCode			lastKey;

			bool                    mDebugEnabled;			
};

#endif // __CEXIT_STATE_H_
