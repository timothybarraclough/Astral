#include "ofMain.h"
#include "testApp.h"
#include <Cocoa/Cocoa.h>
#include <unistd.h>
#define GetCurrentDir getcwd

//========================================================================
int main( ){
	ofSetupOpenGL(800,800,OF_WINDOW);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	
    // pass in width and height too:
    
    ofSetDataPathRoot("../Resources/data/");
    ofFilePath x;
    
    
    string j = x.getAbsolutePath("Astral_brain2.ck");
    string i = x.getAbsolutePath("chuck");
    //ofSystem("chuck --kill");
    
   // ofSystem(i + " --kill");
    
    //This is the one I am using presently.
    ofSystem(i + " " + j + " &");
    
	ofRunApp(new testApp());

}
