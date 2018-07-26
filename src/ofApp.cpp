#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetVerticalSync(true);
	//mesh.load("lofi-bunny.ply");
//	mesh.load("land.ply");
	landscape.loadModel("land.obj", 20);

//	landscape.setRotation(0, 90, 1, 0, 0);
//	landscape.setRotation(1, 270, 0, 0, 1);
//	landscape.setScale(0,0, 0);
//	landscape.setPosition(ofGetWidth() / 2, ofGetHeight() / 2, 0);

//	mesh = landscape;

	mesh = landscape.getMesh(0);
	faces = landscape.getMesh(0).getUniqueFaces();
	ofLogWarning("OfApp") <<  faces.size();

	for (int i = 0; i < mesh.getUniqueFaces().size(); i++) {
		IsTriangle triangle;
		ofMeshFace face = faces.at(i);
		triangle.set(face);
		triangles.push_back(triangle);
	}

	ofVec3f rayStart;
	for (int r = 0; r < 20; r ++) {
		rayStart = ofPoint(10 * r, 200, 10);
		IsRay tmpRay;
		tmpRay.set(rayStart, ofVec3f(0, -200, 0));
		rays.push_back(tmpRay);
	}




	int x = 340;
	int y = 100;
	int p = 40;
	//ofSetWindowPosition(0, 0);
	//ofSetWindowShape(1920, 1080);

	ofxDatGuiComponent* component;

	component = new ofxDatGuiButton("Generate");
	//component->setPosition(x, y);
	component->onButtonEvent(this, &ofApp::onButtonEvent);
	components.push_back(component);

	y += component->getHeight() + p;
	component = new ofxDatGuiToggle("toggle", false);
	component->setPosition(x, y);
	component->onToggleEvent(this, &ofApp::onToggleEvent);
	components.push_back(component);

	y += component->getHeight() + p;
	component = new ofxDatGuiWaveMonitor("wave\nmonitor", 3, .5);
	component->setPosition(x, y);
	components.push_back(component);

	y += component->getHeight() + p;
	component = new ofxDatGuiMatrix("matrix", 21, true);
	component->setPosition(x, y);
	component->onMatrixEvent(this, &ofApp::onMatrixEvent);
	components.push_back(component);

	y += component->getHeight() + p;
	component = new ofxDatGuiTextInput("text input", "# open frameworks #");
	component->setPosition(x, y);
	component->onTextInputEvent(this, &ofApp::onTextInputEvent);
	components.push_back(component);

	y += component->getHeight() + p;
	component = new ofxDatGuiColorPicker("color picker", ofColor::fromHex(0xFFD00B));
	component->setPosition(x, y);
	component->onColorPickerEvent(this, &ofApp::onColorPickerEvent);
	components.push_back(component);

	y = 100;
	x += component->getWidth() + p + 60;

	component = new ofxDatGuiFRM();
	component->setPosition(x, y);
	components.push_back(component);

	y += component->getHeight() + p;
	component = new ofxDatGuiSlider("slider", 0, 100, 50);
	component->setPosition(x, y);
	component->onSliderEvent(this, &ofApp::onSliderEvent);
	components.push_back(component);

	y += component->getHeight() + p;
	// capture the plotter in a variable so we can feed it values later //
	plotter = new ofxDatGuiValuePlotter("value\nplotter", -100, 100);
	plotter->setSpeed(2.0f);
	plotter->setDrawMode(ofxDatGuiGraph::LINES);
	component = plotter;
	component->setPosition(x, y);
	components.push_back(component);

	y += component->getHeight() + p;
	component = new ofxDatGui2dPad("2d pad");
	component->setPosition(x, y);
	component->on2dPadEvent(this, &ofApp::on2dPadEvent);
	components.push_back(component);

	y += component->getHeight() + p - 9;
	ofxDatGuiDropdown* dropdown;
	vector<string> options = { "one", "two", "three", "four" };
	dropdown = new ofxDatGuiDropdown("dropdown menu", options);
	dropdown->setPosition(x, y);
	dropdown->expand();
	dropdown->onDropdownEvent(this, &ofApp::onDropdownEvent);
	components.push_back(dropdown);


}

