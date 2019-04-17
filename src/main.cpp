#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
	ofSetupOpenGL(1024 * 1.5,768 * 1.5,OF_WINDOW);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());

}
