//
//  menuBar.h
//  First_Astral_Sketch_Open_Frameworks
//
//  Created by Timothy J on 12/06/14.
//
//

#include <iostream>
#include "ofMain.h"

class menuBar: public ofBaseApp{
    
    private :
    
    
    int buttonAmount;
    int buttonSizex;
    int buttonSizey;
    
    public :
    
    void setup(int sizex, int sizey, int _buttonAmount, vector<ofColor> _colorScheme);
    vector<ofColor> colorScheme;
    void update();
    void draw();
    
    void highlight(int _index);
    void flash(int _index);
    vector<float> flashAmount;
    ofFbo window;
    int selected;
    

};
