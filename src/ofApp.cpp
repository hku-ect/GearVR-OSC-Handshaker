#include "ofApp.h"

//Hardcoded Gear Setup
static const char* GearIPs[] =
{
    "10.200.200.189",    //gear 0
    "10.200.200.23",    //gear 1
    "10.200.200.46",    //gear 2
    "10.200.200.47",    //gear 3
    "10.200.200.48",    //gear 4
};

static const int NUM_GEARS = 5;
//end Hardcoded Gear Setup

static string* errorLog = new string[5];
const int MAX_ERRORS = 5;
static int currentError = 0;

void doError( const string s )
{
    errorLog[currentError] = s;
}

//--------------------------------------------------------------
void ofApp::setup(){
    oscReceiver = new ofxOscReceiver();
    oscReceiver->setup(5150);
    
    oscSender = new ofxOscSender();
    
    ofSetFrameRate(60);
    ofSetWindowShape(300, 100);
    ofBackground(0,0,0);
    
    ofTrueTypeFont::setGlobalDpi(72);
    
    verdana14.load("verdana.ttf", 14, true, true);
    verdana14.setLineHeight(18.0f);
    verdana14.setLetterSpacing(1.037);
}

ofApp::~ofApp() {
    delete oscReceiver;
    delete oscSender;
}

//--------------------------------------------------------------
void ofApp::update(){
    ofxOscMessage msg;
    while ( oscReceiver->hasWaitingMessages() )
    {
        oscReceiver->getNextMessage(msg);
        if ( msg.getAddress() == "/gear-handshake" )
        {
            string remoteIP = msg.getRemoteIp();
            ofLogError("Handshake request from %s", remoteIP );
            for( int i = 0; i < NUM_GEARS; ++i )
            {
                if ( remoteIP == GearIPs[i] )
                {
                    ofxOscMessage m;
                    ofLogError("Successfully handshaked with %s", remoteIP );
                    m.setAddress("/gear-handshake-reply");
                    m.addIntArg(i);
                    
                    oscSender->setup(remoteIP, 6505);
                    oscSender->sendMessage(m);
                    break;
                }
                
                if ( i == NUM_GEARS - 1 )
                {
                    ofLogError("Did not find Gear IP in list: %s", remoteIP );
                    ofxOscMessage m;
                    m.setAddress("/gear-handshake-badip");
                    m.addIntArg(0);
                    oscSender->setup(remoteIP, 6505);
                    oscSender->sendMessage(m);
                }
            }
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    //string *s = &errorLog[0];
    //int count = 0;
    //while ( s != NULL && count < MAX_ERRORS )
    //{
    //    verdana14.drawString(*s, 0, count * 20);
    //    s++;
    //    count++;
    //}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
