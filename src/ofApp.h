#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxAssimpModelLoader.h"
#include "ofxKinectV2.h"
#include "ofxKinect.h"
#include "ofxCv.h"
#include "ofxOpenCv.h"
#include "CvUtils.h"

#include "Particle.h"
#include "ParticleSystem.h"
#include "baseParticle.h"
#include "sphereParticle.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
    
    
    /// debug
    bool showFourCam = true;
    bool showContourFinder = false;
		
    ///------- particle system on hand
    std::vector<glm::vec2> getRandomPositionOneTipsMultipeContour(const std::vector<ofPolyline>& polylines, std::size_t howManyParticles);
    
    
    std::size_t numGradualAdd = 5;
    std::size_t totalParticles = 30;
    
    ParticleSystem ps;
    std::vector<glm::vec2> noiseOffset;
    
    
    ///--------- Contour Finder
    ofxCv::ContourFinder contourFinder;
    float hullMinumumDefectDepth = 10;
    // a rect for region of interest
    ofRectangle rect;
    // project model on center of hand
    glm::vec3 getContourCenter(std::size_t i);
    //glm::vec3 contourCenters;
    bool detectCountour = false;
    /// contourFinder for kinect, in ofxOpenCV
    ofxCvContourFinder contourFinderKinect;
    
    ///---------- pink planes
    std::vector<std::shared_ptr<baseParticle>> pinkPlanes;
    int maxParticles = 300;
    void createPinkPlanes();

    //------- particles change controller
    bool goDie;
    bool opacityDecrease = false;
    
    //------- 10 second timer
    float timeInterval = 7.0;
    float currentTime = 0.0;
    
    //--------- core
    ofEasyCam cam;
    ofSoundPlayer first, second, bgm;
    bool isfin = false;
    bool reachTime = false;
    
    ///---------- kinect v2
    ofxKinect kinect;
//    ofTexture texDepth;
//    ofTexture texRGB;
    
    // Kinect V2 size
//    float kinectWidth = 512/2;
//    float kinectHeight = 424/2;
    
    float kinectWidth = 320/2;
    float kinectHeight = 240/2;
    
    ///---------- kinect
    
    ofxCvColorImage colorImg;
    ofxCvGrayscaleImage grayImage; // grayscale depth image
    ofxCvGrayscaleImage grayThreshNear; // the near thresholded image
    ofxCvGrayscaleImage grayThreshFar; // the far thresholded image
    
        
    ///------- gui
    
    ofxPanel mapping;
    ofParameter<float> theX, theY;
    ofParameter<float> imageScale;
    ofParameter<float> rotateDeg;
    
    ofxPanel kinectGUI;
    ofParameter<float> nearThresholds, farThresholds;
    ofParameter<float> x, y, w, h;
    // controuFinder GUI
    ofxPanel contourFinderGUI;
    ofParameter<float> simplify;
    ofParameter<float> minAreaRadius, maxAreaRadius;
    
    // particle system
    ofxPanel psGUI;
    ofParameter<float> particleSpeed;
    ofParameter<int> addParticles;
    ofParameter<float> pinkSpeedX, pinkSpeedY;
    
    ofxPanel cvGui;
    ofParameter<float> blurLevel;
    ofParameter<int> threshold;
    ofParameter<bool> invert;
    ofParameter<int> erodeIterations;
    ofParameter<int> dilateIterations;
    
    ofParameter<float> contourMinArea;
    ofParameter<float> contourMaxArea;
    ofParameter<float> contourThreshold;
    ofParameter<bool> contourHoles;
    ofParameter<bool> contourSimplify;



};
