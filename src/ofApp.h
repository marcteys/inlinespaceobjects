#pragma once

#include "ofMain.h"
#include "ofxRay.h"
#include "ofxIntersection.h"
#include "ofxAssimpModelLoader.h"
#include "ofxDatGui.h"


class ofApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
	void TestTwo();

	float sign(ofPoint p1, ofPoint p2, ofPoint p3);
	
	bool PointInTriangle(ofPoint pt, ofPoint v1, ofPoint v2, ofPoint v3);

	ofParameter<float>  startX;
	ofParameter<float>  startY;

	ofParameter<int>  nRayX;
	ofParameter<int>  nRayY;

	ofParameter<float>  spacingX;
	ofParameter<float>  spacingY;

	ofColor bgColor = ofColor::white;
	ofColor fgColor = ofColor::black;

	ofVec3f tmpRayStart;
	ofVec3f tmpRayEnd;

	ofMesh mesh;
	ofEasyCam cam;

	ofxAssimpModelLoader landscape;
	vector<ofMeshFace> faces;
	vector<IsTriangle> triangles;


	vector<IsRay> rays;
	vector<IsLine> lines;
	
	ofxIntersection is;

	ofLight light;
	ofMaterial mat;


	ofxDatGui* gui;
	vector<ofxDatGuiComponent*> components;

	void onButtonEvent(ofxDatGuiButtonEvent e);
	void onToggleEvent(ofxDatGuiToggleEvent e);
	void onSliderEvent(ofxDatGuiSliderEvent e);
	void onDropdownEvent(ofxDatGuiDropdownEvent e);
	void onMatrixEvent(ofxDatGuiMatrixEvent e);
	void onColorPickerEvent(ofxDatGuiColorPickerEvent e);
	void on2dPadEvent(ofxDatGui2dPadEvent e);
	void onTextInputEvent(ofxDatGuiTextInputEvent e);

	bool displayRay;
	bool displayLines = true;
	void DisplayRay();


	void GenerateLines();
};
