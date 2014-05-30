//
//  cursor.h
//  First_Astral_Sketch_Open_Frameworks
//
//  Created by Timothy J on 29/05/14.
//
//

#include <iostream>
#include "ofMain.h"
#include "ofxOsc.h"


class Cursor : public ofBaseApp{
    
    public :
    
    ofPoint position;
    ofFbo fbo;
    ofColor col;
    void update();
    void setup(ofFbo _fbo, ofColor _col);
    void draw();
    
    
    
    
};
