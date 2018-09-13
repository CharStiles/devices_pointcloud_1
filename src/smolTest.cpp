#include "ofApp.h"

GLfloat lightOnePosition[] = { 40.0, 40, 100.0, 0.0 };
GLfloat lightOneColor[] = { 0.99, 0.99, 0.99, 1.0 };

GLfloat lightTwoPosition[] = { -400.0, 400, 1000.0, 0.0 };
GLfloat lightTwoColor[] = { 0.99, 0.99, 0.99, 1.0 };

//--------------------------------------------------------------
bool drawIsa = true;
string path = "C:\\Users\\a\\Documents\\of_v0.10.0_vs2017_release\\apps\\devices_pointcloud_1\\bin\\data\\ply3";
//string path = "E:\\kinoptic_ptclouds_dance2_500_7371_cleaned\\_500_3500";
ofDirectory dir;
int block;
size_t dirNum = 0;
string absPath = "C:\\Users\\a\\Documents\\of_v0.10.0_vs2017_release\\apps\\devices_pointcloud_1\\bin\\data\\ply3\\";
int skip;
float b;
void ofApp::setup() {
	dir.open(path);
	//OF INIT
	frameNum = 2;
	skip = 72;
	ptSize = 1;
	fogBegin = 1000;
	fogEnd = 2000;
	block = 0;
	b = -1;

	//ofEnableAlphaBlending(); // Remember if we are using transparency, we need to let openFrameworks know

	ofBackground(255, 255, 255);
	index = 0;
	clearAlpha = 100;
	ofSetVerticalSync(false); // true in mac false in windows (not sure)

	//ofSetFrameRate(30);
	waterHeight = 0;
	floorHeight = 0;

	// OPEN GL STUFF
	ofDisableArbTex(); // aallows me to use sampler2d instead of sampler2drect (some nvidia bullshit???)
	ofEnableLighting();
	ofEnableDepthTest();
	light.setup();
	light.setPosition(20, 20, 20);
	//light.setDirectional();
	light.enable();
	
	glShadeModel (GL_SMOOTH);
	glEnable(GL_CLIP_PLANE0); // enables clipping for reflection and refraction
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST); //which will draw the points as circles
	/* initialize lighting */
	    glLightfv (GL_LIGHT0, GL_POSITION, lightOnePosition);
	    glLightfv (GL_LIGHT0, GL_DIFFUSE, lightOneColor);
	    glEnable (GL_LIGHT0);
	    glLightfv (GL_LIGHT1, GL_POSITION, lightTwoPosition);
	    glLightfv (GL_LIGHT1, GL_DIFFUSE, lightTwoColor);
	    glEnable (GL_LIGHT1);
	    glEnable (GL_LIGHTING);
	glColorMaterial(GL_FRONT_AND_BACK, GL_SPECULAR);
	glEnable(GL_COLOR_MATERIAL);
	glPointSize(ptSize);

	// PLANES INIT (WATER & FLOOR)

	int planeWidth = ofGetWidth()*3;
	int planeHeight = planeWidth;
	int planeGridSize = 400;
	int planeColums = planeWidth / planeGridSize;
	int planeRows = planeHeight / planeGridSize;

	// center screen.
	cx = ofGetWidth() / 2.0;
	cy = ofGetHeight() / 2.0;
	water.set(planeWidth, planeHeight, planeColums, planeRows, OF_PRIMITIVE_TRIANGLES);
	floor.set(planeWidth, planeHeight, planeColums, planeRows, OF_PRIMITIVE_TRIANGLES);
	water.enableColors();
	//
	//water.rotateDeg(90, 1, 0, 0);
	floor.rotateDeg(90, 1, 0, 0);

	water.setPosition(0, waterHeight, 0);
	floor.setPosition(0, floorHeight, 0);

	// LOAD MODELS
	if (drawIsa) {

		//only show ply files
		dir.allowExt("ply");

		//populate the directory object
		dir.listDir();
		dirNum = dir.size();

		
		if (frameNum > dirNum) {
			frameNum = dirNum;
			cout << "framenum bigger than dirnum";
			ofLogError("not enough files");
		}
		for (int i = 0; i < frameNum; i++) {
			//meshes[i].load("ply/frame"+(ofToString(i+1))+".ply");
			//E:\ptclouds_dance3_1900-3000_clean
			//meshes[i].load("ply3/ptcloud_hd000032" + (ofToString(i + 10)) + ".ply");
			meshes[i].load(absPath + (dir.getName(i + skip)));
		}
	}
	col.loadModel("Column6.obj");
	//col.setPosition(0, -10000, 0);
	col.setScale(0.0003, -0.0003, 0.0003);
	ofLoadImage(colTex, "colTex2.jpg");
	//cout << col.getMeshNames();

	colMesh = col.getMesh(0);

	//mesh.setPosition(ofGetWidth()/2, ofGetHeight()/2, 0);
	//    light.setPosition(0, 0, 500);
	cam.setTarget(ofPoint(0, 0, 0));
	cam.setPosition(0, 10, 800);

	//load image

	ofLoadImage(dudvTex, "waterDUDV.png");
	ofLoadImage(normalTex, "waterNormal.png");
	dudvTex.setTextureWrap(GL_REPEAT, GL_REPEAT);
	normalTex.setTextureWrap(GL_REPEAT, GL_REPEAT);

	// REFLECTION REFRACTION INIT

	reflectionFbo.allocate(ofGetWidth(), ofGetHeight());
	refractionFbo.allocate(ofGetWidth(), ofGetHeight());

	refractionShader.load("refraction_shader");
	reflectionShader.load("reflection_shader");
	waterShader.load("water_shader");
	isaShader.load("reflection_shader");
	colShader.load("col_shader");
	shadowShader.load("shadow_shader");

	
}

