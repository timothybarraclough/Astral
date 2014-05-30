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

void Cursor::update(){
    
    
    
}

void Cursor::draw(){
    fbo.begin();
    ofPushStyle();
    ofSetColor(col);
    ofCircle(position.x, position.y, 4);
    ofPopStyle();
    fbo.end();
    
}