//
//  iOSDevice.cpp
//  First_Astral_Sketch_Open_Frameworks
//
//  Created by Timothy J on 4/06/14.
//
//

#include "iOSDevice.h"


iOSDevice::iOSDevice(string _ipAddress, int _port, int IDNumber){
    //sender.setup(_ipAddress, port);
    //sender.setup
    ipAddress = _ipAddress;
    port = _port;
    //cout << "received a setupcall at  :" << _ipAddress << " with the port " << port << "We set the IDNumber to be  " << IDNumber << "\n";
    //returnSetup();
}

void iOSDevice::latencyTest(){
    ofxOscMessage m;
    m.setAddress( "/latencyPing" );
    m.addIntArg(0);
    sender.sendMessage(m);
}



void iOSDevice::setup(){
    sender.setup(ipAddress, port);
    returnSetup();
}

void iOSDevice::returnSetup(){
    
    ofxOscMessage m;
    m.setAddress("/goodToGo");
    sender.sendMessage(m);
    //there should be a whole bunch of variables that
    //get sent to my iOS app that should be initialized and sent to the guy through an ofxMessage.
    //Maybe.
}

void iOSDevice::close(){
    
   //This actually wasn't necessary........
    
}

void iOSDevice::setVectorNumber(int i){
    ofxOscMessage m;
    m.setAddress("/setNumber");
    m.addIntArg(i);
    sender.sendMessage(m);
}

void iOSDevice::playNote(int i, int j){
    ofxOscMessage m;
    m.setAddress("/playNote");
    m.addIntArg(i);
    m.addIntArg(j);
    sender.sendMessage(m);
}