//--------------------------------------------------------------
void ofApp::update() {
	ofSetWindowTitle(ofToString(ofGetFrameRate()));

	/*
	int x = ofGetMouseX();
	int y = 400;
	int z = ofGetMouseY();//400;
	ray.set(ofPoint(0, 0, 0), ofPoint(x, y, z));*/


	float v = ofRandom(plotter->getMin(), plotter->getMax());
	plotter->setValue(v);
	for (int i = 0; i<components.size(); i++) components[i]->update();
}

void ofApp::draw() {
	TestTwo();

	for (int i = 0; i<components.size(); i++) components[i]->draw();

}

/*
void ofApp::BuildLineMesh() {

}*/

//--------------------------------------------------------------
void ofApp::TestTwo() {
	ofBackgroundGradient(ofColor(64), ofColor(0));

	cam.begin();
	ofSetColor(20, 20, 20);
	mesh.drawWireframe();


	IntersectionData id;

	int cnt = 0;
	ofPoint temp;
	IsRay rayy;
	ofVec3f rayStart;
	ofSetColor(255, 255, 255);

	for (int r = 0; r < rays.size(); r ++) {
		IsRay *ray = &rays.at(r);
		ray->draw();
	}

	for (int i = 0; i<triangles.size(); i++) {
		IsTriangle *tmpTri = &triangles.at(i);
		for (int r = 0; r < rays.size(); r ++) {
			IsRay *ray = &rays.at(r);

			if (abs(tmpTri->getP0().x - ray->getP0().x) < 8 && abs(tmpTri->getP0().z - ray->getP0().z) < 8) {
				id = is.RayTriangleIntersection(triangles.at(i), *ray);
				//Draw rays

				if (id.isIntersection) {

					ofSetColor(255, 0, 0, 100);
					ofDrawSphere(id.pos, 1);
					temp = id.pos;
					ofSetColor(200, 200, 200, 150);
				}
				else {
					ofSetColor(0, 0, 0, 0);
				}

				triangles.at(i).draw();
			}

			//	ray->getP0().x - tmpTri->getP0().x = minX;

		}


	}



	cam.end();

	//ofDrawBitmapStringHighlight("pos " + ofToString(ofGetMouseX()) + ", 400 , " + ofToString(ofGetMouseY()), 10, 30);




	// Get minimum 
	/*
	float minX = 200;
	float minY = 200;
	float minZ = 200;
	float x = abs(tmpTri->getP0().x - ray->getP0().x);
	if (x < minX)
	minX = x;

	float y = abs(tmpTri->getP0().y - ray->getP0().y);
	if (y < minY)
	minY = y;

	float z = abs(tmpTri->getP0().z - ray->getP0().z);
	if (z < minZ)
	minY = z;
	*/
}


void ofApp::TestOne() {
	ofBackgroundGradient(ofColor(64), ofColor(0));

	ofSetColor(255);
	cam.begin();

	/*
	ofSetColor(ofColor::gray);
	mesh.drawWireframe();

	glPointSize(2);
	ofSetColor(ofColor::white);
	mesh.drawVertices();
	*/


	/*

	// DISPLAy closest point
	int n = mesh.getNumVertices();
	float nearestDistance = 0;
	ofVec2f nearestVertex;
	int nearestIndex = 0;
	ofVec2f mouse(mouseX, mouseY);
	for (int i = 0; i < n; i++) {
	ofVec3f cur = cam.worldToScreen(mesh.getVertex(i));
	float distance = cur.distance(mouse);
	if (i == 0 || distance < nearestDistance) {
	nearestDistance = distance;
	nearestVertex = cur;
	nearestIndex = i;
	ofVec3f v = mesh.getVertex(i);
	tmpRayStart = v;
	tmpRayEnd = tmpRayStart;
	tmpRayEnd.z += 0.1;
	ofPushStyle();
	ofSetColor(100, 100, 255);
	ofDrawLine(tmpRayStart, tmpRayEnd);
	ofPopStyle();
	}
	}
	*/



	// ofxIntersection
	IntersectionData id;


	ofPoint rayStart;

	for (int i = 0; i<mesh.getUniqueFaces().size(); i++) {
		IsTriangle triangle;
		ofMeshFace face = mesh.getUniqueFaces().at(i);
		triangle.set(face);
		IsRay ray;

		//Draw rays
		for (int r = 0; r < 10; r += 2) {
			/*
			IsLine l1;
			l1.set(ofPoint(10 + i, 200, 10), ofPoint(10 + i, -200, 10));
			l1.draw();*/
			rayStart = ofPoint(10 + r, 200, 10);

			if (rayStart.distance(triangle.getP0()) < 10)
				continue;

			ray.set(rayStart, ofVec3f(0, -200, 0));
			ray.draw();
		}

	}




	cam.end();



	//	ofSetColor(ofColor::gray);
	//ofDrawLine(nearestVertex, mouse);


	//	ofNoFill();
	//	ofSetColor(ofColor::yellow);
	//	ofSetLineWidth(2);
	//	ofDrawCircle(nearestVertex, 4);
	//	ofSetLineWidth(1);

	//	ofVec2f offset(10, -10);
	//ofDrawBitmapStringHighlight(ofToString(nearestIndex), mouse + offset);
}



