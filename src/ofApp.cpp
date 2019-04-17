#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    //ofHideCursor();
    ofBackground(0);
    
    ///--------- Contour Finder ofxCv
    
    contourFinder.setMinAreaRadius(80);
    contourFinder.setMaxAreaRadius(1000);
    contourFinder.setSimplify(true);
    
    
    //--------- gui
    mapping.setup();
    mapping.setName("mapping");
    mapping.add(theX.set("theX", -650, -1000, 1000));
    mapping.add(theY.set("theY", -320, -1000, 1000));
    mapping.add(imageScale.set("imageScale", 2.665, 1.00, 10.00));
    
    kinectGUI.setup();
    kinectGUI.setName("kinect");
    kinectGUI.add(nearThresholds.set("nearThresholds", 30, 0, 150));
    kinectGUI.add(farThresholds.set("farThresholds", 210, 0, 255));
    kinectGUI.add(x.set("x", 20, 0, 400));
    kinectGUI.add(y.set("y", 0, 0, 400));
    kinectGUI.add(w.set("w", 600, 0, 1500));
    kinectGUI.add(h.set("h", 300, 0, 800));
    
    contourFinderGUI.setup();
    contourFinderGUI.setName("contourFinder");
    contourFinderGUI.add(simplify.set("simplify", 30.0, 0.0, 40.0));
    contourFinderGUI.add(minAreaRadius.set("minAreaRadius", 10.0, 1.0, 100.0));
    contourFinderGUI.add(maxAreaRadius.set("maxAreaRadius", 500.0, 100.0, 1000.0));
    
    
    psGUI.setup();
    psGUI.setName("ps");
    psGUI.add(particleSpeed.set("particleSpeed", 0.30, 0.05, 1.00));
    psGUI.add(addParticles.set("addParticles", 20, 1, 50));
    psGUI.add(pinkSpeedX.set("pinkSpeedX", 1.00, 0.05, 5.00));
    psGUI.add(pinkSpeedY.set("pinkSpeedY", 3.00, 0.05, 5.00));
    
    cvGui.setup();
    cvGui.setName("cvGui");
    cvGui.add(blurLevel.set("Blur Level", 17, 0, 30));
    cvGui.add(threshold.set("Threshold", 93, 0, 255));
    cvGui.add(erodeIterations.set("Erode Iterations", 0, 0, 30));
    cvGui.add(dilateIterations.set("Dilate Iterations", 8, 0, 30));
    cvGui.add(invert.set("Invert", false));
    cvGui.add(contourMinArea.set("Contour Min Area", 10, 1, 100));
    cvGui.add(contourMaxArea.set("Contour Max Area", 200, 1, 500));
    cvGui.add(contourThreshold.set("Contour Threshold", 128, 0, 255));
    cvGui.add(contourHoles.set("Contour Holes", false));
    cvGui.add(contourSimplify.set("Contour Simplify", true));
    

    ///---------- kinect v1
    
    ofSetLogLevel(OF_LOG_VERBOSE);
    
    // enable depth->video image calibration
    kinect.setRegistration(true);
    
    kinect.init();
    //kinect.init(true); // shows infrared instead of RGB video image
    //kinect.init(false, false); // disable video image (faster fps)
    
    kinect.open();        // opens first available kinect
    //kinect.open(1);    // open a kinect by id, starting with 0 (sorted by serial # lexicographically))
    //kinect.open("A00362A08602047A");    // open a kinect using it's unique serial #
    
    ///---------- kinect v2
//    kinectV2.open();
//    panel.setup("", "settings.xml", 10, 100);
//    panel.add(kinectV2.params);
//    panel.loadFromFile("settings.xml");
    
    
    ///---------- ofxCv with Kinect V1
    colorImg.allocate(kinect.width, kinect.height);
    grayImage.allocate(kinect.width, kinect.height);
    grayThreshNear.allocate(kinect.width, kinect.height);
    grayThreshFar.allocate(kinect.width, kinect.height);

    ///---------- ofxCv with Kinect V2
//    colorImg.allocate(kinectV2.width, kinectV2.height);
//    grayImage.allocate(kinectV2.width, kinectV2.height);
//    grayThreshNear.allocate(kinectV2.width, kinectV2.height);
//    grayThreshFar.allocate(kinectV2.width, kinectV2.height);
    
    ofSetFrameRate(30);
    
    //------- load song
    first.load("first.mp3");
    second.load("second.mp3");
    bgm.load("bgm.mp3");
    bgm.setVolume(0.5);
    
    ///-------- particle system
    for(unsigned int i = 0; i < totalParticles; i++){
        noiseOffset.push_back(ofVec2f(ofRandom(0,100000), ofRandom(0,100000)));
    }
    

    

}

