#pragma once

#include "ofMain.h"
#include "ofxEyeTribe.h"
#include "ofxSimpleTimer.h"

struct CalibrationSettings {
    bool calibrating = false;
    bool pointStarted = false;
    int  index = -1;
    int  duration = 1000;
    vector<ofVec2f> points;
};

class testApp : public ofBaseApp
{
    
public:
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);

    CalibrationSettings settings;
    void startCalibration();
    void nextCalibrationPoint();
    void onCalibrationTimer(int &args);

    bool bDrawDebug;
    ofxEyeTribe tet;
    ofxSimpleTimer calibrationTimer;

};
