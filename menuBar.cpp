//
//  menuBar.cpp
//  First_Astral_Sketch_Open_Frameworks
//
//  Created by Timothy J on 12/06/14.
//
//

#include "menuBar.h"

static int border = 5;
//Setup my menubar
void menuBar::setup(int sizex, int sizey, int _buttonAmount, vector<ofColor> _colorScheme){
    //Get colorscheme
    colorScheme = _colorScheme;
    //Get amount of buttons / constellations
    buttonAmount = _buttonAmount;
    //allocate fbo for the menu
    window.allocate(sizex * buttonAmount + (border * 2), sizey + (border * 2),GL_RGBA,8);
    //init button sizes
    buttonSizex = sizex;
    buttonSizey = sizey;
    //set flashamounts for each button to zero and allocate
    for (int i = 0; i < _colorScheme.size(); i++){
        flashAmount.push_back(0);
    }

    
}

void menuBar::update(){
    
    //Flash the buttons if necessary (occurs on testapp::collide)
    for (int i = 0; i < flashAmount.size(); i++){
        if (flashAmount[i] > 0.0) flashAmount[i] -= 1.5;

    }

    //Begin window/ clear / push style etc.
    window.begin();
    ofPushStyle();
    ofClear(0,0,0,0);
    ofFill();
    //Move to border amount
    ofTranslate(border, border);
    //Draw rectangles for each of the buttons (with the right colour)
    for (int i = 0; i < buttonAmount; i++){
        ofSetColor(ofColor(colorScheme[i],130+flashAmount[i]));
        ofRectRounded(i*buttonSizex,0,buttonSizex,buttonSizey,10);
    }
    ofSetColor(colorScheme[selected]);
    ofSetLineWidth(3);
    ofNoFill();
    //Draw a rectangle to highlight the selected button.
    ofRectRounded(selected * buttonSizex, 0, buttonSizex, buttonSizey,10);
    ofPopStyle();
    window.end();
}

void menuBar::draw(){
 
}
//Highlight the correct button (is this being called?)
void menuBar::highlight(int _index){
    selected = _index;
}
//Flash the button when collide is called
void menuBar::flash(int _index){
    flashAmount[_index] = 20.0;
}