//--------------------------------------------------------------
void ofApp::update(){
    
    ofSoundUpdate();
    
    ///------ some setup
    
    totalParticles += addParticles;
    if (totalParticles > 100){
        addParticles = 0;
    }
    
    //contourFinder.setMinAreaRadius(minAreaRadius);
    //contourFinder.setMaxAreaRadius(maxAreaRadius);
    

    
    ///------- kinect
    /// V1 V2
    
    kinect.update();
//    kinectV2.update();
    
//    if(kinectV2.isFrameNew()) {
    if(kinect.isFrameNew()) {

        /// only V2
//        texDepth.loadData( kinectV2.getDepthPixels() );
//        texRGB.loadData( kinectV2.getRgbPixels() );
        
        // load grayscale depth image from the kinect source
        /// V1 V2
//        grayImage.setFromPixels(kinectV2.getDepthPixels());
        
        grayImage.setFromPixels(kinect.getDepthPixels());
        
        // we do two thresholds - one for the far plane and one for the near plane
        // we then do a cvAnd to get the pixels which are a union of the two thresholds
        if(false){
            //            bThreshWithOpenCV) {
            grayThreshNear = grayImage;
            grayThreshFar = grayImage;
            grayThreshNear.threshold(nearThresholds, true);
            grayThreshFar.threshold(farThresholds);
            cvAnd(grayThreshNear.getCvImage(), grayThreshFar.getCvImage(), grayImage.getCvImage(), NULL);
        } else {
            
            // or we do it ourselves - show people how they can work with the pixels
            ofPixels & pix = grayImage.getPixels();
            int numPixels = pix.size();
            
            rect.set(x,y,w,h);
            
            // Region of Interest
            for(std::size_t y= 0; y < grayImage.getHeight(); y++){
                for(std::size_t x = 0; x < grayImage.getWidth(); x++) {
                    if (!rect.inside(x, y))
                    {
                        pix.setColor(x, y, 255);
                    }
                }
            }
            
            for(int i = 0; i < numPixels; i++) {
                if(255 - pix[i] > nearThresholds && 255 - pix[i] < farThresholds) {
                    pix[i] = 255;
                } else {
                    pix[i] = 0;
                }
            }
        }
        
        {/// cv part
            ofPixels & pix = grayImage.getPixels();
            //ofxCv::convertColor(kinect, binaryPixels, CV_RGB2GRAY);
            //ofxCv::blur(binaryPixels, binaryPixels, blurLevel);
            //ofxCv::threshold(binaryPixels, binaryPixels, threshold, invert);
            ofxCv::erode(pix, pix, erodeIterations);
            ofxCv::dilate(pix, pix, dilateIterations);
//            ofxCv::erode(grayImage, grayImage, erodeIterations);
//            ofxCv::dilate(grayImage, grayImage, dilateIterations);
            
            contourFinder.setMinAreaRadius(contourMinArea);
            contourFinder.setMaxAreaRadius(contourMaxArea);
            contourFinder.setThreshold(contourThreshold);
            contourFinder.setFindHoles(contourHoles);
            contourFinder.setSimplify(contourSimplify);
            
            contourFinder.findContours(pix);
            
        }
        
        
        grayImage.flagImageChanged();
        //contourFinder.findContours(grayImage);
        /// V1
        contourFinderKinect.findContours(grayImage, 10, (kinect.width * kinect.height)/2, 20, false);
        /// V2
        //contourFinderKinect.findContours(grayImage, 10, (kinectV2.width * kinectV2.height)/2, 20, false);
    }
    
    
    for (auto contourIndex = 0; contourIndex < contourFinder.size(); ++contourIndex)
    {
        ofPolyline contour = contourFinder.getPolylines()[contourIndex];
        ConvexHull convexHull(contour, hullMinumumDefectDepth);
        ofPolyline convexHullSmoothed = convexHull.convexHull();
        convexHullSmoothed.simplify(simplify);
        
        ///draw createPinkPlanes
        if (convexHullSmoothed.size() > 0)
        {
            while (pinkPlanes.size() <= 10)
            {
                createPinkPlanes();
            }
        }

    }
    
    /// bigBell will ring when the kinect first detect a contour
    /// and trigger to load a random 3d model
    if (!detectCountour && contourFinder.size() > 0)
    {
        numGradualAdd = 100;
        goDie = false;
        
        first.play();
        
        /// model starts to vanish after 7 seconds
        currentTime = ofGetElapsedTimef();
    }
    if(!reachTime && ofGetElapsedTimef() - currentTime  > timeInterval) {
        //second.play();
        goDie = true;
        opacityDecrease = true;
        
    }
    if(ofGetElapsedTimef() - currentTime > timeInterval){
        reachTime = true;
    }else{
        reachTime = false;
    }
    
    if (contourFinder.size() > 0)
    {
        
        detectCountour = true;
        
        ofPolyline ourBiggestContour = contourFinder.getPolyline(0);
        std::vector<ofPolyline> allContours;
        for (std::size_t i = 0; i < contourFinder.size(); i++)
        {
            allContours.push_back(contourFinder.getPolyline(i));
        }
        
        
        if (!isfin)
        {
            for (auto& p: ps.particles)
            {
                for (std::size_t i = 0; i < contourFinder.size(); i++)
                {
                    ofPolyline thisContour = contourFinder.getPolyline(i);
                    if (!thisContour.inside(p->position.x, p->position.y))
                    {
                        p->ageSpeed = 10;
                        p->opcDecreaseSpeed = 3.0;
                        //p->age = 95;//std::numeric_limits<uint64_t>::max();
                    }
                    else
                    {
                        p->ageSpeed = 0;
                    }
                }
                
            }
        }
        else
        {
            for (auto& p: ps.particles)
            {
                for (std::size_t i = 0; i < contourFinder.size(); i++)
                {
                    ofPolyline thisContour = contourFinder.getPolyline(i);
                    if (!thisContour.inside(p->position.x, p->position.y))
                    {
                    p->ageSpeed = 0;
                    p->opcDecreaseSpeed = 2.75;
                    //p->age = 95;//std::numeric_limits<uint64_t>::max();
                    }
                }
            }
        }
        
        if (ps.particles.size() < totalParticles)
        {
            std::size_t numNeeded = totalParticles - ps.particles.size();
            
            if(numGradualAdd < numNeeded){
                numGradualAdd += addParticles;
            }

            std::vector<glm::vec2> newParticlesC = ofApp::getRandomPositionOneTipsMultipeContour(allContours, numGradualAdd);
            
            std::vector<glm::vec2> newParticles = newParticlesC;
            
            for (auto position: newParticles)
            {
                glm::vec2 velocity;
                velocity.x = ofRandom(-particleSpeed, particleSpeed);
                velocity.y = ofRandom(-particleSpeed, particleSpeed);
                
                
                ps.particles.push_back(std::make_unique<Particle>(position, velocity));
            }
        }
        
        float time = ofGetElapsedTimef();
        
    }else{
        detectCountour = false;
    }
    
    // use gui to control particles' speed
    for (auto& p: ps.particles){
        p->displacementScale = particleSpeed;
    }
    //ps.update();
    
    // Update the pinkPlanes
    for (std::size_t i = 0; i < pinkPlanes.size(); i++)
    {
        pinkPlanes[i]->update();
    }
    
    // Create an iterator for the particle vector.
    std::vector<std::shared_ptr<baseParticle> >::iterator iter = pinkPlanes.begin();
    
    while (iter != pinkPlanes.end())
    {
        if ((*iter)->getAge() > (*iter)->maximumAge)
        {
            iter = pinkPlanes.erase(iter);
        }
        else
        {
            ++iter;
        }
    }

}

