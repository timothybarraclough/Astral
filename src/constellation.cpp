//
//  constellation.cpp
//  First_Astral_Sketch_Open_Frameworks
//
//  Created by Timothy J on 19/05/14.
//
//

#include "constellation.h"
const int border = 5;

void constellation::setup(ofPoint _size, int _granularity, ofFbo _fbo, int _myID){

    granularity = _granularity;
    myID = _myID;
    constellationWindow = _fbo;
    windowSize = _size;
    isFinished = false;
    interpIndex = 0;
    //windowSize.x += border * 2;
    //windowSize.y += border * 2;
    //constellationWindow.allocate(windowSize.x + (2 * border),windowSize.y + (2 * border), GL_RGBA);
    sphereWindow.allocate(windowSize.x + (2 * border),windowSize.y + (2 * border), GL_RGBA);
    myStars.resize(0);

}

void constellation::update(int myIndex){

    if (isFinished)
    {
       // cout << "Constellation index :  " << myID << " has an interpolation index  of :  "<< interpIndex <<   "\n";
    }
    
    
    
    
}

void constellation::interpolateBetweenPoints(float x, float y){
    dbp.x = x;
    dbp.y = y;
}

void constellation::draw(){
    
    ofPushStyle();
    constellationWindow.begin();
    //ofClear(255,255,255,0);
    
    ofSetColor(40,40,200,200);
   // ofRectRounded(0, 0, constellationWindow.getWidth() , constellationWindow.getHeight(),5);
   
    
    ofTranslate(border, border);
    ofSetColor(20,220,180);
    ofSetLineWidth(1);
   
    
    for (int i = 0; i < myStars.size(); i++){
        ofSetColor(255, 180, 160,190);
        drawStar(myStars.at(i));
        if (i > 0) {

            ofSetColor(255, 180, 160,90);
           ofLine(myStars.at(i-1),myStars.at(i));
        }
    }
    
    ofSetColor(255,255,255,255);
    if(isFinished)ofCircle(dbp.x, dbp.y, 5);
    ofSetColor(255, 180, 160,30);
   // myBD.draw();

    constellationWindow.end();
    
    //convert fbo to ofImage format
    constellationWindow.readToPixels(fboPixels);
    image.setFromPixels(fboPixels);
    ofPopStyle();
}

void constellation::addPoint(ofPoint theStar){
    
    if (!isFinished){
    float xsnapSize = windowSize.x/granularity;
    float ysnapSize = windowSize.y/granularity;
    int xSnap = round(theStar.x - border,xsnapSize);
    int ySnap = round(theStar.y - border,ysnapSize);
    
    ofPoint starSnapped = ofPoint(xSnap, ySnap);
    ofPoint coord = ofPoint(xSnap/xsnapSize,ySnap/ysnapSize);
    myCoords.insert(myCoords.end(),coord);
    myStars.insert(myStars.end(), starSnapped);
   // cout << "Star added at x point  " << xIndex << "   snapsize of  " << xsnapSize << "\n";
   // cout << "New amount in Constellation  =  " << myStars.size() << "\n";
    }
}

void constellation::drawStar(ofPoint theStar){
        ofCircle(theStar.x, theStar.y, 2);
}

void constellation::endConstellation(){
    //Add the first point to the end in order to make it loop fully.
    myStars.insert(myStars.end(),myStars.at(0));
    myCoords.insert(myCoords.end(),myCoords.at(0));
    pbp.set(myStars.at(0).x,myStars.at(0).y);
    isFinished = true;
    cout << "isFinished set to true";
    
}

//Nice rounding function
float constellation::round(float number, float round) {
    if(fmodf(number, round) <= (round / 2))
        return number - fmodf(number, round);   //Round down
    else return number - fmodf(number, round) + round;   //Round down, then up again
}