float ofApp::sign(ofPoint p1, ofPoint p2, ofPoint p3)
{
	return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
}

bool ofApp::PointInTriangle(ofPoint pt, ofPoint v1, ofPoint v2, ofPoint v3)
{
	v1.y = 0;
	v2.y = 0;
	v3.y = 0;

	bool b1, b2, b3;
	b1 = sign(pt, v1, v2) < 0.0f;
	b2 = sign(pt, v2, v3) < 0.0f;
	b3 = sign(pt, v3, v1) < 0.0f;

	return ((b1 == b2) && (b2 == b3));
}



/*
inline unsigned int s3dVecInTriangle3f2(S3Dvecfv p, ofVec3f v0, S3Dvecfv v1, S3Dvecfv v2)
{
	S3Dvec3f	e10, e20;
	S3Dvec3f vp;
	float		a, b, c, d, e, x, y, ac_bb;

	s3dVecSubtract3f(e10, v1, v0);
	s3dVecSubtract3f(e20, v2, v0);

	a = s3dVecDot3f(e10, e10);
	b = s3dVecDot3f(e10, e20);
	c = s3dVecDot3f(e20, e20);

	ac_bb = a*c - b*b;

	s3dVecSubtract3f(vp, p, v0);

	d = s3dVecDot3f(vp, e10);
	e = s3dVecDot3f(vp, e20);

	//x = d*c - e*b;
	//e = e*a + d*b;

	x = d*c - e*b;

	if (x < 0)
	{
		return false;
	}

	y = e*a - d*b;

	if (y < 0)
	{
		return false;
	}

	return (x + y) <= ac_bb;
}*/

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}



void ofApp::onButtonEvent(ofxDatGuiButtonEvent e)
{
	




}

void ofApp::onToggleEvent(ofxDatGuiToggleEvent e)
{
	cout << "onToggleEvent: " << e.target->getLabel() << "::" << e.target->getChecked() << endl;
}

void ofApp::onSliderEvent(ofxDatGuiSliderEvent e)
{
	cout << "onSliderEvent: " << e.value << "::" << e.scale << endl;
}

void ofApp::onDropdownEvent(ofxDatGuiDropdownEvent e)
{
	cout << "onDropdownEvent: " << e.child << endl;
}

void ofApp::onMatrixEvent(ofxDatGuiMatrixEvent e)
{
	cout << "onMatrixEvent: " << e.child << "::" << e.enabled << endl;
}

void ofApp::onColorPickerEvent(ofxDatGuiColorPickerEvent e)
{
	cout << "onColorPickerEvent: " << e.color << endl;
}

void ofApp::on2dPadEvent(ofxDatGui2dPadEvent e)
{
	cout << "on2dPadEvent: " << e.x << "::" << e.y << endl;
}

void ofApp::onTextInputEvent(ofxDatGuiTextInputEvent e)
{
	cout << "onButtonEvent: " << e.text << endl;
}

