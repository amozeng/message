#include <stdio.h>
#include "ofMain.h"
#include "ofxGui.h"

class meshClass{
    
public:
    
    void setup();
    void update();
    void draw();

    void reset();
    void lineBetween();
    
    //------- get vertices from ofMesh
    vector<glm::vec3> vertices;

    //------- noise offset
    vector<ofVec3f> offsetsSphere;
    vector<ofVec3f> offsetsBox;
    
    //------- ofMesh
    ofPlanePrimitive plane;
    ofMesh planeMesh;
    
    ofMesh meshSphere;
    ofMesh meshBox;
    
    //------- parameters
    float timeScaleClick;
    float randomOpacity;
    float opacityMax, opacityMin;
    bool opacityDecrease = false;
    
    
    //------- fft
    float * fftSmooth;
    int bands;

    //------- for gui
    ofParameterGroup meshParameter;
    ofParameter<float> lineDisMin;
    ofParameter<float> lineDisMax;
    ofParameter<float> lineOpacity;
    ofParameter<float> sphereRadius;
    ofParameter<int> sphereResolution;
    ofParameter<int> angleAdd;




//    float lineDisMin = 500;
//    float lineDisMax = 550;

    
    float sphereSize;
    float sphereRes;
    float opaDecrSpeed;
    float fftSmoothScaler;

};
