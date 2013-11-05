#include "../Headers/CWorldManager.h"

// BEGIN SINGLETON
template<> CWorldManager* Ogre::Singleton<CWorldManager>::msSingleton = 0;
CWorldManager* CWorldManager::getSingletonPtr(void)
{
    return msSingleton;
}
CWorldManager& CWorldManager::getSingleton(void)
{  
    assert( msSingleton );  return ( *msSingleton );  
}
// END SINGLETON

CWorldManager::CWorldManager(Ogre::SceneManager* aSceneManager, Ogre::Camera* aCamera, NxOgre::Scene* aPhysiXScene, Ogre::Light* aDirLight)
: mSceneManager(aSceneManager)
, mCamera(aCamera)
, mPhysicsScene(aPhysiXScene)
, mLogManager(Ogre::LogManager::getSingletonPtr())
{

	//Global terrain options here
	mTerrainGlobals = OGRE_NEW Ogre::TerrainGlobalOptions();
	mTerrainGlobals->setMaxPixelError(8);

	//Important information when calculating lightmaps
	mTerrainGlobals->setLightMapDirection(aDirLight->getDerivedDirection());
	mTerrainGlobals->setCompositeMapAmbient(mSceneManager->getAmbientLight());
	mTerrainGlobals->setCompositeMapDiffuse(aDirLight->getDiffuseColour());
	// Set the distance at which to start using a composite map if present
	mTerrainGlobals->setCompositeMapDistance(3000);

	//From wikipedia:
	//Anisotropic filtering (AF) is a method of enhancing the image quality of textures on surfaces 
	//that are far away and steeply angled with respect to the point of view. Older techniques,
	//such as bilinear and trilinear filtering don't take account of the angle a surface is viewed from,
	//which can result in aliasing or blurring of textures. By reducing detail in one direction more than another,
	//these effects can be reduced. 
	Ogre::MaterialManager::getSingleton().setDefaultTextureFiltering(Ogre::TFO_ANISOTROPIC);
	Ogre::MaterialManager::getSingleton().setDefaultAnisotropy(7);

	//Since we want the ability to load not only one terrain, we define a terrain group.
	mTerrainGroup = OGRE_NEW Ogre::TerrainGroup(mSceneManager, Ogre::Terrain::ALIGN_X_Z,513.0f, 12000.0f);
	mTerrainGroup->setFilenameConvention(TERRAIN_FILE_PREFIX, TERRAIN_FILE_SUFFIX);
	mTerrainGroup->setResourceGroup("Game");
}

/*---------------------------------------------------------------------------------*/
CWorldManager::~CWorldManager()
{
}

void CWorldManager::addTextureLayer(Ogre::String& diffuseAndSpecMap, Ogre::String& normalAndHeightMap, Ogre::Real worldSize)
{
	/* Adds a texture Layer to terrain.
	diffuseAndSpecMap = name of the file that has díffuse and specular maps.
	normalAndHeightMap = name of the file that has normal and displacement maps.
	worldSize = How many times they are tilted. If worldSize = 10 and and the terrains total size i 100, then it is tiled 10 times.*/
	Ogre::Terrain::ImportData& defaultimp = mTerrainGroup->getDefaultImportSettings();
	int size = defaultimp.layerList.size();
	defaultimp.layerList.resize(size + 1);
	defaultimp.layerList[size].worldSize = worldSize;
	defaultimp.layerList[size].textureNames.push_back(diffuseAndSpecMap);
	defaultimp.layerList[size].textureNames.push_back(normalAndHeightMap);
}

/*---------------------------------------------------------------------------------*/
void CWorldManager::buildNxOgreTerrain()
{
	mLogManager->logMessage("Building NxOgre Terrain files...");

	//Loop through all terrains.
	Ogre::TerrainGroup::TerrainIterator ti = mTerrainGroup->getTerrainIterator();
	while(ti.hasMoreElements())
	{
			//Get the terrain
			Ogre::Terrain* t = ti.getNext()->instance;
			//Convert to PhysX heightfields.
			loadTerrainGeometry(t->getMaterialName(), t->getHeightData(), t->getSize(), t->getWorldSize(), t->getMinHeight(), t->getMaxHeight(), t->getPosition());
	}

	mLogManager->logMessage("NxOgre Terrain files was created!");
}
/*---------------------------------------------------------------------------------*/
Ogre::Terrain* CWorldManager::getTerrainAt(int x,int y)
{
        return mTerrainGroup->getTerrain(x,y);
}

