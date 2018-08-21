#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	oneShot = false;
	drawMesh = true;
	toPolyline = false;

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




	int x = 0;
	int y = 0;
	int p = 0;
	//ofSetWindowPosition(0, 0);
	//ofSetWindowShape(1920, 1080);

	ofxDatGuiComponent* component;


	startX.set("Start X", 0, -500, 500);
	startY.set("Start Y", 0, -500, 500);
	nRayX.set("nRayX", 5, 0, 200);
	nRayY.set("nRayY", 20, 0, 200);
	spacingX.set("spacingX", 5, 0, 200);
	spacingY.set("spacingY", 5, 0, 200);
	lineWidth.set("lineWidth", 1, 0, 20);
	simplifySkip.set("simplifySkip", 1, 1, 20);
	

	gui = new ofxDatGui();
	gui->addLabel("gui from of_parameters");
	gui->addButton("Generate");
	gui->addButton("Local");
	gui->addButton("World");
	gui->addBreak();
	gui->addSlider(startX);
	gui->addSlider(startY);
	gui->addSlider(nRayX);
	gui->addSlider(nRayY);
	gui->addSlider(spacingX);
	gui->addSlider(spacingY);
	gui->addSlider(lineWidth);
	gui->addColorPicker("Background", ofColor::white);
	gui->addColorPicker("Lines", ofColor::black);
	gui->addToggle("DisplayRay", displayRay);
	gui->addToggle("DrawLines", true);
	gui->addToggle("DrawMesh", true);
	gui->onColorPickerEvent(this, &ofApp::onColorPickerEvent);
	gui->onButtonEvent(this, &ofApp::onButtonEvent);
	gui->onToggleEvent(this, &ofApp::onToggleEvent);
	gui->onSliderEvent(this, &ofApp::onSliderEvent);
}

//--------------------------------------------------------------
void ofApp::update() {
	ofSetWindowTitle(ofToString(ofGetFrameRate()));
	int x = ofGetMouseX();
	int y = 400;
	int z = ofGetMouseY();//400;
}

void ofApp::draw() {
	
	if (oneShot) {
		ofBeginSaveScreenAsPDF("screenshot-" + ofGetTimestampString() + ".pdf",false, true);
	}

	if (toPolyline)
		DrawLocalPolyLines();
	else
		TestTwo();


	if (oneShot) {
		ofEndSaveScreenAsPDF();
		oneShot = false;
	}
}

/*
void ofApp::BuildLineMesh() {

}*/


void ofApp::DrawLocalPolyLines() {
	ofSetColor(ofColor::black);
	ofNoFill();
	ofSetLineWidth(lineWidth);
	for (auto & p : localPolyLines) {
		p.draw();
	}
}

void ofApp::DisplayRay() {
	ofBackground(bgColor);
	for (int r = 0; r < rays.size(); r++) {
		IsRay *ray = &rays.at(r);
		ray->draw();
	}
}


//--------------------------------------------------------------
void ofApp::TestTwo() {
	ofBackground(bgColor);

	cam.begin();
	if (drawMesh) {
		ofSetColor(ofColor::black,100);
		mesh.drawWireframe();
	}


	
	ofEnableDepthTest();


	switch (lineType) {
	case Polyline:
		DrawPolylines();
		break;
	case Lines:
		DrawLines();
		break;
	case Path:
		DrawPathlines();
		break;
	}
	
	if (displayRay)
		DisplayRay();

	
	ofDisableDepthTest();


	ofSetColor(ofColor::red);

	ofLine(ofPoint(startX, 150, startY), ofPoint(startX, -50, startY));
	ofLine(ofPoint(startX + nRayX * spacingX, 150, startY), ofPoint(startX + nRayX * spacingX, -50, startY));
	ofLine(ofPoint(startX + nRayX * spacingX, 150, startY + nRayY * spacingY), ofPoint(startX + nRayX * spacingX, -50, startY + nRayY * spacingY));
	ofLine(ofPoint(startX, 150, startY + nRayY * spacingY), ofPoint(startX, -50, startY + nRayY * spacingY));

	//ofBox(ofPoint(startX + (startX + nRayX * spacingX) /2, 150, startY + (startY + nRayY * spacingY)/2), startX + nRayX * spacingX, 100, startY + nRayY * spacingY);
	
	cam.end();

}



void ofApp::DrawPathlines() {
	ofSetLineWidth(lineWidth);
	ofSetColor(ofColor::red);
	ofFill();
	for (int i = 0; i < paths.size(); i++) {
		paths.at(i).draw();
	}
}

ofPath polysToPath(const vector<ofPolyline> &polylines) {
	ofPath path;
	for (int outline = 0; outline < polylines.size(); ++outline) {
		for (int i = 0; i < polylines[outline].getVertices().size(); i++) {
			if (i == 0)
				path.moveTo(polylines[outline].getVertices()[i].x, polylines[outline].getVertices()[i].y);
			else
				path.lineTo(polylines[outline].getVertices()[i].x, polylines[outline].getVertices()[i].y);
		}
		path.close();
	}
	return path;
}


