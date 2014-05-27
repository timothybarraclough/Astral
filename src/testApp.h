#pragma once

#include "ofMain.h"
#include "backDrop.h"
#include "constellation.h"
#include "ofxOSC.h"

#define HOST "localhost"
#define PORT 8080
#define InPORT 8081

class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
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
    
    GLUquadricObj *quadric;
    GLUquadricObj *quadric2;
    
    ofxOscSender oscSender;
    ofxOscMessage oscMessage;
    ofPoint pbp;
    ofxOscReceiver oscReceiver;


		
};