void CWorldManager::initTerrain()
{
	//Since paging is not used in this version, x and y will be 0.
	int x = 0; int y = 0;

	//Check if there is a terrain file available in the resource, otherwhise it will be created from an image.
	Ogre::String filename = mTerrainGroup->generateFilename(x, y);
	if (Ogre::ResourceGroupManager::getSingleton().resourceExists(mTerrainGroup->getResourceGroup(), filename))
	{
			mLogManager->logMessage("Terrain " + filename + " was found! No need to load terrain from image");
			mTerrainGroup->defineTerrain(0, 0);
	}
	else
	{
			mLogManager->logMessage("Terrain file was not found. Generating terrain from image...");

			// Configure default import settings
			Ogre::Terrain::ImportData& defaultimp = mTerrainGroup->getDefaultImportSettings();    
			defaultimp.terrainSize = 513.0f;
			defaultimp.worldSize = 12000.0f;
			defaultimp.inputScale = 1200;
			defaultimp.minBatchSize = 33;
			defaultimp.maxBatchSize = 65;

			//Loads heightmap from image.
			Ogre::Image img;
			img.load("terrain.png", "Game");
			mTerrainGroup->defineTerrain(x, y, &img);
			mTerrainsImported = true;
	}       

	// sync load since we want everything in place when we start
	mTerrainGroup->loadAllTerrains(true);

	//If new terrains were imported, we have to define how the texture layers are blending
	if (mTerrainsImported)
	{
			Ogre::TerrainGroup::TerrainIterator ti = mTerrainGroup->getTerrainIterator();
			while(ti.hasMoreElements())
			{
					Ogre::Terrain* t = ti.getNext()->instance;
					initBlendMaps(t);
			}
			//Saves the terrains for further use.
			mTerrainGroup->saveAllTerrains(true);
	}
	mTerrainGroup->freeTemporaryResources();
}

void CWorldManager::destroyTerrain()
{
	OGRE_DELETE mTerrainGroup;
	OGRE_DELETE mTerrainGlobals;
}

void CWorldManager::initBlendMaps(Ogre::Terrain* terrain)
{
    Ogre::TerrainLayerBlendMap* blendMap0 = terrain->getLayerBlendMap(1);
    Ogre::TerrainLayerBlendMap* blendMap1 = terrain->getLayerBlendMap(2);
    Ogre::Real minHeight0 = 70;
    Ogre::Real fadeDist0 = 40;
    Ogre::Real minHeight1 = 70;
    Ogre::Real fadeDist1 = 15;
    float* pBlend0 = blendMap0->getBlendPointer();
    float* pBlend1 = blendMap1->getBlendPointer();
    for (Ogre::uint16 y = 0; y < terrain->getLayerBlendMapSize(); ++y)
    {
        for (Ogre::uint16 x = 0; x < terrain->getLayerBlendMapSize(); ++x)
        {
            Ogre::Real tx, ty;
 
            blendMap0->convertImageToTerrainSpace(x, y, &tx, &ty);
            Ogre::Real height = terrain->getHeightAtTerrainPosition(tx, ty);
            Ogre::Real val = (height - minHeight0) / fadeDist0;
            val = Ogre::Math::Clamp(val, (Ogre::Real)0, (Ogre::Real)1);
            *pBlend0++ = val;
 
            val = (height - minHeight1) / fadeDist1;
            val = Ogre::Math::Clamp(val, (Ogre::Real)0, (Ogre::Real)1);
            *pBlend1++ = val;
        }
    }
    blendMap0->dirty();
    blendMap1->dirty();
    blendMap0->update();
    blendMap1->update();
}