void ofApp::DrawPolylines() {
	ofSetLineWidth(lineWidth);

	
	// 1st method
	/*
	for (int i = 0; i < polyLines.size(); i++) {
		polyLines.at(i).draw();
	}*/

	// 2nd method
	//polysToPath(polyLines).draw();
	

	//3rd method, quick

	clipper.clear();
	ofxPolylines maskLines;
	ofxPolylines targetPoly;


	ofxPolylines result;


	for (int i = 0; i < polyLines.size(); i++) {

		result.clear();
		targetPoly.clear();
		maskLines.clear();
		ofPolyline polyline = polyLines.at(i);

		// Get all polyLines after : 
		/*
		for (int after = i+1; after < polyLines.size(); after++) {
			ofPolyline p = polyLines.at(i);
			maskLines.push_back(p);
		}
	
		targetPoly.push_back(polyline);
		clipper.addPolylines(targetPoly, OFX_CLIPPER_SUBJECT);
		clipper.addPolylines(maskLines, OFX_CLIPPER_CLIP);

		clipper.clip(OFX_CLIPPER_DIFFERENCE, result);

		if (result.size() > 0) {
			ofFill();
			ofSetColor(ofColor::red);
			result[0].draw();
		}
		*/

		vector<ofPoint>& vertices = polyline.getVertices();
		ofSetPolyMode(OF_POLY_WINDING_NONZERO);


		ofFill();
		ofSetColor(bgColor);
		ofBeginShape();
		for (int j = 0; j < vertices.size(); j++) {
			if (oneShot) {
				ofVec3f v2d = cam.worldToScreen(vertices[j]);
				ofVertex(v2d.x, v2d.y);
			}
			else {
				ofVertex(vertices[j]);
			}
		}
		ofEndShape();


		//same thing repeted
		ofNoFill();
		ofSetColor(fgColor);
		ofBeginShape();
		for (int j = 0; j < vertices.size(); j++) {
			if (oneShot) {
				ofVec3f v2d = cam.worldToScreen(vertices[j]);
				ofVertex(v2d.x, v2d.y);
			}
			else {
				ofVertex(vertices[j]);
			}
		}
		ofEndShape();
	}

	// Create big mask

}


void ofApp::DrawLines() {

	mesh.draw();

	if (displayLines) {
		ofPushMatrix();
		ofTranslate(ofPoint(0, lineWidth, 0));
		ofSetLineWidth(lineWidth);
		ofSetColor(fgColor);
		for (int i = 0; i < lines.size(); i++) {
			lines.at(i).draw();
		}
		ofPopMatrix();

	}
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

	if (key == 's') {
		oneShot = true;
	}
	if (key == 'a') {
		gui->setVisible(false);
	} 
	if (key == 'z') {
		gui->setVisible(true);
	}
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

	if (e.target->getLabel() == "Generate") {
		switch (lineType) {
		case Polyline:
			GeneratePolylines();
			break;
		case Lines:
			GenerateLines();
			break;
		case Path:
			GeneratePathLines();
			break;
		}
	}
	else if (e.target->getLabel() == "Local")
		ConvertWorldToLocalPolyLines();
	else if (e.target->getLabel() == "World")
		toPolyline = false;

	
}



void ofApp::onColorPickerEvent(ofxDatGuiColorPickerEvent e) {
	if (e.target->getLabel() == "Background") {
		bgColor = e.color;
	}
	if (e.target->getLabel() == "Lines") {
		fgColor = e.color;
	}
}




void ofApp::GeneratePathLines() {

	rays.clear();

	//Creating ray
	ofPoint rayStart;

	IntersectionData id;
	polyLines.clear();

	ofPath tmpPath;
	bool skipThisLine = false;
	for (int x = 0; x < nRayX; x++) {
		skipThisLine = true;
		for (int y = 0; y < nRayY; y++) {

			rayStart = ofPoint(startX + x *spacingX , 200, startY + y *spacingY);
			IsRay tmpRay;
			tmpRay.set(rayStart, ofPoint(0, -200, 0));
			rays.push_back(tmpRay);

			for (int i = 0; i<triangles.size(); i++) {
				IsTriangle *tmpTri = &triangles.at(i);
				if (abs(tmpTri->getP0().x - tmpRay.getP0().x) < 8 && abs(tmpTri->getP0().z - tmpRay.getP0().z) < 8) {
					id = is.RayTriangleIntersection(triangles.at(i), tmpRay);
					if (id.isIntersection) {
						if (!skipThisLine) {
							tmpPath.lineTo(id.pos);
						}
						else {
							skipThisLine = false;
							tmpPath.close();
							paths.push_back(tmpPath);
						}
					}
				}
				//	triangles.at(i).draw();
			}


		} // end x y loop
	}

	// Colliding with the mesh
}



