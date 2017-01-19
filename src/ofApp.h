#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "client.h"
#include "ofxTextInputField.h"
#include "ofxTextButton.h"
#include "ofxXmlSettings.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
        void exit();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
        ~ofApp();
    
        std::vector<Client*> clients;
    
    private:
        ofxOscReceiver      *oscReceiver;
        ofxOscSender        *oscSender;
    
        ofTrueTypeFont      verdana14;
    
        ofxTextInputField   newName;
        ofxTextInputField   newIP;
    
        ofxTextButton       addButton;
        ofxTextButton       saveButton;
    
        void                setupInputFields();
        void                deactivateInputs();
    
        void                addClient(string name, string ip);
        void                deleteClient(int &index);
    
        void                saveData();
        void                loadData();
};
