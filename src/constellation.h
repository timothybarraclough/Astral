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
    vector<ofColor> colorScheme;
    void drawStar(int _index);
    void setup(ofPoint size,int _granularity, ofFbo _fbo, int _myID,vector<ofColor> colorScheme);
    void draw();
    void update(int myIndex);
    void addPoint(ofPoint thePoint);
    void endConstellation();
    void interpolateBetweenPoints(float x, float y);
    void zero();
    void setTarget(int _target, int _targetTime);
    void setPos(float _x, float _y);
    void collide(int _starIndex);
    
    float round(float number, float round);

    ofFbo constellationWindow;
    ofPoint windowSize;
    
    int granularity;
    vector<ofVec3f> myStars;
    int myID;
    
    ofFbo sphereWindow;
    vector<ofPoint> myCoords;
    vector<float> myStarSize;
    
    
    ofImage image;
    ofPixels fboPixels;
    //ofxOscSender oscSender;
    //ofxOscMessage oscMessage;
    ofPoint pbp;
    ofPoint dbp;
    ofVec2f direction;
    int target;
    int interpIndex;
    bool isFinished;
    
    void addStarByCoord(ofPoint theStar);
    
   // ofxOscReceiver oscReceiver;
    
    
};




