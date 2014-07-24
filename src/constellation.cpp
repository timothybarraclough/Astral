//
//  constellation.cpp
//  First_Astral_Sketch_Open_Frameworks
//
//  Created by Timothy J on 19/05/14.
//
//

#include "constellation.h"
const int border = 5;

void constellation::setup(ofPoint _size, int _granularity, ofFbo _fbo, int _myID, vector<ofColor> _colorScheme){

    granularity = _granularity;
    myID = _myID;
    constellationWindow = _fbo;
    windowSize = _size;
    isFinished = false;
    interpIndex = 0;
    sphereWindow.allocate(windowSize.x + (2 * border),windowSize.y + (2 * border), GL_RGBA);
    myStars.resize(0);
    colorScheme.push_back(ofColor(203,68,68));
    colorScheme.push_back(ofColor(43,129,193));
    colorScheme.push_back(ofColor(88,188,161));
    colorScheme.push_back(ofColor(211,151,11));
}



void constellation::collide(int _theStar){
    
    myStarSize.at(_theStar) = 0;
    pbp.x = myStars.at(_theStar).x;
    pbp.y = myStars.at(_theStar).y;
}

void constellation::setTarget(int _target, int _targetTime){
    pbp = myStars.at(_target);
    dbp = myStars.at(_target + 1);
    myStarSize.at(_target) = 0;
    direction = myStars.at(_target + 1) - myStars.at(_target);
    direction /= 6;
    direction /= _targetTime;
}



void constellation::setPos(float _x, float _y){
    
    pbp.x = (_x * (windowSize.x/granularity)/2.0) +  (pbp.x /2.0);
    pbp.y = (_y * (windowSize.y/granularity)/2.0) + (pbp.y /2.0);
}

void constellation::update(int myIndex){
    
    if (isFinished)
    {
        //direction *= 2;
        //pbp += direction;
    }
    
    for (int i = 0; i < myStars.size(); i++){
        if  (myStarSize[i] < 18) myStarSize[i] += 1.0;
        //else myStarSize[i] = 0;
    }
    
    
    
    
}

void constellation::interpolateBetweenPoints(float x, float y){
    dbp.x = x;
    dbp.y = y;
}

void constellation::draw(){
    
    
    ofPushStyle();
    constellationWindow.begin();
    ofTranslate(border, border);
    ofSetLineWidth(1);
    
    
    for (int i = 0; i < myStars.size(); i++){
        
        drawStar(i);
        if (i > 0) {
            
            //ofSetColor(255, 180, 160,90);
            ofSetColor(ofColor(colorScheme[myID],90));

            ofLine(myStars.at(i-1),myStars.at(i));
        }
    }
    
    //ofSetColor(255,255,255,255);
    //ofSetColor(255, 180, 160,190);
    ofSetColor(ofColor(colorScheme[myID],190));

    if(isFinished){
       if(myStars.size() > 2)ofCircle(pbp.x, pbp.y, 4);
    }
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
        int z = ofRandom(-200,200);
        
        ofPoint starSnapped = ofPoint(xSnap, ySnap);
        ofPoint coord = ofPoint(xSnap/xsnapSize,ySnap/ysnapSize);
        myCoords.insert(myCoords.end(),coord);
        myStars.insert(myStars.end(), starSnapped);
        myStarSize.insert(myStarSize.end(), 0);
        // cout << "Star added at x point  " << xIndex << "   snapsize of  " << xsnapSize << "\n";
        // cout << "New amount in Constellation  =  " << myStars.size() << "\n";
    }
}

void constellation::addStarByCoord(ofPoint theStar){
    myCoords.insert(myCoords.end(), theStar);
    theStar.x *= windowSize.x/granularity;
    theStar.y *= windowSize.y/granularity;
    myStars.insert(myStars.end(), theStar);
    myStarSize.insert(myStarSize.end(), 0);
    ofxOscMessage m;
    
  //  oscSender.sendMessage( m );
    
}

void constellation::drawStar(int _index){
    ofSetColor(colorScheme[myID]);
    
    ofCircle(myStars[_index].x, myStars[_index].y, 2);
    ofSetColor(ofColor(colorScheme[myID],185 - (10 * myStarSize[_index])));
  //  ofSetColor(255, 255, 255,200 - (10 * myStarSize[_index]));
    
   if (myStarSize[_index] > 0) ofCircle(myStars[_index].x, myStars[_index].y, myStarSize[_index]);
    
}

void constellation::endConstellation(){
    //Add the first point to the end in order to make it loop fully.
    myStars.insert(myStars.end(),myStars.at(0));
    myCoords.insert(myCoords.end(),myCoords.at(0));
    myStarSize.insert(myStarSize.end(), 10);
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

void constellation::zero(){
    myStars.erase(myStars.begin(), myStars.end());
    myCoords.erase(myCoords.begin(), myCoords.end());
    myStarSize.erase(myStarSize.begin(), myStarSize.end());
    isFinished = false;
}