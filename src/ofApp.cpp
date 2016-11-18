#include "ofApp.h"

//Hardcoded Gear Setup
static const char* GearIPs[] =
{
    "10.200.200.189",    //gear 0
    "10.200.200.190",    //gear 1
    "10.200.200.192",    //gear 2
    "10.200.200.193",    //gear 3
    "10.200.200.68",     //gear 4
    "10.200.200.23",     //gear 5   //TEST IP (one of the laptops/desktops)
    "10.200.200.194",       //wiebe
    "10.200.200.25",        //machiel
};

static const char* GearNames[] =
{
    "Yellow",           //gear 0
    "Blue",             //gear 1
    "Orange",           //gear 2
    "Purple",           //gear 3
    "Pink",             //gear 4
    "Yellow",           //TEST IP (one of the laptops/desktops)
    "Yellow",           //Wiebe
    "Machiel",
};

static const int NUM_GEARS = 8;
//end Hardcoded Gear Setup

const int MAX_ERRORS = 10;
static string* errorLog = new string[MAX_ERRORS];
static int currentError = 0;

void doError( const string s )
{
    if ( currentError == MAX_ERRORS )
    {
        for( int i = 0; i < MAX_ERRORS-1; ++i )
        {
            errorLog[i] = errorLog[i+1];
        }
        errorLog[currentError-1] = s;
    }
    else
    {
        errorLog[currentError++] = s;
    }
}

void doError( const string s1, const string s2 )
{
    std::stringstream stream;
    stream << s1;
    stream << s2;
    doError( stream.str() );
}

//--------------------------------------------------------------
void ofApp::setup(){
    oscReceiver = new ofxOscReceiver();
    oscReceiver->setup(5150);
    
    oscSender = new ofxOscSender();
    
    ofSetFrameRate(60);
    ofSetWindowShape(600, 200);
    ofBackground(0,0,0);
    
    ofTrueTypeFont::setGlobalDpi(72);
    
    verdana14.load("verdana.ttf", 12, true, true);
    verdana14.setLineHeight(18.0f);
    verdana14.setLetterSpacing(1.037);
    
    doError("Running...");
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
            //ofLogError("Handshake request from %s", remoteIP );
            doError("Handshake request from ", remoteIP );
            for( int i = 0; i < NUM_GEARS; ++i )
            {
                if ( remoteIP == GearIPs[i] )
                {
                    ofxOscMessage m;
                    //ofLogError("Successfully handshaked with %s", remoteIP );
                    doError("Successfully handshaked with ", remoteIP );
                    m.setAddress("/gear-handshake-reply");
                    m.addStringArg(GearNames[i]);
                    
                    oscSender->setup(remoteIP, 6505);
                    oscSender->sendMessage(m);
                    break;
                }
                
                if ( i == NUM_GEARS - 1 )
                {
                    //ofLogError("Did not find Gear IP in list: %s", remoteIP );
                    doError("Did not find Gear IP in list: ", remoteIP );
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
    string *s = &errorLog[0];
    int count = 0;
    while ( s != NULL && count < MAX_ERRORS )
    {
        verdana14.drawString(*s, 2, count * 18 + 14);
        s++;
        count++;
    }
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
