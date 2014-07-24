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
    //FrameRate, Vertical Sync and Alpha Blending
    ofSetFrameRate(60);
    ofSetVerticalSync(true);
    ofEnableAlphaBlending();

    //OSC for chucK and Bonjour for iOS Begin
    oscSender.setup( HOST, PORT );
    oscReceiver.setup( InPORT );
    bonjourServer.startService("_osc._udp.", "Astral", InPORT,"Astral" );
    
    //This could probably be explicitly defined in a header or something.
    myBDStart = ofVec2f(20, 20);
    myBDSize = ofVec2f(502, 502);
    
    //Effect FBOs.
    /*
    blur.allocate(myBDSize.x/4.0,myBDSize.y/4.0);
    bounce.allocate(myBDSize.x/4.0,myBDSize.y/4.0);
    image.allocate(myBDSize.x + (2 * border),myBDSize.y + (2 * border), OF_IMAGE_COLOR);
    */
    //Setup my ColorScheme
    colorScheme.insert( colorScheme.end(),ofColor(203,68,68));
    colorScheme.insert( colorScheme.end(),ofColor(43,129,193));
    colorScheme.insert( colorScheme.end(),ofColor(88,188,161));
    colorScheme.insert( colorScheme.end(),ofColor(211,151,11));
    menu.setup(myBDSize.x / 4.0,100,4, colorScheme);
    
    //Setup my FBO windows for the background and constellation Window
    
    fbo.allocate(myBDSize.x + (2 * border),myBDSize.y + (2 * border), GL_RGBA,8);
    myBD.setup(myBDSize, granularity,fbo);
    
    // Setup my constellation bank -- initialize the constellations empty
    constellationCounter = 0;
    for (int i = 0; i < constellationAmount; i++){
    constellation c;
    c.setup(ofPoint(0,0), 0, fbo, 0, colorScheme);
    c.isFinished = false;
        constellationBank[i] = c;
    }
    //Make a new Constellation for holding the currently drawn constellation
    currentConstellation.setup(myBDSize,granularity,fbo,constellationCounter, colorScheme);

    //Set my Background
    ofBackground(30,130,50);
    ofDisableArbTex();

    //Send a message to chucK to reset everything if necessary.
   /*
    ofxOscMessage m;
    m.setAddress( "/resetOF" );
    m.addStringArg("resetOF");
    oscSender.sendMessage( m);
    */
    
    //Greyish Color for the iOS Cursor
    ofColor c(255,255,255,180);
    
    //Cursor for iOS
    myCursor.setup(fbo,c);
    myCursor.position = ofPoint(fbo.getWidth()/2,fbo.getHeight()/2);
    

}



