#include "testApp.h"

static const int scaleFactor = 1;
static const int border = 5;
static const int granularity = 16;
//--------------------------------------------------------------
void testApp::setup(){
    ofSetFrameRate(30);
    ofSetVerticalSync(true);

    
    oscSender.setup( HOST, PORT );
    oscReceiver.setup( InPORT );

    myBDStart = ofVec2f(20, 30);
    myBDSize = ofVec2f(400, 400);
    fbo.allocate(myBDSize.x + (2 * border),myBDSize.y + (2 * border), GL_RGBA);
    myBD.setup(myBDSize, 16,fbo);
    constellationCounter = 0;
    currentConstellation.setup(myBDSize,16,fbo);
    //constellationBank.insert(constellationBank.end(), currentConstellation);
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
    while( oscReceiver.hasWaitingMessages() )
    {
        // get the next message
        ofxOscMessage m;
        oscReceiver.getNextMessage( &m );
        //cout << m.getAddress();
        // check for mouse moved message
        /*
        if (m.getAddress() == "/pPosition")
        {
            // both the arguments are int32's
            float guy = myBDSize.x / granularity;
            float x = m.getArgAsFloat(0) * guy;
            
            float y = m.getArgAsFloat(1) * guy;
            constellationBank.at(m.getArgAsInt32(2)).pbp.set(x, y);
            
        }
        
        
        if (m.getAddress() == "/pPosition")
        {
            // both the arguments are int32's
           
            constellation co = constellationBank.at(m.getArgAsInt32(4));
            float guy = myBDSize.x / granularity;
            co.pbp.x = m.getArgAsFloat(0) * guy;
            co.pbp.y = m.getArgAsFloat(1) * guy;
            co.dbp.x = m.getArgAsFloat(2) * guy;
            co.dbp.y = m.getArgAsFloat(3) * guy;
            co.intTime = m.getArgAsInt32(5);
           // cout << "New target  =  " << co.dbp.x << "  /  " << co.dbp.y << "\n";
           // cout << "New xurret  =  " << co.pbp.x << "  /  " << co.pbp.y << "\n";
           // .pbp.set(x, y);
            
        }
         */
        
        if (m.getAddress() == "/pPosition")
        {
            constellation co = constellationBank.at(m.getArgAsInt32(2));
            //co.interpIndex = m.getArgAsInt32(0);
            co.interpIndex = ( co.interpIndex + 1 ) % co.myStars.size();
            //cout << co.myCoords[m.getArgAsInt32(0)].x << "  x  coord" << co.myCoords[0].y << "  y coord  \n";
            constellationBank.at(m.getArgAsInt32(2)).interpolateBetweenPoints(constellationBank.at(m.getArgAsInt32(2)).myStars.at(m.getArgAsInt32(0)).x, (constellationBank.at(m.getArgAsInt32(2)).myStars.at(m.getArgAsInt32(0)).y));
            // cout << co.pbp.x << "\n";
            // co.intTime = m.getArgAsInt32(1);
            //cout << co.myStars[m.getArgAsInt32(0)].x << "\n";
            //cout <<  "constellation index  " << m.getArgAsInt32(2) << "  star index  " << m.getArgAsInt32(0) << "\n";
            
        }
        else if(m.getAddress() == "/addStar"){
            cout << "got new star";
            int newX = m.getArgAsInt32(0) * (myBDSize.x/granularity);
            int newY = m.getArgAsInt32(1) * (myBDSize.y/granularity);
            ofPoint newPoint = ofPoint(newX,newY);
            currentConstellation.addPoint(newPoint);
        }
        
        else if(m.getAddress() == "/addConstellation"){
            
            constellation newConst;
            
            newConst.setup(myBDSize,16,fbo);
            if (currentConstellation.isFinished){
               cout <<  "placing constellation at   " << constellationBank.size() << "\n";
            if (constellationBank.size() < 4 )constellationBank.insert(constellationBank.end(), currentConstellation);
            else constellationBank[constellationCounter] = currentConstellation;
                cout << "constellation bank size is now  :  " << constellationBank.size() << "\n";
            constellationCounter =  (constellationCounter + 1) % 4;
            currentConstellation = newConst;
            cout << "constellation added \n";
            }
        }
    }
    fbo.begin();
    ofClear(255,255,255);
    fbo.end();
    for (int i = 0; i < constellationBank.size(); i++){
        constellationBank.at(i).update(i);
        constellationBank.at(i).draw();
    }
    currentConstellation.draw();
   // currentConstellation2.draw();

}

//--------------------------------------------------------------
void testApp::draw(){
    //cout << constellationBank.size() << "\n";
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
    //currentConstellation.constellationWindow.draw(myBDStart.x, myBDStart.y);
    fbo.draw(myBDStart.x, myBDStart.y);
    //currentConstellation.sphereWindow.draw(ofGetWidth()/2 + myBDStart.x, myBDStart.y);

}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    if (key == 32){
        currentConstellation.endConstellation();
        ofxOscMessage m;
        m.setAddress( "/endConst" );
        m.addIntArg(0);
        oscSender.sendMessage( m);
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
        case 0:{
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
                    oscSender.sendMessage( m );
                    //currentConstellation2.addPoint(newPoint);
                }
            }
                break;
        }
        case 2: {
            currentConstellation.endConstellation();
            ofxOscMessage m;
            m.setAddress( "/endConst" );
            m.addIntArg( currentConstellation.myStars.at(0).x );
            m.addIntArg( currentConstellation.myStars.at(0).y );
            oscSender.sendMessage(m);
            break;
        }
            
            
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
