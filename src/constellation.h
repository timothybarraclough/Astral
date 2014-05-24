//
//  constellation.h
//  First_Astral_Sketch_Open_Frameworks
//
//  Created by Timothy J on 19/05/14.
//
//
#include <iostream>
#include "ofMain.h"
#include <vector>
#include "backDrop.h"
#include "ofxOsc.h"



class constellation : public ofBaseApp{
    

    
    
    public :
    void drawStar(ofPoint theStar);
    void setup(ofPoint size,int _granularity, ofFbo _fbo);
    void draw();
    void update(int myIndex);
    void addPoint(ofPoint thePoint);
    void endConstellation();
    void interpolateBetweenPoints(float x, float y);
    
    float round(float number, float round);

    ofFbo constellationWindow;
    ofPoint windowSize;
    
    int granularity;
    vector<ofPoint> myStars;
    
    ofFbo sphereWindow;
    vector<ofPoint> myCoords;
    
    
    ofImage image;
    ofPixels fboPixels;
    //ofxOscSender oscSender;
    //ofxOscMessage oscMessage;
    ofPoint pbp;
    ofPoint dbp;
    int intTime;
    int interpIndex;
    
    bool isFinished;
    
   // ofxOscReceiver oscReceiver;
    
    
};




