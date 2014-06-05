//
//  cursor.cpp
//  First_Astral_Sketch_Open_Frameworks
//
//  Created by Timothy J on 29/05/14.
//
//

#include "cursor.h"

void Cursor::setup(ofFbo _fbo, ofColor _col){
    
    fbo = _fbo;
    col = _col;
    
    
    
}

void Cursor::update(float x, float y){
    //currentX = 1;
    
    static float x0 = 0;
    static float y0 = 0;
    
    const float dt = (1.0 / 5);
    const double RC = 0.3;
    const double alpha = dt / (RC + dt);
    
    //ofVec2f smoothed;
    position.x = (alpha * x) + (1.0 - alpha) * x0;
    position.y = (alpha * y) + (1.0 - alpha) * y0;
    
    
    //If some relationship between the acceleration and the previous values,
    //output some OSC. this should be if there is significant change ?
    //self.hooploop.updated = true;
    x0 = position.x;
    y0 = position.y;
    
    
    
}

void Cursor::draw(){
    fbo.begin();
    ofPushStyle();
    ofSetColor(col);
    ofCircle(position.x, position.y, 4);
    ofPopStyle();
    fbo.end();
    
}