#include "ofMain.h"
#include "testApp.h"
#include <Cocoa/Cocoa.h>

//========================================================================
int main( ){
	ofSetupOpenGL(650,650,OF_WINDOW);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	
    // pass in width and height too:
   // system("chuck --kill");
    system("chuck /Developer/Astral/First_Astral_Sketch_Open_Frameworks/ChucK/Astral_brain.ck &");
	ofRunApp(new testApp());

}
