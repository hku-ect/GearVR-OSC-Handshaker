//
//  client.h
//  GearVR_OSC-Handshaker
//
//  Created by Aaron Oostdijk on 07/12/2016.
//
//

#ifndef client_h
#define client_h

#include <stdio.h>
#include "ofMain.h"

#define CLIENT_YSIZE    18
#define CLIENT_X        22
#define CLIENT_Y        14

class Client {
public:
    Client( string name, string ip );
    
    ofEvent<int>    deleteClient;
    
    void            draw(float x, float y);
    void            setup();
    
    ofRectangle     getArea();
    
    string          getIP();
    string          getName();
    bool            inside(float x, float y, int index);

private:
    string          ipaddress;
    string          objectName;
    
    ofRectangle     delButton;
    ofRectangle     area;
    
    ofTrueTypeFont	verdana14;
};

#endif /* client_h */
