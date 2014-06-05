//
//  iOSDevice.h
//  First_Astral_Sketch_Open_Frameworks
//
//  Created by Timothy J on 4/06/14.
//
//
#pragma once
#include "ofxOsc.h"

class iOSDevice : public ofBaseApp {
    
public :    
    ofxOscSender sender;
    ofxOscReceiver receiver;
    int port;
    int IDNumber;
    string ipAddress;
    iOSDevice(string _ipAddress, int _port, int IDNumber);
    void latencyTest();
    void setVectorNumber(int i);
    void returnSetup();
    void setup();
    void close();
};





