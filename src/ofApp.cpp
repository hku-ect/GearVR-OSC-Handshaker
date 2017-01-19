#include "ofApp.h"

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
    
    ofSetFrameRate(240);
    ofSetWindowShape(700, 200);
    ofBackground(0,0,0);
    
    ofTrueTypeFont::setGlobalDpi(72);
    
    verdana14.load("verdana.ttf", 12, true, true);
    verdana14.setLineHeight(18.0f);
    verdana14.setLetterSpacing(1.037);
    
    setupInputFields();
    loadData();
    
    doError("Running...");
}

ofApp::~ofApp() {
    delete oscReceiver;
    delete oscSender;
}


void ofApp::setupInputFields() {
    //interface to add clients
    newName.setup(ofRectangle(380, 30, 120, 24 ), 12, "Name", "Motive Rigidbody Name");
    newIP.setup(ofRectangle(380,60,120,24), 12, "127.0.0.1", "Client IP" );
    
    addButton.setup(ofRectangle(380,100,120,30), "Add", 12, ofColor(255,255,255), ofColor(50,50,50));
    saveButton.setup(ofRectangle(380,160,120,30), "Save", 12, ofColor(255,255,255), ofColor(50,50,50));
}

void ofApp::deactivateInputs()
{
    //deactivate all inputfields
    newName.deactivate();
    newIP.deactivate();
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
            doError("Handshake request from: ", remoteIP );
            for( int i = 0; i < clients.size(); ++i )
            {
                if ( remoteIP == clients[i]->getIP() )
                {
                    ofxOscMessage m;
                    m.setAddress("/gear-handshake-reply");
                    m.addStringArg(clients[i]->getName());
                    doError("Successfully handshaked as: ", clients[i]->getName() );
                    
                    oscSender->setup(remoteIP, 6505);
                    oscSender->sendMessage(m);
                    break;
                }
                
                if ( i == clients.size() - 1 )
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
        else if ( msg.getAddress() == "/gear-virtualpos" )
        {
            string remoteIP = msg.getRemoteIp();
            for( int i = 0; i < clients.size(); ++i )
            {
                //doError("Got & sent virual pos: ", remoteIP );
                if ( remoteIP != clients[i]->getIP() )
                {
                    oscSender->setup(clients[i]->getIP(), 6200);
                    oscSender->sendMessage(msg);
                }
            }
        }
    }
}

void ofApp::addClient( string name, string ip ) {
    Client *c = new Client(name,ip);
    ofAddListener(c->deleteClient, this, &ofApp::deleteClient);
    clients.push_back(c);
}

void ofApp::deleteClient(int &index) {
    ofRemoveListener(clients[index]->deleteClient, this, &ofApp::deleteClient);
    delete clients[index];
    clients.erase(clients.begin() + index);
}

//--------------------------------------------------------------
void ofApp::draw(){
    string *s = &errorLog[0];
    int count = 0;
    while ( s != NULL && count < MAX_ERRORS )
    {
        ofSetColor(255,127,0);
        verdana14.drawString(*s, 2, clients.size() * CLIENT_YSIZE + CLIENT_Y + 2 + count * 18 );
        s++;
        count++;
    }
    
    ofSetColor(0,200,0);
    verdana14.drawString("New Client", 380, 20 );
    
    newName.draw();
    newIP.draw();
    addButton.draw();
    saveButton.draw();
    
    for( int i = 0; i < clients.size(); ++i ) {
        clients[i]->draw(CLIENT_X, i * CLIENT_YSIZE + CLIENT_Y);
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (newIP.getState()) {
        newIP.addKey(key);
        return;
    }
    if (newName.getState()) {
        newName.addKey(key);
        return;
    }
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
    deactivateInputs();
    
    //clients
    for (int i = 0; i < clients.size(); i++)
    {
        if ( clients[i]->inside(x - CLIENT_X, y - ( CLIENT_Y + CLIENT_YSIZE * i ), i ) ) {
            return;
        }
    }
    
    //input fields
    if(newName.isInside(x, y)) return;
    if(newIP.isInside(x, y)) return;
    
    //add/save button
    if(addButton.isInside(x, y)) {
        addClient( newName.getText(), newIP.getText() );
        return;
    }
    if(saveButton.isInside(x, y)) saveData();
}

void ofApp::saveData() {
    ofxXmlSettings save;
    save.addTag("setup");
    save.pushTag("setup", 0);
    save.popTag();
    for (int i = 0; i < clients.size(); i++) {
        save.addTag("client");
        save.pushTag("client",i);
        save.addValue("ip", clients[i]->getIP());
        save.addValue("objectName", clients[i]->getName());
        save.popTag();
    }
    save.save("setup.xml");
}

void ofApp::loadData() {
    ofxXmlSettings data("setup.xml");
    data.pushTag("setup",0);
    data.popTag();
    
    int numClients = data.getNumTags("client");
    for (int i = 0; i < numClients; i++)
    {
        data.pushTag("client",i);
        string ip = data.getValue("ip", "127.0.0.1");
        string name = data.getValue("objectName", "unknown");
        addClient(name,ip);
        data.popTag();
    }
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

void ofApp::exit() {
    for (int i = 0; i < clients.size(); i++) {
        delete clients[i];
    }
}