//--------------------------------------------------------------
void ofApp::update() {
	index = (index + 1) % (frameNum * 2);

}

//--------------------------------------------------------------
void ofApp::draw() {
	ptSize = ofMap(pow(cam.getDistance(), 0.5), 0, 20, 8, 0,true);
	glPointSize(ptSize);
	ofBackground(0);
	//ofBackgroundGradient(ofColor::darkSlateGrey,ofColor::black, OF_GRADIENT_LINEAR);
	//waterHeight = -500 + (fmod((ofGetElapsedTimef() *10), 500.0f));
	waterHeight = b * 10;
	water.setPosition(0, waterHeight, 0);
	if (index >= frameNum) {
		indexTemp = (frameNum - (index - frameNum)) - 1;
		///meshes[(frameNum - (index-frameNum))-1].drawWireframe();
	}
	else {
		
		indexTemp = index;
	}

	cam.begin();
	//FLOOR
	//ofSetColor(0, 255, 255);
	//floor.drawWireframe();

	//REFLECTION
	reflectionFbo.begin();
	ofClear(0, 0, 0, 0);
	ofBackground(0);
	reflectionCam.begin();
	reflectionCam.setGlobalOrientation(cam.getGlobalOrientation());
	float distance = 2 * (cam.getPosition().y - waterHeight);
	float pitch = cam.getPitchRad() * 2;
	//float roll = cam.getRollRad() ;
	reflectionCam.setPosition(cam.getPosition().x,cam.getPosition().y-distance , cam.getPosition().z);
	reflectionCam.tiltRad(-pitch);
	//reflectionCam.rollRad(-roll);
	//cam.rollRad(-roll);
	
	drawCol(1, -waterHeight);

	reflectionShader.begin();
	reflectionShader.setUniform4f("plane", 0, 1, 0, -waterHeight);
	reflectionShader.setUniform2f("fogParams", fogBegin, fogEnd);
	if (drawIsa) {
		meshes[indexTemp].drawVertices();
	}
	reflectionShader.end();
	
	reflectionCam.end();
	reflectionFbo.end();

	cam.end();

	// REFRACTION
	refractionFbo.begin();
	ofClear(0, 0, 0, 0);
	ofBackground(0);

	cam.begin();
	refractionShader.begin();
	refractionShader.setUniform4f("plane", 0, -1, 0, waterHeight);
	refractionShader.setUniform2f("fogParams", fogBegin,fogEnd);
	if (drawIsa) {
		meshes[indexTemp].drawVertices();
	}

	refractionShader.end();
	drawCol(-1, waterHeight);

	cam.end();
	refractionFbo.end();
	cam.begin();
	// DRAW SCENE
	if (drawIsa){

		isaShader.begin();
		isaShader.setUniform4f("plane", 0, 1, 0, -waterHeight);
		isaShader.setUniform2f("fogParams", fogBegin, fogEnd);
		meshes[indexTemp].drawVertices();
		//meshes[(indexTemp + (frameNum / 2)) % frameNum].draw();
		isaShader.end();

		//DRAW SHADOW
		shadowShader.begin();
		meshes[indexTemp].drawVertices();
		shadowShader.end();

	}
	drawCol(1, -waterHeight);

	//WATER
	
	waterShader.begin();
	float mf = speed * ofGetElapsedTimef();
	float moveFactor = (fmod(mf, 1.0f)*2) - 1;

	waterShader.setUniformTexture("reflectionTexture", reflectionFbo, 0);
	waterShader.setUniformTexture("refractionTexture", refractionFbo, 1);
	waterShader.setUniformTexture("dudvMap", dudvTex, 2);
	waterShader.setUniformTexture("normalMap", normalTex, 3);
	waterShader.setUniform1f("moveFactor", moveFactor);
	waterShader.setUniform1f("texScale", 5);
	waterShader.setUniform3f("cameraPosition", cam.getX(), cam.getY(),cam.getZ());
	waterShader.setUniform3f("lightColor", 1, 1, 1);
	//	waterShader.setUniform3f("lightPosition", 50 * (cam.getDistance() / 2.), 500 * (cam.getDistance() / 2.), 200 * (cam.getDistance() / 2.));
	//waterShader.setUniform3f("lightPosition", 50 * (cam.getDistance() / 20.), 50 * (cam.getDistance() / 20.), 20 * (cam.getDistance() / 20.));
	waterShader.setUniform3f("lightPosition", 50 , 50, 200);
	waterShader.setUniform2f("fogParams", fogBegin,fogEnd);
	glPointSize(2+(ptSize*5));
	water.draw();
	glPointSize(ptSize);
	waterShader.end();

	cam.end();


}
void ofApp::drawCol(float dir,float normal) {

	colTex.bind();
	colShader.begin();
	colShader.setUniform4f("plane", 0, dir, 0, normal);
	colShader.setUniform2f("fogParams", fogBegin, fogEnd);
	ofPushMatrix();
	ofScale(30, 30, 30);
	ofRotate(-90.0f, 1.0f, 0.0f, 0.0f);
	ofRotate(210.0f, 0.0f, 0.0f, 1.0f);
	ofTranslate(0.8, .22, -18.7);
	glPointSize(ptSize * 4);
	//cout << b;
	float s = 50;
	float s2 = 1;
	colMesh.drawVertices();
	ofTranslate(-s, 0, 0);
	//ofTranslate(-2985,0,-2000);
	for (int i = -s2; i < s2; i++) {
		ofPushMatrix();
		ofTranslate(0, (i * s * (cos(i * 1.2f))), 0);
		for (int j = -s2; j < s2; j++) {
			if (i == 0 && j == 0) {
				continue;
			}

			ofTranslate((s * cos(i + j * 1.1f)), 0, abs(0.1 * cos(j)));			
			ofRotate(30.0f, 0.0f, 0.0f, 1.0f);
			//colMesh.draw();
			colMesh.drawVertices();

		}
		ofPopMatrix();

	}
	ofPopMatrix();
	glPointSize(ptSize);
	colShader.end();
	colTex.unbind();
	//
	//
}
//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	if (key == 'd') {
		drawIsa = !drawIsa;
	}
	if (key == 'r') {
		float roll = cam.getRollRad() ;

		reflectionCam.rollRad(-roll);
		cam.rollRad(-roll);

	}
	if (key == '/') {
		block = block + 1;
		if ((frameNum * (block + 1)) + skip > dirNum) {
			block = 0;
		}
		for (int i = 0; i < frameNum; i++) {
			//meshes[i].load("ply/frame"+(ofToString(i+1))+".ply");
			//E:\ptclouds_dance3_1900-3000_clean
			//meshes[i].load("ply3/ptcloud_hd000032" + (ofToString(i + 10)) + ".ply");
			//ofFile is(absPath + (dir.getName((frameNum * (block)) + skip)));
			meshes[i].load(absPath + (dir.getName((frameNum * (block)) + skip)));
		}
	}
	if (key == ',') {
		b = b - .1;
	}
	if (key == '.') {
		b = b + .1;
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

	