//--------------------------------------------------------------
void ofApp::draw(){
    
    /// draw yellow circle on the center of contour
    /*
    for (auto contourIndex = 0; contourIndex < contourFinder.size(); ++contourIndex)
    {
        const ofPolyline contour = contourFinder.getPolylines()[contourIndex];
        ConvexHull convexHull(contour, hullMinumumDefectDepth);
        ofPolyline convexHullSmoothed = convexHull.convexHull();
        convexHullSmoothed.simplify(simplify);
     
        glm::vec3 contourCenters = ofApp::getContourCenter(contourIndex);
        ofSetColor(255, 255, 0, 200);
        ofDrawCircle(contourCenters, 30);
    }
    */
    
    
    ofSetColor(255, 255, 255);
    
    
    if(showFourCam){
        ofPushMatrix();
        //ofTranslate(ofGetWidth()/2 + 100, 0);
        
        /// draw for kinect V1
        kinect.drawDepth(10, 10, 400, 300);
        kinect.draw(420, 10, 400, 300);
        
        grayImage.draw(10, 320, 400, 300);
        contourFinderKinect.draw(10, 320, 400, 300);

        
//        texDepth.draw(10, 10, kinectWidth, kinectHeight);
//        texRGB.draw(10 + 10 + kinectWidth, 10, kinectWidth, kinectHeight);
//        grayImage.draw(10, 10 + 10 + kinectHeight, kinectWidth, kinectHeight);
        
        
        //contourFinderKinect.draw(10 + 10 + kinectWidth, 10 + 10 + kinectHeight, kinectWidth, kinectHeight);
        ofPopMatrix();
        
        float guiY = 600;
        kinectGUI.setPosition(0, guiY);
        kinectGUI.draw();
        contourFinderGUI.setPosition(kinectGUI.getPosition().x + kinectGUI.getWidth() + 10, guiY);
        contourFinderGUI.draw();
        psGUI.setPosition(contourFinderGUI.getPosition().x + contourFinderGUI.getWidth() + 10, guiY);
        psGUI.draw();
        
        mapping.draw();
        cvGui.draw();
    }
    ///------- particle system on hand

    ofPushMatrix();
        ofScale(-1 * imageScale, -1 * imageScale, 1 * imageScale);
        ofTranslate(theX, theY);
        if(showContourFinder){
            contourFinder.draw();
        }
        //ps.draw();
        for (std::size_t i = 0; i < pinkPlanes.size(); i++)
        {
            pinkPlanes[i]->draw();
        }
    ofPopMatrix();
    
    cout << "contourFinder num:" << contourFinder.size() << endl;
    
    ///------- gui
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch (key) {
        case 'a':
            cout << " A Pressed" << endl;
            cout << "showFourCam" << endl;
            showFourCam = !showFourCam;
            break;
            
        case 'c':
            showContourFinder = !showContourFinder;
            break;
    }
    
}



