//
//  backDrop.h
//  First_Astral_Sketch_Open_Frameworks
//
//  Created by Timothy J on 19/05/14.
//
//

#pragma once

#include <iostream>
#include "ofMain.h"
#include <vector>

class backDrop : public ofBaseApp {
    
public:
    void setup(ofPoint size,int _granularity, ofFbo fbo);
    void draw();
    ofFbo backDropWindow;
    ofPoint windowSize;
    int granularity;
    

};

