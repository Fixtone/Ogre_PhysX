#ifndef C_WORLDMANAGER_H_
#define C_WORLDMANAGER_H_

//##### OGRE INCLUDES
#include "Ogre.h"
#include "OgreTerrain.h"
#include "OgreTerrainGroup.h"
#include "OgreTerrainQuadTreeNode.h"
#include "OgreTerrainMaterialGeneratorA.h"
//##### OGRE INCLUDES

//##### EXTERNALS INCLUDES
#include "../Externals/NxOgre/sdk/NxOgre.h"
#include "../Externals/Critter/sdk/Critter.h"
//##### EXTERNALS INCLUDES

#define TERRAIN_FILE_PREFIX Ogre::String("terrain")
#define TERRAIN_FILE_SUFFIX Ogre::String("dat")

class CWorldManager : public Ogre::Singleton<CWorldManager>
{
	public:
		/* dirLight = the directional light in the scene. Used for light maps. */
		CWorldManager(Ogre::SceneManager* aSceneManager, Ogre::Camera* aCamera, NxOgre::Scene* aPhysiXScene, Ogre::Light* aDirLight);
		~CWorldManager();
	
		/**	Get|Set Singleton **/
		static CWorldManager& getSingleton(void);
		static CWorldManager* getSingletonPtr(void);

		/* Adds a texture Layer to terrain.
		diffuseAndSpecMap = name of the file that has díffuse and specular maps.
		normalAndHeightMap = name of the file that has normal and displacement maps.
		worldSize = How many times they are tilted. If worldSize = 10 and and the terrains total size i 100, then it is tiled 10 times.*/
		void addTextureLayer(Ogre::String& diffuseAndSpecMap, Ogre::String& normalAndHeightMap, Ogre::Real worldSize);

		/* Generate the terrain in the Physics Scene. */
		void buildNxOgreTerrain();

		/* Get terrain at coordinates x,y */
		Ogre::Terrain* getTerrainAt(int x,int y);

		/* Initiate the terrain. Loads from a binary file or creates it from a heightmap. */
		void initTerrain();
		void destroyTerrain();

		NxOgre::SceneGeometry* getTerrainSceneGeometry(){return mSceneGeometry;}

	private:

		 /* Blend all the texture layers and creates the final texture for the terrain. */
		void initBlendMaps(Ogre::Terrain* terrain);
		void initBlendMapsByBlendMap(Ogre::Terrain* terrain);

		/* Creates a PhysX heightfield from the info in the terrain. */
		void loadTerrainGeometry(const Ogre::String& name, float* data, Ogre::uint16 size, Ogre::Real worldSize, Ogre::Real minHeight, Ogre::Real maxHeight, const Ogre::Vector3& position);

		bool							mTerrainsImported;
		Ogre::TerrainGlobalOptions*		mTerrainGlobals;
		Ogre::TerrainGroup*				mTerrainGroup;
		Ogre::SceneManager*				mSceneManager;
		Ogre::Camera*					mCamera;
		NxOgre::Scene*					mPhysicsScene;
		Ogre:: LogManager*				mLogManager;
		NxOgre::SceneGeometry*			mSceneGeometry;
};

#endif //C_WORLDMANAGER_H_