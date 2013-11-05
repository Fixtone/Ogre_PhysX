#include "../Headers/CPlayState.h"

CPlayState::CPlayState(CGraphicsManager* aCGraphicsManager, 
                       CInputManager* aCInputManager)
: CBaseState(aCGraphicsManager,aCInputManager)
, mRootSceneNode(0)
, mElapsedSeconds(0.0f)
, mDebugEnabled(false)
, mPhysicsManager(false)
{
    //
    // TODO Constructor logic HERE
    //        
}

CPlayState::~CPlayState()
{
        //
        // TODO Distructor logic HERE
        //
        finalize();
}

/** Initialize current state */
void CPlayState::initialize()
{	
	CBaseState::initialize();

	//Create camera
	mCamera = mSceneManager->createCamera("GameCamera");
	mCamera->setPosition(0.0f,600.0f,0.0f);
	mCamera->setNearClipDistance(0.05f);
	mCamera->setFarClipDistance(600);
	// Create one viewport, entire window
    Ogre::Viewport* vp = mWindow->addViewport(mCamera);
    vp->setBackgroundColour(Ogre::ColourValue(0,0,0));
	mCamera->setAspectRatio(Ogre::Real(mWindow->getViewport(0)->getActualWidth()) / Ogre::Real(mWindow->getViewport(0)->getActualHeight()));
	mWindow->getViewport(0)->setCamera(mCamera);

	mCameraMan = new OgreBites::SdkCameraMan(mCamera);

	mDebugEnabled = false;

	mCInputManager->setKeyMode(true);
	mCInputManager->setMouseMode(true);
	mCInputManager->setJoyStickMode(true);
		
	mCInputManager->setKeyMode(true);
	mCInputManager->setMouseMode(true);
	mCInputManager->setJoyStickMode(true);

	this->mNextGameStateId = this->getStateId();

	// Add Playstate as frame listener
	mRoot->addFrameListener(this);

	//#################Physics (NxOgre)
	mPhysicsManager = new CPhysicsManager(mSceneManager);
	//#################Physics (NxOgre)

	//#################Shadows
	mSceneManager->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_MODULATIVE);
	//#################Shadows

	//#################SkyBox	 
	mSceneManager->setSkyBox(true,"ViolentDays", 99999*3,true);
	//#################SkyBox

	//#################Lighting
	Ogre::Vector3 lightdir(0.5f,-0.9f,0.5f);
	lightdir.normalise();
	mSceneManager->setShadowFarDistance(60.0f);
	Ogre::Light* l = mSceneManager->createLight("dirLight");
	l->setType(Ogre::Light::LT_DIRECTIONAL);
	l->setDirection(lightdir);
	l->setDiffuseColour(Ogre::ColourValue(0.97f, 0.90f, 0.70f));
	l->setSpecularColour(Ogre::ColourValue(0.2f, 0.2f, 0.2f));

	mSceneManager->setAmbientLight(Ogre::ColourValue(0.2f, 0.2f, 0.2f));
	//#################Lighting
	
	//#################Terrain
	 //Init the World Manager class.
	mWorldManager = new CWorldManager(mSceneManager, mCamera, mPhysicsManager->getPhysicsScene(),l);

	//Terrain texture layers.
	Ogre::String difSpecMap = "dirt_grayrocky_diffusespecular.dds";
	Ogre::String normHeightMap = "dirt_grayrocky_normalheight.dds";
	mWorldManager->addTextureLayer(difSpecMap, normHeightMap , 20.0f);
	difSpecMap = "grass_green-01_diffusespecular.dds";
	normHeightMap = "grass_green-01_normalheight.dds";
	mWorldManager->addTextureLayer(difSpecMap,normHeightMap , 30.0f);
	difSpecMap = "Sand.dds";
	normHeightMap = "Sand_Normal.dds";
	mWorldManager->addTextureLayer(difSpecMap, normHeightMap, 200.0f);

	mWorldManager->initTerrain();

	 //Create the terrain in NxOgre.
	mWorldManager->buildNxOgreTerrain();
	//#################Terrain

	this->createGameInfo();

	buttonTimer = 0.0;
	lastKey = OIS::KeyCode::KC_UNASSIGNED;

	//mHydraxManager = new CHydraxManager();
	//mHydraxManager->createHydrax(mRoot,mSceneManager,camera,mWindow->getViewport(0));
}

/** Load resources */
void CPlayState::load()
{
        //
        // TODO: Load?
        //
}

