// Copyright (c) 2017 Christopher Baker <https://christopherbaker.net>
// SPDX-License-Identifier:    MIT

#include "Particle.h"
#include "ofGraphics.h"


Particle::Particle()
{
    offsetX = ofRandom(0,1000);
    offsetY = ofRandom(0,1000);
    loadFont();
}


Particle::Particle(const glm::vec2& _position): position(_position)
{
    loadFont();

}


Particle::Particle(const glm::vec2& _position, const glm::vec2& _velocity):
    position(_position),
    velocity(_velocity)
{
    loadFont();

}


Particle::~Particle()
{
}

void Particle::reset(){
    
}


void Particle::update()
{
    age = age + ageSpeed;
    velocity += (gravity + impulse);
    impulse *= 0.0;
    velocity *= drag;
    position += velocity;
    opacity = opacity - opcDecreaseSpeed;
    
    time = ofGetElapsedTimef();
    timeScale = 5.0;
    displacementScale = 0.75;
    timeOffset.x = offsetX;
    timeOffset.y = offsetY;

    position.x += (ofSignedNoise(time*timeScale+timeOffset.x)) * displacementScale;
    position.y += (ofSignedNoise(time*timeScale+timeOffset.y)) * displacementScale;
    
    //age = ofMap(opacity, 0, 255, 100, 0, true);

    /// trail / history
//    glm::vec2 thePos = glm::vec2(position.x, position.y);
//    history.push_back(thePos);
//    if ( history.size() > 25 ) history.pop_back();

}


void Particle::draw() 
{
    /// draw trail
//    for (int i = 0; i < history.size(); i++)
//    {
//        glm::vec2 thePos = history[i];
//        ofSetColor(255, i*10);
//        ofDrawCircle(thePos.x, thePos.y, i);
//    }
    /// draw circle
    ofSetColor(255, opacity);
    //std::cout << opacity << std::endl;
    ofDrawCircle(position, 0.35);
    int randomNum = ofRandom(1,10); /// 1-9
    drawRandomWord(randomNum);
}


bool Particle::isDead() const
{
    return age > 100 || opacity <= 0;
}

void Particle::drawRandomWord(int randomNum){
    ofSetColor(255);
    if(randomNum == 1){
        hongdao.drawString("巛",position.x, position.y);
    }else if (randomNum == 2){
        hongdao.drawString("冖", position.x, position.y);
    }else if (randomNum == 3){
        hongdao.drawString("灬", position.x, position.y);
    }else if (randomNum == 4){
        hongdao.drawString("乚", position.x, position.y);
    }else if (randomNum == 5){
        hongdao.drawString("風", position.x, position.y);
    }else if (randomNum == 6){
        hongdao.drawString("辶", position.x, position.y);
    }else if (randomNum == 7){
        hongdao.drawString("丩", position.x, position.y);
    }else if (randomNum == 8){
        hongdao.drawString("勹", position.x, position.y);
    }else if (randomNum == 9){
        hongdao.drawString("耒", position.x, position.y);
    }

}

void Particle::loadFont(){
    
    hongdao.load("hongdao.ttf", 30);

    ofTrueTypeFontSettings settings("hongdao.ttf", 30);
    settings.antialiased = true;
    settings.addRange(ofUnicode::Latin1Supplement);
    settings.addRange(ofUnicode::Hiragana);
    settings.addRange(ofUnicode::Katakana);
    settings.addRange(ofUnicode::CJKUnified);
    settings.addRange(ofUnicode::Space);
    settings.addRange(ofUnicode::CJKLettersAndMonths);
    
    hongdao.load(settings);
}
