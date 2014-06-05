#include "testApp.h"

static const int scaleFactor = 1;
static const int border = 5;
static const int granularity = 24;
static const int constellationAmount = 4;
//--------------------------------------------------------------
void testApp::setup(){
    //ofSetFrameRate(60);
    //ofSetVerticalSync(true);

    
    oscSender.setup( HOST, PORT );
    oscReceiver.setup( InPORT );
   //bonjourServer.
    bonjourServer.startService("_osc._udp.", "Astral", InPORT,"Astral" );
    
    myBDStart = ofVec2f(20, 30);
    myBDSize = ofVec2f(600, 600);
    
    fbo.allocate(myBDSize.x + (2 * border),myBDSize.y + (2 * border), GL_RGBA,8);
    myBD.setup(myBDSize, granularity,fbo);
    constellationCounter = 0;
    currentConstellation.setup(myBDSize,granularity,fbo,constellationCounter);
    //constellationBank.insert(constellationBank.end(), currentConstellation);
    // currentConstellation2.setup(myBDSize,8,fbo);
    ofBackground(30,130,50);
    /*
     quadric = gluNewQuadric();
     gluQuadricTexture(quadric, GL_TRUE);
     gluQuadricNormals(quadric, GLU_SMOOTH);
     */
    ofDisableArbTex();
    
    bg.loadImage("bg.jpg");
    bg.resize(ofGetWidth(), ofGetHeight());
    
    ofxOscMessage m;
    m.setAddress( "/resetOF" );
    m.addStringArg("resetOF");
    oscSender.sendMessage( m);
    
    ofColor c(255,255,255,180);
    myCursor.setup(fbo,c);
    myCursor.position = ofPoint(fbo.getWidth()/2,fbo.getHeight()/2);
    
    //iOSDevices.resize(0);
}



//--------------------------------------------------------------
void testApp::update(){
    while( oscReceiver.hasWaitingMessages() )
    {
        // get the next message
        ofxOscMessage m;
        oscReceiver.getNextMessage( &m );
        if (m.getAddress() == "/pPosition")
        {
            constellationBank[m.getArgAsInt32(0)].setPos(m.getArgAsFloat(1),m.getArgAsFloat(2));
            
            
        }
        
        else if (m.getAddress() == "/collide")
        {
            constellationBank[m.getArgAsInt32(0)].collide(m.getArgAsInt32(1));
            
            
        }
        
        else if (m.getAddress() == "/resetEverythingCK"){
            
            
            for (int i = 0; i < constellationAmount; i++){
                
                constellationBank[i].zero();
            }
            
        }
        else if(m.getAddress() == "/addStar"){
            int newX = m.getArgAsInt32(0) * (myBDSize.x/granularity);
            int newY = m.getArgAsInt32(1) * (myBDSize.y/granularity);
            ofPoint newPoint = ofPoint(newX,newY);
            currentConstellation.addPoint(newPoint);
        }
        
        else if(m.getAddress() == "/addConstellation"){
            
            constellation newConst;
            
            newConst.setup(myBDSize,granularity,fbo, (constellationCounter));
            
            if (currentConstellation.isFinished){
                cout <<  "placing constellation at   " << m.getArgAsInt32(0) << "\n";
                constellationBank[m.getArgAsInt32(0)] = currentConstellation;
                constellationCounter =  (constellationCounter + 1) % 4;
                currentConstellation = newConst;
                
            }
        }
        
        else if(m.getAddress() == "/cursorPosition"){
           // cout << m.getAddress() << "     ";
           // cout << m.getArgAsInt32(0) << "     ";
           // cout << m.getArgAsInt32(1) << "\n";
            
            
            float x = m.getArgAsFloat(0) * fbo.getWidth() / 512.0;
            float y = m.getArgAsFloat(1) * fbo.getHeight()/512.0;
            myCursor.update(x,y);
        }
        
        else if(m.getAddress() == "/iOSEndConst"){
           
            keyPressed(32);
            cout << "received end const message \n";
            
        }
        
        else if(m.getAddress() == "/iOSAddStar"){
            cout <<"received Add star message \n";
            
            mousePressed(myCursor.position.x+myBDStart.x,myCursor.position.y+myBDStart.y,0);
            
        }
        
        
        else if(m.getAddress() == "/setupiOSDevice"){
            //string _host = m.getArgAsString(0);
            string _host = m.getRemoteIp();
            bool pleaseConnect = true;
            for (int i = 0; i < iOSDevices.size();i++){
                if (iOSDevices.at(i).ipAddress == _host){
                    pleaseConnect = false;
                   // iOSDevices.at(i).close();
                    iOSDevices.at(i).setup();
                }
            }
            if (pleaseConnect)
            {
            int _port = m.getArgAsInt32(0);
            cout << "we got to here at least...";
            iOSDevice j = iOSDevice(_host,_port,iOSDevices.size());
            cout << iOSDevices.size();
            iOSDevices.push_back(j);
            //iOSDevices.resize(iOSDevices.size());
            //iOSDevices.insert(iOSDevices.end(), j);
            cout << "pushed back the size and inserted our new device \n";
            
            //iOSDevices.at(iOSDevices.size()).setup();
            iOSDevices.back().setup();
            }
            else {
                
            };
            
        }
        
        else if (m.getAddress() == "/latencyPing"){
            
            cout <<"Received our Ping \n";
            iOSDevices.back().latencyTest();
            
        }
        
        else {
            cout << m.getAddress() << "     \n";
            
        }
    }
    
    fbo.begin();
    ofClear(255,255,255);
    fbo.end();
    for (int i = 0; i < 4; i++){
        if(constellationBank[i].isFinished){
            constellationBank[i].update(i);
            constellationBank[i].draw();
        }
    }
    currentConstellation.update(0);
    currentConstellation.draw();
    
        myCursor.draw();
    
   // myCursor.update();
    /*
     fbo.readToPixels(fboPixels);
     image.setFromPixels(fboPixels);
     */
    // currentConstellation2.draw();
    
}

//--------------------------------------------------------------
void testApp::draw(){
    
    ofBackground(30,30,50);
    // bg.draw(0,0);
    myBD.draw();
    //currentConstellation.constellationWindow.draw(myBDStart.x, myBDStart.y);
    fbo.draw(myBDStart.x, myBDStart.y);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    if (key == 32){
        if (currentConstellation.myStars.size() > 0){
            currentConstellation.endConstellation();
            ofxOscMessage m;
            m.setAddress( "/endConst" );
            m.addIntArg(constellationCounter);
            oscSender.sendMessage( m);
        }
    }
   // else cout << key;
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
                    float xsnapSize = currentConstellation.windowSize.x/granularity;
                    float ysnapSize = currentConstellation.windowSize.y/granularity;
                    float xSnap = round(newPoint.x - border,xsnapSize);
                    float ySnap = round(newPoint.y - border,ysnapSize);
                    
                    ofPoint starSnapped = ofPoint(xSnap, ySnap);
                    starSnapped /= xsnapSize;
                   // ofPoint newcoord = ofPoint(xSnap/xsnapSize,ySnap/ysnapSize);
                    ofxOscMessage m;
                    
                    m.setAddress( "/addStar" );
                    m.addIntArg( starSnapped.x);
                    m.addIntArg( starSnapped.y);
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

void testApp::exit(){
    cout << "chuck is kill \n no \n";
    ofFilePath p;
    ofSystem("chuck --kill");
    
}

float testApp::round(float number, float round) {
    if(fmodf(number, round) <= (round / 2))
        return number - fmodf(number, round);   //Round down
    else return number - fmodf(number, round) + round;   //Round down, then up again
}
