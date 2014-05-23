//
//  constellation.cpp
//  First_Astral_Sketch_Open_Frameworks
//
//  Created by Timothy J on 19/05/14.
//
//

#include "constellation.h"
const int border = 5;

void constellation::setup(ofPoint _size, int _granularity, ofFbo _fbo){
    oscSender.setup( HOST, PORT );
    oscReceiver.setup( InPORT );
    granularity = _granularity;
    constellationWindow = _fbo;
    windowSize = _size;
  //  windowSize.x += border * 2;
   // windowSize.y += border * 2;
   // constellationWindow.allocate(windowSize.x + (2 * border),windowSize.y + (2 * border), GL_RGBA);
    sphereWindow.allocate(windowSize.x + (2 * border),windowSize.y + (2 * border), GL_RGBA);
    
    myStars.resize(0);
    
    
}

void constellation::draw(){
    while( oscReceiver.hasWaitingMessages() )
    {
        // get the next message
        ofxOscMessage m;
        oscReceiver.getNextMessage( &m );
        cout << m.getAddress();
        // check for mouse moved message
        if (m.getAddress() == "/pPosition")
        {
            // both the arguments are int32's
            float guy = windowSize.x / granularity;
           pbp.x = m.getArgAsFloat(0) * guy;
            
           pbp.y = m.getArgAsFloat(1) * guy;
          

        }
        
        else if(m.getAddress() == "/addStar"){
            cout << "got new star";
            int newX = m.getArgAsInt32(0) * (windowSize.x/granularity);
            int newY = m.getArgAsInt32(1) * (windowSize.y/granularity);
            ofPoint newPoint = ofPoint(newX,newY);
            addPoint(newPoint);
        }
    }
    ofPushStyle();
    constellationWindow.begin();
    ofClear(255,255,255,0);
    
    ofSetColor(40,40,200,200);
   // ofRectRounded(0, 0, constellationWindow.getWidth() , constellationWindow.getHeight(),5);
   
    
    ofTranslate(border, border);
    ofSetColor(20,220,180,180);
    ofSetLineWidth(3);
   
    
    for (int i = 0; i < myStars.size(); i++){
        ofSetColor(255, 180, 160,190);
        drawStar(myStars.at(i));
        if (i > 0) {
           /* for (int j = 0; j < myStars.size(); j++){
                ofLine(myStars.at(i),myStars.at(j));
            }*/
            ofSetColor(255, 180, 160,90);
           ofLine(myStars.at(i-1),myStars.at(i));
        }
    }
    ofCircle(pbp.x, pbp.y, 5);
    ofSetColor(255, 180, 160,30);
   // myBD.draw();

    constellationWindow.end();
    
    //convert fbo to ofImage format
    constellationWindow.readToPixels(fboPixels);
    image.setFromPixels(fboPixels);
    ofPopStyle();
    /*
    ofPushStyle();
    sphereWindow.begin();
    ofClear(255,255,255,0);
    ofSetColor(240,240,240,210);
    ofNoFill();
    ofTranslate(border/2,border/2);
    ofRectRounded(border, border, windowSize.x , windowSize.y,5);
    ofFill();
    ofPoint Center = ofPoint(0,0,0);
    ofNoFill();
    
    
    ofTranslate(sphereWindow.getWidth()/2, sphereWindow.getHeight()/2,205);//sphereWindow.getWidth()/2);
    ofSetColor(255, 255, 255);
    ofRotateX(-90);
    
    
    ofCircle(Center, 80);
    ofRotateX(-90);
    ofSetColor(0, 255, 0);
    ofCircle(Center, 80);
    ofRotateY(-90);
    ofSetColor(0, 255, 255);
    ofCircle(Center, 80);
    ofSetColor(255, 255, 0);
    ofRotateX(90);
    ofRotateZ(-90);
    
    
    ofCircle(Center, 80);
    ofPoint North = ofPoint(100,0,0);
    ofPoint West = ofPoint(0,100,0);
    ofLine(Center, North);
    ofFill();
    
    
    ofRotateY((ofGetFrameNum()));
    //ofRotateX((ofGetFrameNum())/0.7);
    ofPoint lastPoint;
    ofSetColor(80, 80, 80);
    ofSphere(Center, 10);
    for (int i = 0; i < myCoords.size(); i++){
        ofPath p;
        p.setStrokeColor(255);
        p.setFilled(false);
        p.setStrokeWidth(1);
        
        ofVec3f Star = ofVec3f(0,0,60);
        
        ofPushMatrix();
        float xRot = ofRadToDeg(myCoords.at(i).x * (PI/(granularity)));
       // ofRotateX(xRot);
        float yRot = ofRadToDeg(myCoords.at(i).y * (PI/(granularity)));
       // ofRotateY(yRot);
        Star.rotate(xRot, yRot,0);
       // ofPoint Star = ofPoint(0,0,100);
       
        ofSphere(Star, 1);
       // ofCircle(Star, 1);
        
        ofPopMatrix();
       // cout << Star.x;
        //ofPushMatrix();
        
        if (i == 0){
            
            p.moveTo(Star);
        }
        else if (i > 0){
           // ofTranslate(Star);
            p.moveTo(Star);
            p.curveTo(North);
            p.curveTo(Star);
            p.curveTo(lastPoint);
            p.curveTo(Center);
            
            //ofLine(lastPoint, Star);
            
        }
        lastPoint = Star;
        //ofPopMatrix();
        
        p.draw();
        
    }
    ofSetColor(255, 180, 160,30);
    
    sphereWindow.end();
    
    //convert fbo to ofImage format
    constellationWindow.readToPixels(fboPixels);
    image.setFromPixels(fboPixels);
    
    ofPopStyle();
     */
}

void constellation::addPoint(ofPoint theStar){
    
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

void constellation::drawStar(ofPoint theStar){
        ofCircle(theStar.x, theStar.y, 2);
}

void constellation::endConstellation(){
    //Add the first point to the end in order to make it loop fully.
    myStars.insert(myStars.end(),myStars.at(0));
    ofxOscMessage m;
    m.setAddress( "/endConst" );
    m.addIntArg( myStars.at(0).x );
    m.addIntArg( myStars.at(0).y );
    oscSender.sendMessage(m);
    
}

//Nice rounding function
float constellation::round(float number, float round) {
    if(fmodf(number, round) <= (round / 2))
        return number - fmodf(number, round);   //Round down
    else return number - fmodf(number, round) + round;   //Round down, then up again
}