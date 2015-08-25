#include "testApp.h"

void testApp::setup()
{
    ofSetFrameRate(60);
    ofSetCircleResolution(45);
    
    tet.open();
    // tet.open(6555); //<---- if you want change device port
//
//    settings.points.push_back( ofVec2f(0.25, 0.25) );
//    settings.points.push_back( ofVec2f(0.50, 0.25) );
//    settings.points.push_back( ofVec2f(0.75, 0.25) );
//
//    settings.points.push_back( ofVec2f(0.25, 0.50) );
//    settings.points.push_back( ofVec2f(0.50, 0.50) );
//    settings.points.push_back( ofVec2f(0.75, 0.50) );
//
//    settings.points.push_back( ofVec2f(0.25, 0.75) );
//    settings.points.push_back( ofVec2f(0.50, 0.75) );
//    settings.points.push_back( ofVec2f(0.75, 0.75) );

    settings.points.push_back( ofVec2f(0.1, 0.1) );
    settings.points.push_back( ofVec2f(0.9, 0.1) );
    settings.points.push_back( ofVec2f(0.9, 0.9) );
    settings.points.push_back( ofVec2f(0.1, 0.9) );

    settings.points.push_back( ofVec2f(0.3, 0.3) );
    settings.points.push_back( ofVec2f(0.7, 0.3) );
    settings.points.push_back( ofVec2f(0.7, 0.7) );
    settings.points.push_back( ofVec2f(0.3, 0.7) );

    calibrationTimer.setup(settings.duration);
    ofAddListener(calibrationTimer.TIMER_COMPLETE, this, &testApp::onCalibrationTimer);
}

void testApp::update()
{
    calibrationTimer.update();
}

void testApp::draw()
{
    
    
    // draw background
    //-----------------------------------------------------------------------------
    ofBackgroundGradient(ofColor(150), ofColor(30));
    ofDisableAntiAliasing();
    ofSetColor(0, 0, 0);
    for (int i = 0; i < ofGetHeight(); i += 20)
    {
        ofLine(0, i, ofGetWidth(), i);
        for (int j = 0; j < ofGetWidth(); j += 20)
        {
            ofLine(j, 0, j, ofGetHeight());
        }
    }
    
    if (bDrawDebug && !settings.calibrating) {
        ofFill();
        ofSetColor(ofColor::salmon);
        for (auto& point : settings.points) {
            ofCircle(point * ofGetWindowSize(), 30);
        }
    }

    // draw gaze data
    //-----------------------------------------------------------------------------
    if (!settings.calibrating) {
        if (tet.isConnected())
        {
            ofFill();
            
            // red circle is raw gaze point
            ofSetColor(ofColor::red);
            ofCircle(tet.getPoint2dRaw(), 5);
            
            // green dot is smoothed gaze point
            ofSetColor(ofColor::green);
            ofCircle(tet.getPoint2dAvg(), 10);
            
            // when fixated is show orenge circle
            if (tet.isFix())
            {
                ofSetColor(ofColor::orange, 100);
                ofCircle(tet.getPoint2dAvg(), 40);
            }
            
            // and draw data from each eyes
            ofNoFill();
            ofSetColor(ofColor::aqua);
            ofCircle(tet.getLeftEyeRaw(), 5);
            ofCircle(tet.getRightEyeRaw(), 5);
            
            ofSetColor(ofColor::purple);
            ofCircle(tet.getLeftEyeAvg(), 5);
            ofCircle(tet.getRightEyeAvg(), 5);
            
            ofSetColor(ofColor::yellow);
            ofCircle(tet.getLeftEyePcenter().x * ofGetWidth(), tet.getLeftEyePcenter().y * ofGetHeight(), 20);
            ofCircle(tet.getRightEyePcenter().x * ofGetWidth(), tet.getRightEyePcenter().y * ofGetHeight(), 20);
        }
    } else {
        ofFill();
        if (settings.pointStarted) {
            ofSetColor(ofColor::red);
            ofCircle(settings.points[settings.index] * ofGetWindowSize(), 40);
        } else {
            ofSetColor(ofColor::salmon);
            ofCircle(settings.points[settings.index+1] * ofGetWindowSize(), 40);
        }
    }


    // get gaze data and server state
    //-----------------------------------------------------------------------------
    gtl::ServerState s = tet.getServerState();
    stringstream ss;
    ss << "fps: " << ofGetFrameRate() << endl;
    ss << endl;
    ss << "[ API ]" << endl;
    ss << "timestamp: " << tet.getTimestamp() << endl;
    ss << endl;
    ss << "[ GAZE DATA ]" << endl;
    ss << "is fixed: " << (tet.isFix() ? "TRUE" : "FALSE") << endl;
    ss << "point 2d raw: " << tet.getPoint2dRaw() << endl;
    ss << "point 2d avg: " << tet.getPoint2dAvg() << endl;
    ss << "left eye raw: " << tet.getLeftEyeRaw() << endl;
    ss << "left eye avg: " << tet.getLeftEyeAvg() << endl;
    ss << "left eye pupil size: " << tet.getLeftEyePupilSize() << endl;
    ss << "left eye pupil coordinates normalized: " << tet.getLeftEyePcenter() << endl;
    ss << "right eye raw: " << tet.getRightEyeRaw() << endl;
    ss << "right eye avg: " << tet.getRightEyeAvg() << endl;
    ss << "right eye pupil size: " << tet.getRightEyePupilSize() << endl;
    ss << "right eye pupil coordinates normalized: " << tet.getRightEyePcenter() << endl;
    ss << endl;
    ss << "[ SERVER STATE ]" << endl;
    ss << "framerate: " << s.framerate << endl;
    ss << "heartbeat interval: " << s.heartbeatinterval << endl;
    ss << "is calibrated: " << s.iscalibrated << endl;
    ss << "is calibrating: " << s.iscalibrating << endl;
    ss << "push: " << s.push << endl;
    ss << "tracker state: " << s.trackerstate << endl;
    ss << "version: " << s.version << endl;
    ss << endl;
    ss << "[ KEY ]" << endl;
    ss << "f: " << "toggle fulscreen" << endl;
    ss << "o: " << "open(connect)" << endl;
    ss << "c: " << "close(disconnect)" << endl;
    ss << "s: " << "starting server";
    
    ofSetColor(255, 255, 255);
    ofDrawBitmapString(ss.str(), 20, 20);


}

