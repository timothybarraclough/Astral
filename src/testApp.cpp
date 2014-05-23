#include "testApp.h"

static const int scaleFactor = 1;
static const int border = 5;
//--------------------------------------------------------------
void testApp::setup(){
    ofSetFrameRate(30);
    ofSetVerticalSync(true);
    
    
    myBDStart = ofVec2f(20, 30);
    myBDSize = ofVec2f(400, 400);
    fbo.allocate(myBDSize.x + (2 * border),myBDSize.y + (2 * border), GL_RGBA);
    myBD.setup(myBDSize, 16,fbo);
    currentConstellation.setup(myBDSize,16,fbo);
   // currentConstellation2.setup(myBDSize,8,fbo);
    ofBackground(30,30,50);
    
    quadric = gluNewQuadric();
    quadric2 = gluNewQuadric();
    gluQuadricTexture(quadric, GL_TRUE);
    gluQuadricNormals(quadric, GLU_SMOOTH);
    gluQuadricTexture(quadric2, GL_TRUE);
    gluQuadricNormals(quadric2, GLU_SMOOTH);
    
    ofDisableArbTex();

}

//--------------------------------------------------------------
void testApp::update(){
    
    currentConstellation.draw();
   // currentConstellation2.draw();

}

//--------------------------------------------------------------
void testApp::draw(){
    /*
    ofPushStyle();
    ofTranslate(ofGetWidth()/2,ofGetHeight()/2, ofGetWidth()/2);
    
    //rotate sphere over time
    ofRotateY(-90 + 18 * sin(ofGetFrameNum()/100.0));
    ofRotateX(90); //north pole facing up
    
    //bind and draw texture
    currentConstellation.image.getTextureReference().bind();
    gluSphere(quadric, 150, 1500, 1500);
    currentConstellation2.image.getTextureReference().unbind();
    ofPopStyle();
     */
    ofBackground(30,30,50);
    myBD.draw();
    currentConstellation.constellationWindow.draw(myBDStart.x, myBDStart.y);
    //currentConstellation.sphereWindow.draw(ofGetWidth()/2 + myBDStart.x, myBDStart.y);

}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    if (key == 32){
        ofxOscMessage m;
        m.setAddress( "/endConst" );
        m.addIntArg(0);
        currentConstellation.oscSender.sendMessage( m );
    }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
    


}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    switch (button) {
        case 0:
            if (x > myBDStart.x && x < myBDSize.x + myBDStart.x + (border * 2)){
                
                if (y > myBDStart.y&& y < myBDSize.y + (border * 2) + myBDStart.y){
                    ofVec2f newPoint = ofVec2f(x - myBDStart.x * scaleFactor,y - myBDStart.y * scaleFactor);
                   // ofPoint newcoord = currentConstellation.addPoint();
                    float xsnapSize = currentConstellation.windowSize.x/currentConstellation.granularity;
                    float ysnapSize = currentConstellation.windowSize.y/currentConstellation.granularity;
                    int xSnap = round(newPoint.x - border,xsnapSize);
                    int ySnap = round(newPoint.y - border,ysnapSize);
                    
                    ofPoint starSnapped = ofPoint(xSnap, ySnap);
                    ofPoint newcoord = ofPoint(xSnap/xsnapSize,ySnap/ysnapSize);
                    ofxOscMessage m;
                    
                    m.setAddress( "/addStar" );
                    m.addIntArg( newcoord.x);
                    m.addIntArg( newcoord.y);
                    currentConstellation.oscSender.sendMessage( m );
                    //currentConstellation2.addPoint(newPoint);
                }
            }
                break;
        case 2:
            currentConstellation.endConstellation();
            
        default:
            break;
    }

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}

float testApp::round(float number, float round) {
    if(fmodf(number, round) <= (round / 2))
        return number - fmodf(number, round);   //Round down
    else return number - fmodf(number, round) + round;   //Round down, then up again
}
