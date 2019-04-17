#include "meshClass.h"

//--------------------------------------------------------------
void meshClass::setup(){
    
    //------- parameters
    randomOpacity = ofRandom(200, 255);
    opacityMin = 100;
    opacityMax = 255;
    opaDecrSpeed = 0.75;
    
    timeScaleClick = 5;
    fftSmoothScaler = 3.0;
    
    sphereSize = 1;
    
    //------- fft
    
    fftSmooth = new float [8192];
    for (int i = 0; i < 8192; i++) {
        fftSmooth[i] = 0;
    }
    bands = 64;
    
    //------- mesh setup
    
    plane.set(1000, 1000);   ///dimensions for width and height in pixels
    plane.setPosition(0, 0, 0); /// position in x y z
    plane.setResolution(20, 20);
    
    planeMesh = plane.getMesh();
    
    int numVerts = planeMesh.getNumVertices();
    for (int i = 0; i < numVerts; ++i) {
        offsetsSphere.push_back(ofVec3f(ofRandom(0,100000), ofRandom(0,100000), ofRandom(0,100000)));
    }
    
    //-------
    meshParameter.add(lineDisMin.set("lineDisMin", 50, 1.0, 1000));
    meshParameter.add(lineDisMax.set("lineDisMax", 55, 1.0, 1000));
    meshParameter.add(lineOpacity.set("lineOpacity", 20, 1.0, 255));
    
    meshParameter.add(sphereRadius.set("sphereRadius", 500, 1.0, 1000));
    meshParameter.add(sphereResolution.set("sphereResolution", 3, 1, 10));
    meshParameter.add(angleAdd.set("angleAdd", 3, 1, 10));

    
}

void meshClass::reset() {
    
    opacityDecrease = false;
    opacityMin = 100;
    opacityMax = 255;
    timeScaleClick = 5;
    
//    lineDisMin = 500;
//    lineDisMax = 550;
    
    planeMesh = plane.getMesh();
    int numVerts = planeMesh.getNumVertices();
    
    for (int i = 0; i < numVerts; ++i) {
        offsetsSphere.push_back(ofVec3f(ofRandom(0,100000), ofRandom(0,100000), ofRandom(0,100000)));
    }

}



//--------------------------------------------------------------
void meshClass::update(){
    
    if(opacityDecrease){
        if(opacityMin > 0) {
            opacityMin -= opaDecrSpeed;
        }
        if(opacityMax > 0) {
            opacityMax -= opaDecrSpeed;
        }
    }
    
    plane.set(50, 50);

    
    //------ fft
    
    ofSoundUpdate();
    
    float * value = ofSoundGetSpectrum(bands);
    for(int i = 0; i < bands ; i++ ) {
        fftSmooth[i] *= 0.9f;
        if(fftSmooth[i] < value[i]) {
            fftSmooth[i] = value[i];
        }
    }
    
    
    
    //------- MESH jitter test with sound
    
    
    int numVerts = planeMesh.getNumVertices();
    
    for (int j=0; j<numVerts; ++j) {
        
        ofVec3f vert = planeMesh.getVertex(j);
        
        
        float time = ofGetElapsedTimef();

        
        ofVec3f timeOffsets = offsetsSphere[j];
        
        for(int i = 0; i < bands; i ++) {
            
            vert.x += (ofSignedNoise(time * timeScaleClick + timeOffsets.x)) * -(fftSmooth[i] * fftSmoothScaler);
            vert.y += (ofSignedNoise(time*timeScaleClick+timeOffsets.y)) * -(fftSmooth[i] * fftSmoothScaler);
            vert.z += (ofSignedNoise(time*timeScaleClick+timeOffsets.z)) * -(fftSmooth[i] * fftSmoothScaler);
            
            planeMesh.setVertex(j, vert);
        }
    }
}



//--------------------------------------------------------------
void meshClass::draw(){
    
    vertices = planeMesh.getVertices();
    
    ofPushMatrix();
    //ofTranslate(0,0,300);
    for (int i = 0; i < vertices.size(); i++)
    {
        randomOpacity = ofRandom(opacityMin, opacityMax);
        ofSetColor(255, randomOpacity);
        ofDrawSphere(vertices[i].x, vertices[i].y, vertices[i].z, sphereSize);
    }
    ofPopMatrix();
    lineBetween();
}


void meshClass::lineBetween() {
    vertices = planeMesh.getVertices();
    for(int i = 0; i < vertices.size(); i++){
        for(int j = 0; j < vertices.size(); j++){
            float distance = glm::distance(vertices[j], vertices[i]);
//            cout << distance << endl;
            if(distance > lineDisMin && distance < lineDisMax) {
                ofSetColor(255,lineOpacity);
                ofDrawLine(vertices[j], vertices[i]);
            }
        }
    }
}