void CWorldManager::initBlendMapsByBlendMap(Ogre::Terrain* terrain)
{
	//Start with checking how many texture layers that were added.
	//Since we can't blend the bottom texture, the needed amount of blend maps is nr of layers -1
	int numberOfBlendMaps = mTerrainGroup->getDefaultImportSettings().layerList.size();

	//Creating a vector of pointers to the blendLayerMaps
	std::vector<Ogre::TerrainLayerBlendMap*> blendMaps;

	//Blend pointers point to coordinates at the blendLayerMaps.
	std::vector<float*> blendPointers;

	//The images we're going to get the blend data from
	std::vector<Ogre::Image> blendMapImages;

	//Since paging is not used in this version, x and y at getTerrain will be 0.
	int blendMapSize = mTerrainGroup->getTerrain(0, 0)->getLayerBlendMapSize();

	//i starts at 1 since blendlayer map for the second texture has the index 1.
	for (int i= 1; i<numberOfBlendMaps; i++)
	{
			//Since paging is not used in this version, x and y at getTerrain will be 0.
			Ogre::TerrainLayerBlendMap* blendMap = mTerrainGroup->getTerrain(0, 0)->getLayerBlendMap(i);
			blendMaps.push_back(blendMap);
			blendPointers.push_back(blendMap->getBlendPointer());

			//Loading image named blendMap"i".png from the Resource Group "Game".
			Ogre::Image img;
			img.load("blendMap" +  Ogre::StringConverter::toString(i) + ".png","Game");
			//If the size isn't right, resize.
			if(img.getWidth() != blendMapSize)
			{
					img.resize(blendMapSize, blendMapSize);
			}
			blendMapImages.push_back(img);
	}

	//Work one pixel at a time. The Blend Pointers are pointing to a one dimensionall array
	//even though the picture is 2D. Looping this way (y first, x second) works.
	for (Ogre::uint16 y = 0; y < blendMapSize; ++y)
	{
			for (Ogre::uint16 x = 0; x < blendMapSize; ++x)
			{       
					for (unsigned int i= 0; i < blendMaps.size(); i++)
					{
							Ogre::Real value;
							//The color value in the blendMapImage.
							Ogre::ColourValue colourValue = blendMapImages[i].getColourAt(x,y,0);
							//The image is black and white, yet getting information from the red channel works.
							value = colourValue.r;
							//The pixel value that the blendPointer is pointing to is getting the colour value.
							// ++ to get to next pixel for next loop.
							*blendPointers[i]++ = value;
					}
			}
	}

	//The final blending.
	for (unsigned int i= 0;i < blendMaps.size(); i++)
	{
			blendMaps[i]->dirty();
			blendMaps[i]->update();
	}

}
/*---------------------------------------------------------------------------------*/
void CWorldManager::loadTerrainGeometry(const Ogre::String& name, float* data, Ogre::uint16 size, Ogre::Real worldSize, Ogre::Real minHeight, Ogre::Real maxHeight, const Ogre::Vector3& position)
{
	//Code snippet imported from http://www.ogre3d.org/wiki/index.php/BloodyMess_Ogre::Terrain_Heightfield (Modified)
	//Takes a Ogre::Terrain and creates a NxOgre HeightField from the Terrain data.

	// Create the manual heightfield
	NxOgre::ManualHeightField mhf;
	mhf.begin(size, size);
	Ogre::Real normMin = -32768.0f;
	Ogre::Real normMax = 32767.0f;
	// Sample the data to the manual heightfield
	for(int x = 0; x < size; ++x)
	{
			NxOgre::Enums::HeightFieldTesselation tess = NxOgre::Enums::HeightFieldTesselation_NW_SE;
			for(int z = size-1; z >= 0; --z)
			{
					Ogre::Real height = data[(size * z) + x];
					short sample = (short)(((height - minHeight) / (maxHeight - minHeight)) * (normMax - normMin) + normMin);
					mhf.sample(sample, 0, 0, tess);
					if(tess == NxOgre::Enums::HeightFieldTesselation_NE_SW)
							tess = NxOgre::Enums::HeightFieldTesselation_NW_SE;
					else
							tess = NxOgre::Enums::HeightFieldTesselation_NE_SW;
			}
	}
	// Create the actual heightfield
	NxOgre::HeightField *hf = mhf.end(true,"file://DesetTerrain.xm");
	hf->setName("Terrain");
	Ogre::Real hf_size = worldSize + (worldSize / size);
	Ogre::Real hf_height = (maxHeight - minHeight) / 2.0f;
	Ogre::Real hf_pose_x = position.x - (worldSize / 2.0f);
	Ogre::Real hf_pose_y = position.y + ((maxHeight + minHeight) / 2.0f);
	Ogre::Real hf_pose_z = position.z - (worldSize / 2.0f);
	
	#if NxOgreVersionMajor <= 1 && NxOgreVersionMinor <= 5
		NxOgre::HeightFieldGeometry* hfg = new NxOgre::HeightFieldGeometry(hf, NxOgre::Vec3(hf_size, hf_height, hf_size));
		hfg->setLocalPose(NxOgre::Matrix44(NxOgre::Vec3(hf_pose_x, hf_pose_y, hf_pose_z)));
		mvpPhysicsScene->createSceneGeometry(hfg);
	#else
		NxOgre::HeightFieldGeometryDescription desc(hf, NxOgre::Vec3(hf_size, hf_height, hf_size));
		Critter::BodyDescription bodyDescription;	
		bodyDescription.mName = "Terrain";
		mSceneGeometry = mPhysicsScene->createSceneGeometry(desc, NxOgre::Matrix44(NxOgre::Vec3(hf_pose_x, hf_pose_y, hf_pose_z)),bodyDescription);
		
	#endif
}

