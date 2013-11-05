#include "Headers\CGame.h"
#include "windows.h"
#include "OgreException.h"

#define WIN32_LEAN_AND_MEAN

void main(int argc, char **argv)
{
	try
	{
		CGame* theBestGame = new CGame();
		theBestGame->Initialize();
		theBestGame->Start();
		delete theBestGame;
	} 
	catch(Ogre::Exception& e ) 
	{
		MessageBoxA(NULL, e.what(),"CronoCasException : ", MB_OK | MB_ICONERROR | MB_TASKMODAL);
	} catch(...)
	{
		MessageBoxA(NULL, "Unhandled Exception","CronoCasException : ", MB_OK | MB_ICONERROR | MB_TASKMODAL);
	}

	exit(0);
}

/*
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32 || OGRE_PLATFORM == PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h" 
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32 || OGRE_PLATFORM == PLATFORM_WIN32
    VOID WINAPI mainCC( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
    void mainCC(int argc, char *argv[])
#endif
    {
		ccGame cronoCas;
        try {
			cronoCas.start();
            //ccGame* cronoCas = new ccGame();
			//cronoCas->start();
			//delete cronoCas;
        } catch( Ogre::Exception& e ) {
           MessageBoxA( NULL, e.getFullDescription().c_str(), "CronoCasException!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
        }catch(...){
			MessageBoxA(NULL, "Unhandled Exception","CronoCasException : ", MB_OK | MB_ICONERROR | MB_TASKMODAL);
		}
        exit(0);
    }*/