/** Manage input */
void CPlayState::input()
{
        //
        // TODO Read input
        //
        mCInputManager->capture();
}

/** Rendering queue */
bool CPlayState::frameRenderingQueued(const Ogre::FrameEvent& evt)
{ 
	//mCCameraManager->frameRenderingQueued(evt);
	mTrayMgr->frameRenderingQueued(evt);
	mCameraMan->frameRenderingQueued(evt);

	if (mDetailsPanel->isVisible())   // if details panel is visible, then update its contents
	{
		mDetailsPanel->setParamValue(0, Ogre::StringConverter::toString(mCamera->getDerivedPosition().x));
		mDetailsPanel->setParamValue(1, Ogre::StringConverter::toString(mCamera->getDerivedPosition().y));
		mDetailsPanel->setParamValue(2, Ogre::StringConverter::toString(mCamera->getDerivedPosition().z));
		mDetailsPanel->setParamValue(4, Ogre::StringConverter::toString(mCamera->getDerivedOrientation().w));
		mDetailsPanel->setParamValue(5, Ogre::StringConverter::toString(mCamera->getDerivedOrientation().x));
		mDetailsPanel->setParamValue(6, Ogre::StringConverter::toString(mCamera->getDerivedOrientation().y));
		mDetailsPanel->setParamValue(7, Ogre::StringConverter::toString(mCamera->getDerivedOrientation().z));
	}

	mPhysicsManager->update(evt.timeSinceLastFrame);

    return true;
}

/** Update internal stuff */
void CPlayState::update(const float elapsedSeconds)
{
	//mHydraxManager->update(elapsedSeconds);
	mElapsedSeconds = elapsedSeconds;   	
}

/** Render */
void CPlayState::render(const float elapsedSeconds)
{
        //
        // TODO
        //
}

/** Unload resources */
void CPlayState::unload() 
{  
    if(mRoot)
		mRoot->removeFrameListener(this);    
}

/** Destroy the state */
void CPlayState::finalize()
{
	if(mWorldManager)
	{		
		delete mWorldManager;
	}

	if(mPhysicsManager)
	{
		mPhysicsManager->destroyPhysicsWorld();
		delete mPhysicsManager;
	}

	if(mTrayMgr)
		delete mTrayMgr;

	if(mCameraMan)
		delete mCameraMan;

    CBaseState::finalize();


    //Debug::Out("**************************");
}

/** Get state Id */
GameStateId CPlayState::getStateId()
{
    //
    // BEWARE! Make sure this is the correspondent state or we fuck up everything!
    //
    return GameStateId::Play;
}

/** Called when the state has to be paused */
void CPlayState::pause()
{    

    // Hide axes if visible
    //if(mDebugEnabled)
}

/** Called when the state has to be resumed (from pause) */
void CPlayState::resume()
{
    //
    // Set next state to this state (Play)  
    //
    this->mNextGameStateId = this->getStateId();		
   

    // Show axes if visible
    //if(mDebugEnabled)        
}

