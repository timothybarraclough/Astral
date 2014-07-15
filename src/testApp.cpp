#include "testApp.h"


static const int scaleFactor = 1;
static const int border = 5;
static const int granularity = 24;
static const int constellationAmount = 4;
static const int menuPosX = 20;
static const int menuPosY = 650;
static const int menuSizeX = 600;
static const int menuSizeY = 100;
//--------------------------------------------------------------
void testApp::setup(){
    ofSetFrameRate(60);
    ofSetVerticalSync(true);
    ofEnableAlphaBlending();
    //ofSetWindowShape(640*2, 480);

    oscSender.setup( HOST, PORT );
    oscReceiver.setup( InPORT );
   //bonjourServer.
    bonjourServer.startService("_osc._udp.", "Astral", InPORT,"Astral" );
    
    myBDStart = ofVec2f(20, 30);
    myBDSize = ofVec2f(502, 502);
    blur.allocate(myBDSize.x/4.0,myBDSize.y/4.0);
    bounce.allocate(myBDSize.x/4.0,myBDSize.y/4.0);
    image.allocate(myBDSize.x + (2 * border),myBDSize.y + (2 * border), OF_IMAGE_COLOR);

    
    //blur.setPasses(1);
    //blur.setRadius(2.4);
    
    fbo.allocate(myBDSize.x + (2 * border),myBDSize.y + (2 * border), GL_RGBA,8);
    myBD.setup(myBDSize, granularity,fbo);
    constellationCounter = 0;
    for (int i = 0; i < constellationAmount; i++){
    constellation c;
    c.setup(ofPoint(0,0), 0, fbo, 0, colorScheme);
    c.isFinished = false;
        constellationBank[i] = c;
    }
    currentConstellation.setup(myBDSize,granularity,fbo,constellationCounter, colorScheme);
    //constellationBank.insert(constellationBank.end(), currentConstellation);
    // currentConstellation2.setup(myBDSize,8,fbo);
    ofBackground(30,130,50);
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
    colorScheme.insert( colorScheme.end(),ofColor(203,68,68));
    colorScheme.insert( colorScheme.end(),ofColor(43,129,193));
    colorScheme.insert( colorScheme.end(),ofColor(88,188,161));
    colorScheme.insert( colorScheme.end(),ofColor(211,151,11));

   // colorScheme[1] = ofColor(43,129,193);
   // colorScheme[2] = ofColor(88,188,161);
   // colorScheme[3] = ofColor(211,151,11);
    menu.setup(myBDSize.x / 4.0,100,4, colorScheme);
    
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
            constellation &c = constellationBank[m.getArgAsInt32(0)];
            menu.flash(m.getArgAsInt32(0));
            c.collide(m.getArgAsInt32(1));
            float xp =  c.myStars.at(m.getArgAsInt32(1)).x / 4.0;
            float yp =  c.myStars.at(m.getArgAsInt32(1)).y / 4.0;
            
            blur.begin();
            ofFill();
            ofColor(255);
            ofEllipse(xp,yp,5,5);
            blur.end();
            
            
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
            // (menu.selected + 1 ) % constellationAmount;
            newConst.setup(myBDSize,granularity,fbo, (constellationCounter),colorScheme);
            
            if (currentConstellation.isFinished){
                cout <<  "placing constellation at   " << m.getArgAsInt32(0) << "\n";
                constellationBank[m.getArgAsInt32(0)] = currentConstellation;
               
               // constellationCounter +=  1;
               // constellationCounter %= constellationAmount;
                currentConstellation = newConst;
            }
        }
        
        else if(m.getAddress() == "/cursorPosition"){
            
           // cout << m.getAddress() << "     ";
           // cout << m.getArgAsInt32(0) << "     ";
           // cout << m.getArgAsInt32(1) << "\n";
            
            
            float x = m.getArgAsFloat(0) * fbo.getWidth() / 12.0;
            float y = m.getArgAsFloat(1) * fbo.getHeight()/ 12.0;
            
            myCursor.update(x,y);
        }
        
        else if(m.getAddress() == "/iOSEndConst"){
           
            keyPressed(32);/*
            if (currentConstellation.myStars.size() > 0){
                currentConstellation.endConstellation();
                ofxOscMessage m;
                m.setAddress( "/endConst" );
                m.addIntArg(menu.selected);
                oscSender.sendMessage( m);
                constellationCounter += 1;
                constellationCounter %= constellationAmount;
            cout << "received end const message \n";
            }
                            */
            
        }
        
        else if(m.getAddress() == "/iOSChangeConst"){
            
            
            cout << "received end const message \n";
            
            changeConstellation(m.getArgAsInt32(0));
            
        }
        
        else if(m.getAddress() == "/iOSAddStar"){
            float xval = m.getArgAsFloat(0);
            float yval = m.getArgAsFloat(1);
            cout <<"received Add star message to x : " <<xval << "  y : "<<yval<<"\n";

            currentConstellation.addStarByCoord(ofPoint(xval,yval));
            m.setAddress( "/addStar" );
            m.addIntArg( xval );//* currentConstellation.windowSize.x/granularity);
            m.addIntArg( yval );//* currentConstellation.windowSize.y/granularity);
            oscSender.sendMessage( m );
           // mousePressed(myCursor.position.x+myBDStart.x,myCursor.position.y+myBDStart.y,0);
            
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
        
        else if (m.getAddress() == "/latencyPing2"){
            
            cout <<"Received our Ping \n";
            iOSDevices.back().latencyTest2();
            
        }
        
        else {
            cout << m.getAddress() << "     \n";
            
        }
    }
    
    fbo.begin();
    
    ofClear(127,127,127,30);
    myBD.draw();
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
    menu.highlight(constellationCounter);
    menu.update();
    
    
    //blur << fbo.getTextureReference();
   // blur.setFade(sin( ofGetElapsedTimef() ));
    
    blur.begin();
   // blur.clear();
    //ofClear(255,255,255,0);
    //myBD.push(x,y);
  //myBD.draw();
    blur.end();
    blur.update();
    
    
    blur.update();
    //bounce.setTexture(fbo.getTextureReference(),1);
    bounce.setTexture(fbo.getTextureReference(),1);

    
    bounce << blur;
    bounce.begin();
    fbo.draw(0,0);
    bounce.end();
    bounce.update();
    
    
    
    
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
    
    //currentConstellation.constellationWindow.draw(myBDStart.x, myBDStart.y);
    
    menu.window.draw(myBDStart.x,650);
   //fbo.draw(myBDStart.x, myBDStart.y);
    //blur.draw(50, 50,400,400);
    bounce.draw(myBDStart.x,myBDStart.y);
    fbo.draw(myBDStart.x, myBDStart.y);
    myBD.draw();
    //bounce.draw(myBDStart.x, myBDStart.y);
    
}

