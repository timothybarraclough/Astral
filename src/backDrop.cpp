//
//  backDrop.cpp
//  First_Astral_Sketch_Open_Frameworks
//
//  Created by Timothy J on 19/05/14.
//
//

#include "backDrop.h"

const int border = 5;

void backDrop::setup(ofPoint _size, int _granularity, ofFbo _fbo){
    granularity = _granularity;
    windowSize = _size;
    backDropWindow = _fbo;
    
    int amount = windowSize.x / granularity;
    for (int i = 0; i < granularity; i+=amount){
        for(int j = 0; j < granularity; j+= amount){
            
            points.push_back(ofVec3f(i,j,0));
        }
    }
}

void backDrop::draw(){
    ofPushStyle();

    backDropWindow.begin();
    ofTranslate(border, border);
    ofNoFill();
    ofSetLineWidth(2);
    ofSetColor(200,200,200,80);
    ofRect(0, 0, windowSize.x, windowSize.y);
    ofFill();
    ofSetLineWidth(1);
    
    //Draw the x lines
    for (int g = 1; g < granularity; g++){
        int xPlacement =  (g * (windowSize.x/granularity));
        ofLine(xPlacement,0,xPlacement,windowSize.y);
    }
    
    //Draw the y lines
    int yGranularity = (granularity / 3) * 4;
    for (int g = 1; g < granularity; g++){
        int yPlacement = (g * (windowSize.x/granularity));
        ofLine(0,yPlacement,windowSize.x,yPlacement);
    }
    backDropWindow.end();

    ofPopStyle();
    
}