std::vector<glm::vec2> ofApp::getRandomPositionOneTipsMultipeContour(const std::vector<ofPolyline>& polylines, std::size_t howManyParticles)
{
    std::vector<glm::vec2> results;
    glm::vec2 v;
    
    std::vector<ofPolyline> allContours;
    std::vector<ofPolyline> convexHullSmoothed;
    

        
    for (auto contourIndex = 0; contourIndex < contourFinder.size(); ++contourIndex)
    {
        
        ofPolyline contour = contourFinder.getPolylines()[contourIndex];
        ConvexHull convexHull(contour, hullMinumumDefectDepth);
        ofPolyline convexHullSmoothed = convexHull.convexHull();
        convexHullSmoothed.simplify(simplify);
        
        allContours.push_back(contour);
    }
    

    
    if (contourFinder.size() > 0)
    {
        for (std::size_t i = 0; i < howManyParticles; ++i)
        {
            glm::vec2 newParticle;
            bool found = false;
            do{
                
                //ofRectangle smallBox;
                std::size_t randomContour = (int)ofRandom(0,contourFinder.size()); /// cout the random number
                ofRectangle boundBox = allContours[randomContour].getBoundingBox();
                newParticle.x = ofRandom(boundBox.getMinX(), boundBox.getMaxX());
                newParticle.y = ofRandom(boundBox.getMinY(), boundBox.getMaxY());
                
                for (std::size_t i = 0; i < contourFinder.size(); i ++)
                {
                    if(allContours[i].inside(newParticle.x, newParticle.y))
                    {
                        found = true;
                        break;
                    }
                }
            }while (!found);
            
            results.push_back(newParticle);
        }
    }
    return results;
}
    
    

glm::vec3 ofApp::getContourCenter(std::size_t i){
    
    ofPolyline _contour = contourFinder.getPolylines()[i];
    glm::vec3 center = _contour.getCentroid2D();
    
    return center;
}



void ofApp::createPinkPlanes()
{
    // Create a half-sized box for calculations.
    ofPoint halfBoxSize = ofPoint(300,300,300);
    
    // Instantiate our base particle.
    std::shared_ptr<baseParticle> aParticle = std::make_shared<baseParticle>();
    
    // Change postion on convexHull
    for (auto contourIndex = 0; contourIndex < contourFinder.size(); ++contourIndex)
    {
        ofPolyline contour = contourFinder.getPolylines()[contourIndex];
        ConvexHull convexHull(contour, hullMinumumDefectDepth);
        ofPolyline convexHullSmoothed = convexHull.convexHull();
        convexHullSmoothed.simplify(simplify);
        
        for (auto point: convexHullSmoothed){
            aParticle->position = ofPoint(point.x, point.y, 0);
        }
    }
    
    // Set a random velocity.
    aParticle->velocity = ofPoint(ofRandom(-pinkSpeedX, pinkSpeedX),
                                  ofRandom(pinkSpeedY, 0),
                                  ofRandom(-3, 3));
    
    // Set a random drag.
    float drag = ofRandom(0.990, 0.999);
    
    // Our drag is the same in the x, y and z directions.
    aParticle->drag = ofPoint(drag, drag, drag);
    
    // Add the particle to our collection.
    pinkPlanes.push_back(aParticle);
}




