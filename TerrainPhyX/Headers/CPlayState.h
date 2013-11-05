#ifndef __CPLAY_STATE_H_
#define __CPLAY_STATE_H_

//##### GAME INCLUDES
#include "CBaseState.h"
#include "CPhyXManager.h"
#include "CWorldManager.h"
//##### GAME INCLUDES

/**
        Play game state class
*/
class CPlayState : public CBaseState
{
	public:
			CPlayState(CGraphicsManager* aCGraphicsManager, 
					   CInputManager* aCInputManager);

			~CPlayState();
	
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
			bool mousePressed(const OIS::MouseEvent& e, OIS::MouseButtonID id);
			bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);

	private:
			
			void createGameInfo();

			Ogre::Camera*				mCamera;
			OgreBites::SdkCameraMan*	mCameraMan;
			OgreBites::SdkTrayManager*	mTrayMgr;
			OgreBites::ParamsPanel*		mDetailsPanel;
			CPhysicsManager*			mPhysicsManager;
			CWorldManager*				mWorldManager;
			Ogre::SceneNode*			mRootSceneNode;
			float						mElapsedSeconds;
			bool						mActiveDebugPhysX;

	protected:

			float                   buttonTimer;
			OIS::KeyCode			lastKey;

			bool                    mDebugEnabled;
			//CHydraxManager*			mHydraxManager;
};

#endif // __CPLAY_STATE_H_