//--------------------------------------------------------------
void testApp::update(){
    //If an OSC message is awaiting
    while( oscReceiver.hasWaitingMessages() )
    {
        // get the next message
        ofxOscMessage m;
        oscReceiver.getNextMessage( &m );
        
/*----------------------//CHUCK OSC MESSAGES-----------------------*/
        
        //constellation ball position
        if (m.getAddress() == "/pPosition")
        {
            constellationBank[m.getArgAsInt32(0)].setPos(m.getArgAsFloat(1),m.getArgAsFloat(2));
        }
        //collision (play note)
        else if (m.getAddress() == "/collide")
        {
            constellation &c = constellationBank[m.getArgAsInt32(0)];
            menu.flash(constellationBank[m.getArgAsInt32(0)].myID);
            c.collide(m.getArgAsInt32(1));
            /*
            float xp =  c.myStars.at(m.getArgAsInt32(1)).x / 4.0;
            float yp =  c.myStars.at(m.getArgAsInt32(1)).y / 4.0;
            */
            ofPoint CollisionPoint = c.myCoords[m.getArgAsInt32(1)];
            for(int i = 0; i < iOSDevices.size(); i++){
                iOSDevices[i].playNote(CollisionPoint.x, CollisionPoint.y);
            }
            
        }
        
        //Message from Chuck to reset everything
        else if (m.getAddress() == "/resetEverythingCK"){
            
          
            for (int i = 0; i < constellationAmount; i++){
                
                constellationBank[i].zero();
            }
           
            
        }
        //Add a star to the constellation, verified from chucK
        else if(m.getAddress() == "/addStar"){
            cout << "return add star call \n";
            int newX = m.getArgAsInt32(0) * (myBDSize.x/granularity);
            int newY = m.getArgAsInt32(1) * (myBDSize.y/granularity);
            ofPoint newPoint = ofPoint(newX,newY);
            currentConstellation.addPoint(newPoint);
        }
        //add a new constellation
        else if(m.getAddress() == "/addConstellation"){
            
            constellation newConst;
            newConst.setup(myBDSize,granularity,fbo, (constellationCounter),colorScheme);
            
            if (currentConstellation.isFinished){
                constellationBank[m.getArgAsInt32(0)] = currentConstellation;
                currentConstellation = newConst;
            }
        }
        
        
        //IOS OSC Messages
        //set cursor position
        else if(m.getAddress() == "/cursorPosition"){
            float x = m.getArgAsFloat(0) * fbo.getWidth() / 12.0;
            float y = m.getArgAsFloat(1) * fbo.getHeight()/ 12.0;
            myCursor.update(x,y);
        }
        //end constellation
        else if(m.getAddress() == "/iOSEndConst"){
            endConstellation(m.getArgAsInt32(0));
        }
        //change the constellation
        else if(m.getAddress() == "/iOSChangeConst"){
            changeConstellation(m.getArgAsInt32(0));
        }
        
        else if(m.getAddress() == "/iOSEraseConst"){
            eraseConstellation(constellationBank[m.getArgAsInt32(0)].myID);
        }
        //add a star (forward message to chucK)
        else if(m.getAddress() == "/iOSAddStar"){
            //
            float xval = m.getArgAsFloat(0);
            float yval = m.getArgAsFloat(1);
            ofxOscMessage m;
            
            m.setAddress( "/addStar" );
            m.addIntArg( (int)xval);
            m.addIntArg( (int)yval);
            oscSender.sendMessage( m );
        }
        
        //Set-up a new ios device
        else if(m.getAddress() == "/setupiOSDevice"){
            //string _host = m.getArgAsString(0);
            string _host = m.getRemoteIp();
            bool pleaseConnect = true;
            for (int i = 0; i < iOSDevices.size();i++){
                if (iOSDevices.at(i).ipAddress == _host){
                    pleaseConnect = false;
                    iOSDevices.at(i).setup();
                }
            }
            if (pleaseConnect)
            {
            int _port = m.getArgAsInt32(0);
            iOSDevice j = iOSDevice(_host,_port,iOSDevices.size());
            cout << iOSDevices.size();
            iOSDevices.push_back(j);
            iOSDevices.back().setup();
            }
            else {
                
            };
            
        }
        //Test iOS Latency
        else if (m.getAddress() == "/latencyPing"){
            iOSDevices.back().latencyTest();
            
        }

        
        //IF message is not defined, print out the address of the message
        else {

            
        }
    }
    //Clear drawing FBO.
    fbo.begin();
    ofClear(127,127,127,0);
    //Draw background to fbo
    myBD.draw();
    fbo.end();
    
    //For every constellation in the bank, draw into the FBO
    for (int i = 0; i < 4; i++){
        if(constellationBank[i].isFinished){
            constellationBank[i].update(i);
            constellationBank[i].draw();
        }
    }
    //update our current constellation
    currentConstellation.update(0);
    //draw it to the fbo
    currentConstellation.draw();
    //draw the cursor
    myCursor.draw();
    //highlight our current constellation in the menu bar
    menu.highlight(constellationCounter);
    //update the menu bar.
    menu.update();

    
}

//--------------------------------------------------------------
void testApp::draw(){
    //Color background
    ofBackground(30,30,50);
    //Draw menu bar
    menu.window.draw(myBDStart.x,542);
    //Draw fbo
    fbo.draw(myBDStart.x, myBDStart.y);

    
}

