//
//  client.cpp
//  GearVR_OSC-Handshaker
//
//  Created by Aaron Oostdijk on 07/12/2016.
//
//

#include "client.h"

Client::Client( string name, string ip ) {
    ipaddress = ip;
    objectName = name;
    setup();
}

void Client::setup() {
    //del button
    delButton = ofRectangle(-18,-10,12,12);
    
    //font for IP/Name
    verdana14.load("verdana.ttf", 14, true, true);
    verdana14.setLineHeight(18.0f);
    verdana14.setLetterSpacing(1.037);
}

void Client::draw(float x, float y) {
    ofSetColor(255,255,255);
    verdana14.drawString(ipaddress, x, y);
    verdana14.drawString(objectName, x + 160, y);
    
    //draw the del button relative to x/y
    ofSetLineWidth(1);
    ofPushMatrix();
    ofTranslate(x,y);
    
    ofSetColor(255,0,0);
    ofDrawRectangle(delButton);
    
    ofPopMatrix();
    
    ofSetColor(255,255,255);
    verdana14.drawString("x", x + delButton.x + 2, y + delButton.y + 10);
}

bool Client::inside( float x, float y, int index ) {
    if ( delButton.inside(x,y) ) {
        ofNotifyEvent(deleteClient, index);
        return true;
    }
    
    return false;
}

ofRectangle Client::getArea() {
    return area;
}

string Client::getIP() {
    return ipaddress;
}

string Client::getName() {
    return objectName;
}