/** Buffered input - keyboard key clicked */
bool CPlayState::keyPressed(const OIS::KeyEvent& e)
{
	Ogre::String newVal;
	Ogre::PolygonMode pm;

	switch(e.key)
    {
		case OIS::KeyCode::KC_ESCAPE:
			mPhysicsManager->destroyPhysicsBodies();
			mWorldManager->destroyTerrain();
			this->mNextGameStateId = GameStateId::Exit;
		break;
		case OIS::KeyCode::KC_0:
			if(mActiveDebugPhysX)
			{
				mPhysicsManager->setDebugPhysX(false);
				mActiveDebugPhysX = false;
			}
			else
			{
				mPhysicsManager->setDebugPhysX(true);
				mActiveDebugPhysX = true;
			}
		break;
		case OIS::KeyCode::KC_B:
			mPhysicsManager->makeBox(NxOgre::Vec3(mCamera->getPosition().x,mCamera->getPosition().y,mCamera->getPosition().z),NxOgre::Vec3(0,0,-10));
		break;
		case OIS::KeyCode::KC_F:
			mTrayMgr->toggleAdvancedFrameStats();
		break;
		case OIS::KeyCode::KC_G:
			if (mDetailsPanel->getTrayLocation() == OgreBites::TL_NONE)
			{
				mTrayMgr->moveWidgetToTray(mDetailsPanel, OgreBites::TL_TOPRIGHT, 0);
				mDetailsPanel->show();
			}
			else
			{
				mTrayMgr->removeWidgetFromTray(mDetailsPanel);
				mDetailsPanel->hide();
			}
		break;
		case OIS::KeyCode::KC_T:
			Ogre::TextureFilterOptions tfo;
			unsigned int aniso;

			switch (mDetailsPanel->getParamValue(9).asUTF8()[0])
			{
				case 'B':
					newVal = "Trilinear";
					tfo = Ogre::TFO_TRILINEAR;
					aniso = 1;
				break;
				case 'T':
					newVal = "Anisotropic";
					tfo = Ogre::TFO_ANISOTROPIC;
					aniso = 8;
				break;
				case 'A':
					newVal = "None";
					tfo = Ogre::TFO_NONE;
					aniso = 1;
				break;
				default:
					newVal = "Bilinear";
					tfo = Ogre::TFO_BILINEAR;
					aniso = 1;
			}
			Ogre::MaterialManager::getSingleton().setDefaultTextureFiltering(tfo);
			Ogre::MaterialManager::getSingleton().setDefaultAnisotropy(aniso);
			mDetailsPanel->setParamValue(9, newVal);
		break;
		case OIS::KeyCode::KC_R:
		{
			switch (mCamera->getPolygonMode())
			{
				case Ogre::PM_SOLID:
					newVal = "Wireframe";
					pm = Ogre::PM_WIREFRAME;
				break;
				case Ogre::PM_WIREFRAME:
					newVal = "Points";
					pm = Ogre::PM_POINTS;
				break;
				default:
					newVal = "Solid";
					pm = Ogre::PM_SOLID;
			}

			mCamera->setPolygonMode(pm);
			mDetailsPanel->setParamValue(10, newVal);
			break;
		}		
		case OIS::KeyCode::KC_F5:
			Ogre::TextureManager::getSingleton().reloadAll();
		break;
	}

	// Free camera mode move
	//if(mCCameraManager->getCameraMode() == CCameraManager::CameraManagerEnumerators::Free)
    //{
    //       //if( e.key != OIS::KC_UP && e.key != OIS::KC_DOWN &&     e.key != OIS::KC_RIGHT && e.key != OIS::KC_LEFT && e.key != OIS::KC_SPACE && e.key != OIS::KC_LCONTROL )
    //                mCCameraManager->freeCameraKeyboardDown(e);
    //}

	mCameraMan->injectKeyDown(e);

	return true;
}

bool CPlayState::keyReleased(const OIS::KeyEvent& e)
{
	// Free camera mode move
   // if(mCCameraManager->getCameraMode() == CCameraManager::CameraManagerEnumerators::Free)
    //{
            //if( e.key != OIS::KC_UP && e.key != OIS::KC_DOWN &&     e.key != OIS::KC_RIGHT && e.key != OIS::KC_LEFT && e.key != OIS::KC_SPACE && e.key != OIS::KC_LCONTROL )
    //                mCCameraManager->freeCameraKeyboardUp(e);
    //}
	mCameraMan->injectKeyUp(e);
    return true;
}

/** Mouse input */
bool CPlayState::mouseMoved(const OIS::MouseEvent& e)
{    
	mCameraMan->injectMouseMove(e);
	// mCCameraManager->freeCameraMouse(e);
	return true;
}

bool CPlayState::mousePressed(const OIS::MouseEvent& arg, OIS::MouseButtonID id)
{
	mCameraMan->injectMouseDown(arg, id);
	return true;
}

bool CPlayState::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
	mCameraMan->injectMouseUp(arg, id);
	return true;
}

void CPlayState::createGameInfo()
{
	mTrayMgr = new OgreBites::SdkTrayManager("InterfaceName", mWindow, mCInputManager->getMouse(), this);
	mTrayMgr->showFrameStats(OgreBites::TL_BOTTOMLEFT);
	mTrayMgr->hideCursor();

	// create a params panel for displaying sample details
    Ogre::StringVector items;
    items.push_back("cam.pX");
    items.push_back("cam.pY");
    items.push_back("cam.pZ");
    items.push_back("");
    items.push_back("cam.oW");
    items.push_back("cam.oX");
    items.push_back("cam.oY");
    items.push_back("cam.oZ");
    items.push_back("");
    items.push_back("Filtering");
    items.push_back("Poly Mode");

    mDetailsPanel = mTrayMgr->createParamsPanel(OgreBites::TL_NONE, "DetailsPanel", 200, items);
    mDetailsPanel->setParamValue(9, "Bilinear");
    mDetailsPanel->setParamValue(10, "Solid");
    mDetailsPanel->hide();
}