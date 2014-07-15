//
//  menuBar.cpp
//  First_Astral_Sketch_Open_Frameworks
//
//  Created by Timothy J on 12/06/14.
//
//

#include "menuBar.h"

static int border = 5;

void menuBar::setup(int sizex, int sizey, int _buttonAmount, vector<ofColor> _colorScheme){
    
    colorScheme = _colorScheme;
    buttonAmount = _buttonAmount;
    window.allocate(sizex * buttonAmount + (border * 2), sizey + (border * 2),GL_RGBA,8);
    buttonSizex = sizex;
    buttonSizey = sizey;
    for (int i = 0; i < _colorScheme.size(); i++){
        flashAmount.push_back(0);
    }

    
}

void menuBar::update(){
    
    for (int i = 0; i < flashAmount.size(); i++){
        if (flashAmount[i] > 0.0) flashAmount[i] -= 2.0;

    }

    window.begin();
    ofPushStyle();
    ofClear(0,0,0,0);
    ofFill();
    ofTranslate(border, border);
    for (int i = 0; i < buttonAmount; i++){
        ofSetColor(ofColor(colorScheme[i],130+flashAmount[i]));
        ofRectRounded(i*buttonSizex,0,buttonSizex,buttonSizey,10);
    }
    ofSetColor(colorScheme[selected]);
    ofSetLineWidth(3);
    ofNoFill();
    ofRectRounded(selected * buttonSizex, 0, buttonSizex, buttonSizey,10);
    ofPopStyle();
    window.end();
}

void menuBar::draw(){
    
    
}

void menuBar::highlight(int _index){
    selected = _index;

}

void menuBar::flash(int _index){
    flashAmount[_index] = 40.0;
}