//Change the constellation to the index
void testApp::changeConstellation(int index){
    constellationCounter = index;
    //Send message to chucK
    ofxOscMessage m;
    m.setAddress( "/changeConst");
    m.addIntArg(constellationCounter);
    oscSender.sendMessage(m);
    //Set current constellation to the id we suggested
    currentConstellation.myID = constellationCounter;
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    cout << constellationCounter << "\n";
    switch (key){
        case 32: {
            endConstellation(menu.selected);
            break;
        }
        case 356: {
            changeConstellation((constellationCounter + 3) % 4);
            break;
        }
        case 358: {
            changeConstellation((constellationCounter + 3) % 4);
            break;
        }
            
            //Up Key, raise an octave (not implemented in chucK)
        case 357:{
                ofxOscMessage m;
                m.setAddress( "/changeOctave");
                m.addIntArg(constellationCounter);
                m.addIntArg(1);
                oscSender.sendMessage(m);
                break;
            }
            //Down Key, lower an octave (not implemented in chucK)
        case 359:{
                ofxOscMessage m;
                m.setAddress( "/changeOctave");
                m.addIntArg(constellationCounter);
                m.addIntArg(-1);
                oscSender.sendMessage(m);
            break;
            }
            //1 key --> 1st constellation
        case 49:{
                changeConstellation(0);
            break;
            }
            //2 key --> 2nd constellation
        case 50:{
                changeConstellation(1);
            break;
            }
            //3 key --> 3rd constellation
        case 51:{
                changeConstellation(2);
            break;
            }
            //4 key --> 4th constellation
        case 52:{
                changeConstellation(3);
            break;
            }
            //Shift key Export an image of the FBO
        case OF_KEY_SHIFT:{
                exportImage();
            break;
            }
            //Q button
        case 113:{
            eraseConstellation(0);
            break;
        }
            //W button
        case 119:{
            eraseConstellation(1);
            break;
        }
            //E button
        case 101:{
            eraseConstellation(2);
            break;
        }
            //R Button
        case 114:{
            eraseConstellation(3);
            break;
        }
           //Any other button
        default :{
            cout << key << "\n";
            break;
        }
            
    }
    /*
    //Space bar, end constellation
    if (key == 32){
        endConstellation(menu.selected);
        
        
    }

    //left key --> go back a constellation
    else if (key == 356){
        changeConstellation((constellationCounter + 3) % 4);
        

    }
    //right key --> go forward a constellation
    else if (key == 358){
        changeConstellation((constellationCounter + 1) % 4);
    }
    
    //Up Key, raise an octave (not implemented in chucK)
    else if (key == 357){
        ofxOscMessage m;
        m.setAddress( "/changeOctave");
        m.addIntArg(constellationCounter);
        m.addIntArg(1);
        oscSender.sendMessage(m);
        
    }
    //Down Key, lower an octave (not implemented in chucK)
    else if (key == 359){
        ofxOscMessage m;
        m.setAddress( "/changeOctave");
        m.addIntArg(constellationCounter);
        m.addIntArg(-1);
        oscSender.sendMessage(m);
        
    }
    //1 key --> 1st constellation
    else if (key == 49){
        changeConstellation(0);
    }
    //2 key --> 2nd constellation
    else if (key == 50){
        changeConstellation(1);
    }
    //3 key --> 3rd constellation
    else if (key == 51){
        changeConstellation(2);
    }
    //4 key --> 4th constellation
    else if (key == 52){
        changeConstellation(3);
    }
    //Shift key Export an image of the FBO
    else if (key == OF_KEY_SHIFT){
        exportImage();
    }
    
    else{
        exportImage();
    }
     */
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
            //Left mouse press
        case 0:{
            
            //Within the fboFrame
            if (x > myBDStart.x && x < myBDSize.x + myBDStart.x + (border * 2)){
                
                if (y > myBDStart.y&& y < myBDSize.y + (border * 2) + myBDStart.y){
                    //Snap the point to the grid and turn into co-ordinates
                    ofVec2f newPoint = ofVec2f(x - myBDStart.x * scaleFactor,y - myBDStart.y * scaleFactor);
                    float xsnapSize = currentConstellation.windowSize.x/granularity;
                    float ysnapSize = currentConstellation.windowSize.y/granularity;
                    float xSnap = round(newPoint.x - border,xsnapSize);
                    float ySnap = round(newPoint.y - border,ysnapSize);
                    
                    ofPoint starSnapped = ofPoint(xSnap, ySnap);
                    starSnapped /= xsnapSize;

                    //Add star via chucK
                    ofxOscMessage m;
                    m.setAddress( "/addStar" );
                    m.addIntArg( starSnapped.x);
                    m.addIntArg( starSnapped.y);
                    oscSender.sendMessage( m );
                }
            }
            
            //IF press is within the menu, switch to the menu item // change constellation
            else if (x > menuPosX  && x < menuPosX + menuSizeX){
                cout << " inside the x  \n";
                if (y > menuPosY && y < menuPosY + menuSizeY){
                int pos = y - menuPosY;
                y /= menuSizeY/constellationAmount;
                cout << "my y is set to   " << y << "  \n";
                    constellationCounter = y;
                  }
                
                
            }
            //OR else print where the press occurred
            else {cout << "  x   :  " << x << "  y  :  " << y << "  \n";}
            break;
        }
        default:
            break;
    }
    
}

//Method for ending the constellation, sends message to chucK
void testApp::endConstellation(int index){
    
    if (currentConstellation.myStars.size() > 0){
        currentConstellation.endConstellation();
        ofxOscMessage m;
        m.setAddress( "/endConst" );
        m.addIntArg(index);
        oscSender.sendMessage(m);
        constellationCounter += 1;
        constellationCounter %= constellationAmount;
    }
    
}

void testApp::eraseConstellation(int index){
    
    constellationBank[index].zero();
    ofxOscMessage m;
    m.setAddress( "/eraseConst" );
    m.addIntArg(constellationBank[index].myID);
    oscSender.sendMessage(m);
    
    
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
//Delete something
static bool deleteAll(ofColor * theElement){delete theElement; return true;}

//Kill chucK on destruction
void testApp::exit(){

    cout << "chuck is kill \n no \n";
    ofFilePath p;
    ofSystem("chuck --kill");
    
}

//Method for rounding this should be a static method in a utility class.
float testApp::round(float number, float round) {
    if(fmodf(number, round) <= (round / 2))
        return number - fmodf(number, round);   //Round down
    else return number - fmodf(number, round) + round;   //Round down, then up again
}

//Export the FBO as an image (triggered by shift button)
void testApp::exportImage(){
   /* ofPixels hiResPixels;
    fbo.readToPixels(hiResPixels);
    ofFileDialogResult dialog_result = ofSystemSaveDialog("hi-res export", "export");
    string filename = dialog_result.getPath() + ".png";
    ofSaveImage(hiResPixels, filename);
    */
}