void testApp::changeConstellation(int index){
    constellationCounter = index;
    ofxOscMessage m;
    m.setAddress( "/changeConst");
    m.addIntArg(constellationCounter);
    oscSender.sendMessage(m);
    currentConstellation.myID = constellationCounter;
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    cout << constellationCounter << "\n";
    if (key == 32){
        if (currentConstellation.myStars.size() > 0){
            currentConstellation.endConstellation();
            ofxOscMessage m;
            m.setAddress( "/endConst" );
            m.addIntArg(menu.selected);
            oscSender.sendMessage( m);
            constellationCounter += 1;
            constellationCounter %= constellationAmount;
        }
    }
    
    else if (key == 356){
        changeConstellation((constellationCounter + 3) % 4);
        

    }
    
    else if (key == 358){
        changeConstellation((constellationCounter + 1) % 4);
    }
    
    else if (key == 357){
        ofxOscMessage m;
        m.setAddress( "/changeOctave");
        m.addIntArg(constellationCounter);
        m.addIntArg(1);
        oscSender.sendMessage(m);
        
    }
    
    else if (key == 359){
        ofxOscMessage m;
        m.setAddress( "/changeOctave");
        m.addIntArg(constellationCounter);
        m.addIntArg(-1);
        oscSender.sendMessage(m);
        
    }
    
    else if (key == 49){
        changeConstellation(0);
    }
    else if (key == 50){
        changeConstellation(1);
    }
    else if (key == 51){
        changeConstellation(2);
    }
    else if (key == 52){
        changeConstellation(3);
    }
    
    else if (key == OF_KEY_SHIFT){
        exportImage();
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
                    float xsnapSize = currentConstellation.windowSize.x/granularity;
                    float ysnapSize = currentConstellation.windowSize.y/granularity;
                    float xSnap = round(newPoint.x - border,xsnapSize);
                    float ySnap = round(newPoint.y - border,ysnapSize);
                    
                    ofPoint starSnapped = ofPoint(xSnap, ySnap);
                    blur.begin();
                    ofFill();
                    ofColor(255);
                    ofEllipse(starSnapped.x/4.0,starSnapped.y/4.0,1,1);
                    blur.end();
                    
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
            
            else if (x > menuPosX  && x < menuPosX + menuSizeX){
                cout << " inside the x  \n";
                if (y > menuPosY && y < menuPosY + menuSizeY){
                int pos = y - menuPosY;
                y /= menuSizeY/constellationAmount;
                cout << "my y is set to   " << y << "  \n";
                    constellationCounter = y;
                  }
                
                
            }
            else {cout << "  x   :  " << x << "  y  :  " << y << "  \n";}
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
static bool deleteAll(ofColor * theElement){delete theElement; return true;}

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

void testApp::exportImage(){
    ofPixels hiResPixels;
    fbo.readToPixels(hiResPixels);
    ofFileDialogResult dialog_result = ofSystemSaveDialog("hi-res export", "export");
    string filename = dialog_result.getPath() + ".png";
    ofSaveImage(hiResPixels, filename);
    
}