void ofApp::GeneratePolylines() {

	rays.clear();

	//Creating ray
	ofPoint rayStart;

	IntersectionData id;
	polyLines.clear();

	ofPoint lastHit;
	IsRay tmpRay;

	for (int x = 0; x < nRayX; x++) {
		ofPolyline tempLine;
		bool tmpLineInit = false;
		
		for (int y = 0; y < nRayY; y++) {

			rayStart = ofPoint(startX + x *spacingX, 200, startY + y *spacingY);
			tmpRay.set(rayStart, ofPoint(0, -10000, 0));
			rays.push_back(tmpRay);

			for (int i = 0; i<triangles.size(); i++) {
				IsTriangle *tmpTri = &triangles.at(i);
				if (abs(tmpTri->getP0().x - tmpRay.getP0().x) < 8 && abs(tmpTri->getP0().z - tmpRay.getP0().z) < 8) {
					id = is.RayTriangleIntersection(triangles.at(i), tmpRay);
					if (id.isIntersection) {
						if (!tmpLineInit) { // the the first ray, at the bottom
							tempLine.addVertex(id.pos.x, -distOffset, id.pos.z);
							tmpLineInit = true;
						}
						tempLine.addVertex(id.pos);
						lastHit = id.pos;
					}
				}
				//	triangles.at(i).draw();
			}
		} // end  y loop



		if (tempLine.getVertices().size() > 0) {
			tempLine.getVertices().at(0).z = tempLine.getVertices().at(0).z - distOffset;
			tempLine.getVertices().at(1).z = tempLine.getVertices().at(1).z - distOffset;
			tempLine.getVertices().at(tempLine.getVertices().size() - 1).z = tempLine.getVertices().at(tempLine.getVertices().size() - 1).z + distOffset;
		}

		tempLine.addVertex(lastHit.x, -distOffset, lastHit.z + distOffset);
		tempLine.close();
		polyLines.push_back(tempLine); // end tmp line

	} // end x loop

	// Colliding with the mesh
}


void ofApp::ConvertWorldToLocalPolyLines() {
	localPolyLines.clear();
	for (int i = 0; i < polyLines.size(); i++) {

		ofPolyline polyline;
		for (auto & vertex : polyLines.at(i).getVertices()) {
			polyline.addVertex(cam.worldToScreen(vertex));
			ofLog(OF_LOG_NOTICE, ofToString(cam.worldToScreen(vertex)));
		}
		polyline.close();
		localPolyLines.push_back(polyline);
	}
	toPolyline = true;
}

void ofApp::GenerateLines() {

	rays.clear();

	//Creating ray
	ofPoint rayStart;

	IntersectionData id;
	lines.clear();

	ofPoint prevPoint;
	prevPoint.set(ofPoint(0, 0, 0));
	IsLine tempLine;
	tempLine.set(prevPoint, prevPoint);
	IsRay tmpRay;
	IsTriangle *tmpTri;

	bool skipThisLine = false;
	for (int x = 0; x < nRayX; x++) {
		skipThisLine = true;
		for (int y = 0; y < nRayY;y++) {

			rayStart = ofPoint(startX  + x *spacingX, 200, startY + y *spacingY);
			tmpRay.set(rayStart, ofPoint(0, -200, 0));
			rays.push_back(tmpRay);

			for (int i = 0; i<triangles.size(); i++) {
				tmpTri = &triangles.at(i);
				if (abs(tmpTri->getP0().x - tmpRay.getP0().x) < 8 && abs(tmpTri->getP0().z - tmpRay.getP0().z) < 8) {

					id = is.RayTriangleIntersection(triangles.at(i), tmpRay);
					if (id.isIntersection) {
						if (!skipThisLine) {
							tempLine.set(prevPoint, id.pos);
							lines.push_back(tempLine);
						}
						else {
							skipThisLine = false;
						}
						prevPoint = id.pos;
					}
				}
					//	triangles.at(i).draw();
			}

		} // end x y loop
	}

	// Colliding with the mesh


	/*
	int count = 0;
	for (int r = 0; r < rays.size(); r++) {
		IsRay *ray = &rays.at(r);

		for (int i = 0; i<triangles.size(); i++) {
			IsTriangle *tmpTri = &triangles.at(i);
			if (abs(tmpTri->getP0().x - ray->getP0().x) < 8 && abs(tmpTri->getP0().z - ray->getP0().z) < 8) {
				id = is.RayTriangleIntersection(triangles.at(i), *ray);
				if (id.isIntersection) {
					if (count == 0) {
						
					}
					else {
						if (count == nRayY) {
							count = 0;
						}
						else {
							tempLine.set(prevPoint, id.pos);
							lines.push_back(tempLine);
						}
						prevPoint = id.pos;

					}

				}
				//	triangles.at(i).draw();
			}
		}
		count++;
	}
	*/

}


void ofApp::onToggleEvent(ofxDatGuiToggleEvent e)
{
	if(e.target->getLabel() == "DisplayRay")
		displayRay = e.target->getChecked();
	else if (e.target->getLabel() == "DrawLines")
		displayLines = e.target->getChecked();
	else if (e.target->getLabel() == "DrawMesh")
		drawMesh = e.target->getChecked();

	
}

void ofApp::onSliderEvent(ofxDatGuiSliderEvent e)
{
//	cout << "onSliderEvent: " << e.value << "::" << e.scale << endl;
}