void testApp::keyPressed(int key)
{
    if (key == 'f') ofToggleFullscreen();
    if (key == 'o') tet.open();
    if (key == 'c') tet.close();
    if (key == 's') tet.startServer();
    if (key == 'C') startCalibration();
    if (key == 'r') tet.printCalibrationResult();
    if (key == 'd') bDrawDebug = !bDrawDebug;
}

void testApp::startCalibration()
{
    if (settings.calibrating) {
        tet.getTracker().calibration_abort();
    }

    settings.calibrating = true;
    settings.index = -1;
    settings.pointStarted = false;

    tet.getTracker().calibration_clear();
    tet.getTracker().calibration_start(settings.points.size());

//    nextCalibrationPoint();
    calibrationTimer.start(true);
}

void testApp::onCalibrationTimer(int &args)
{
    nextCalibrationPoint();
}

void testApp::nextCalibrationPoint()
{
//    if (settings.index == -1) {
//        ofLogNotice("Tracker", "Get ready...");
//        settings.index++;
//        return;
//    }

    if (!settings.pointStarted) {
        settings.index++;
        ofVec2f point = settings.points[settings.index] * ofGetWindowSize();
        tet.getTracker().calibration_point_start(point.x, point.y);
    } else {
        tet.getTracker().calibration_point_end();

        if (settings.index == settings.points.size()-1) {
            calibrationTimer.stop();
            settings.calibrating = false;
        }
    }
    settings.pointStarted = !settings.pointStarted;

//    if (settings.index > 0) {
////        ofLogNotice("Tracker", "Sending point end");
//        tet.getTracker().calibration_point_end();
//    }
//
//    if (settings.index < settings.points.size()) {
//        ofVec2f point = settings.points[settings.index] * ofGetWindowSize();
////        ofLogNotice("Tracker") << "Sending point start " << settings.index <<  ", " << settings.points[settings.index]
////         << ", " << point;
//        tet.getTracker().calibration_point_start(point.x, point.y);
//    } else {
////        ofLogNotice("Tracker", "Calibration finished");
//        settings.calibrating = false;
//        calibrationTimer.stop();
//    }


}