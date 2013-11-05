#include "../Headers/CPhyXManager.h"

// BEGIN SINGLETON
template<> CPhysicsManager* Ogre::Singleton<CPhysicsManager>::msSingleton = 0;
CPhysicsManager* CPhysicsManager::getSingletonPtr(void)
{
    return msSingleton;
}
CPhysicsManager& CPhysicsManager::getSingleton(void)
{  
    assert( msSingleton );  return ( *msSingleton );  
}
// END SINGLETON

CPhysicsManager::CPhysicsManager(Ogre::SceneManager* aSceneManager)
{
	mSceneMgr = aSceneManager;
	mEntityCounter = 0;

	// Create the world.
	mPhysicsWorld = NxOgre::World::createWorld();
  
	// Allow NxOgre to use the Ogre resource system, via the Critter OgreResourceProtocol class.
	// - ResourceProtocols are normally owned by the ResourceSystem, so we don't have to delete it later.
	NxOgre::ResourceSystem::getSingleton()->openProtocol(new Critter::OgreResourceProtocol());

	mPhysicsWorld->getRemoteDebugger()->connect();

	// Create the scene
	NxOgre::SceneDescription scene_description;
	scene_description.mGravity = NxOgre::Vec3(0, -9.8f, 0);
	scene_description.mName = "GameScene";
	scene_description.mUseHardware = false;

	mPhysicsScene = mPhysicsWorld->createScene(scene_description);
  
	// Set default material properties
	mDefaultMaterial = mPhysicsScene->getMaterial(0);
	mDefaultMaterial->setRestitution(0.1f);
	mDefaultMaterial->setDynamicFriction(0.9);
	mDefaultMaterial->setStaticFriction(0.5);
  
	// The NxOgre Mesh Manager is responsible for loading and storing all Meshes (Triangle, Convex, Cloth, etc.)
	// - Let's get a copy of it's pointer
	NxOgre::MeshManager* mMeshManager = NxOgre::MeshManager::getSingleton();	

	// Create the rendersystem.
	mRenderSystem = new Critter::RenderSystem(mPhysicsScene, mSceneMgr);
}

CPhysicsManager::~CPhysicsManager(){}

void CPhysicsManager::destroyPhysicsWorld()
{
	//Delete all the physics.
	if(mRenderSystem)
		delete mRenderSystem;

	mPhysicsWorld->destroyWorld();
	mPhysicsWorld->destroySingletons();

}

void CPhysicsManager::destroyPhysicsBodies()
{
	BOOST_FOREACH(Critter::Body* body, mBoxVector)
	{	
		mRenderSystem->destroyBody(body);
	}
	mBoxVector.clear();
}

void CPhysicsManager::update(const double timeSinceLastFrame)
{
	//Steps the physics scene with timeSinceLastFrame seconds.
	mPhysicsWorld->advance(timeSinceLastFrame);
}

Ogre::String CPhysicsManager::PhysxFilename(const char* filename)
{
	//Adds nxogre: at the start of the "filename" char array and ".nxs" at the end.
	char nxOgreFileName[80] = "nxogre:";
	std::strcat(nxOgreFileName,filename);
	std::strcat(nxOgreFileName,".nxs");
	std::cout << "INUTI ÄR DEN = " << nxOgreFileName;
	return Ogre::String(nxOgreFileName);
}

/*---------------------------------------------------------------------------------*/
Ogre::String CPhysicsManager::OgreFilename(const char* filename)
{
	//Adds ".mesh" at the end of "filename".
	char ogreFileName[80] = "";
	std::strcat(ogreFileName,filename);
	std::strcat(ogreFileName,".mesh");
	std::cout << "INUTI ÄR DEN = " << ogreFileName;
	return Ogre::String(ogreFileName);
}

Critter::Body* CPhysicsManager::makeBox(const NxOgre::Matrix44& globalPose, const NxOgre::Vec3& initialVelocity, NxOgre::GroupIdentifier group)
{
  
	Critter::BodyDescription bodyDescription;
	bodyDescription.mMass = 40.0f;
	bodyDescription.mLinearVelocity = initialVelocity;
	bodyDescription.mName = "Box_cube.1m";

	NxOgre::BoxDescription box_desc;
	box_desc.mSize.set(1,1,1);
	box_desc.mGroup = group;	
  
	Critter::Body* box = mRenderSystem->createBody(box_desc, globalPose, "cube.1m.mesh", bodyDescription);	
	box->setContactCallback(this);
	
	//if(mBoxVector.size()>1)
	mPhysicsScene->setActorFlags(box,CWorldManager::getSingletonPtr()->getTerrainSceneGeometry() , NxOgre::Enums::ContactPairFlags_All);
			
	

	mBoxVector.push_back(box);

	return box;
}

void CPhysicsManager::onContact(const NxOgre::ContactPair& pair)
{
	/*Ogre::LogManager::getSingletonPtr()->logMessage("##############COLLISION##############");
	Ogre::LogManager::getSingletonPtr()->logMessage(pair.mFirst->getName());
	Ogre::LogManager::getSingletonPtr()->logMessage(pair.mSecond->getName());*/
}

void CPhysicsManager::onVolumeEvent(NxOgre::Volume* volume, NxOgre::Shape* volumeShape, NxOgre::RigidBody* collisionBody, NxOgre::Shape* rigidBodyShape, unsigned int collisionEvent)
{
	// Only want actors or derivatives (Bodies)
	//if (collisionBody->isActorBased() == false)
	//return;

	// Cast it into the lowest possible class known, which is an Actor. Bodies are Actors, so this is okay
	// If I wanted to be more specific, I could use collisionBody->getRigidBodyType() to distinguish if it
	// is a normal Actor a Critter Body.

	//NxOgre::Actor* actor = static_cast<NxOgre::Actor*>(collisionBody);

}

void CPhysicsManager::setDebugPhysX(bool aActive)
{
	if(aActive)
		mRenderSystem->setVisualisationMode(NxOgre::Enums::VisualDebugger::VisualDebugger_Body_Axes);
	else
		mRenderSystem->setVisualisationMode(NxOgre::Enums::VisualDebugger::VisualDebugger_ShowNone);
}
