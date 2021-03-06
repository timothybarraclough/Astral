#pragma once

#include "ofMain.h"
#include "backDrop.h"
#include "constellation.h"
#include "cursor.h"
#include "ofxOSC.h"
#include "ofxBonjour.h"
#include <Cocoa/Cocoa.h>
#include "iOSDevice.h"
#include "menuBar.h"
#include "ofxFX.h"
#include "ofxBokeh.h"
#include "ofxRipples.h"
#include "ofxBounce.h"



#define HOST "localhost"
#define PORT 8080
#define InPORT 8081


class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
        void exit();
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
        float round(float number, float round);
    
    
    constellation currentConstellation;
    int constellationCounter;
    constellation constellationBank[4];
    constellation currentConstellation2;
    ofVec2f myBDSize;
    ofVec2f myBDStart;
    ofFbo fbo;
    backDrop myBD;
    ofImage bg;
    ofImage image;
    ofPixels fboPixels;
    
    ofxOscSender oscSender;
    ofxOscMessage oscMessage;
    ofPoint pbp;
    ofxOscReceiver oscReceiver;
    
    Server bonjourServer;
    Cursor myCursor;
    
    vector<iOSDevice> iOSDevices;
    
    menuBar menu;
    vector<ofColor> colorScheme;
    
    void exportImage();
    void changeConstellation(int index);
    void endConstellation(int index);
    void eraseConstellation(int index);


		
};
