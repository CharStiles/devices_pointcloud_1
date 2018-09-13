#pragma once

#include "ofMain.h"
#include "ofxAssimpModelLoader.h"

class ofApp : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();
	void drawCol(float dir,float normal);

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
	void drawWater();
	void drawReflection();
	void drawRefraction();

	ofImage image;
	ofTrueTypeFont font;

	ofShader waterShader;
	ofShader reflectionShader;
	ofShader refractionShader;
	ofShader isaShader;
	ofShader colShader;
	ofShader shadowShader;

	ofFbo reflectionFbo;
	ofFbo refractionFbo;

	ofPlanePrimitive water;
	ofPlanePrimitive floor;
	ofCylinderPrimitive stage;

	ofxAssimpModelLoader model;
	ofxAssimpModelLoader col;

	ofMesh colMesh;
	ofVboMesh meshes[50];
	//use it exactly the same as an ofMesh, that it is actually an ofMesh, but when it's drawn, instead of uploading all the vertices to the graphics card every time call draw on it, 
	// it uploads them once when we draw for the first time and only uploads them again if they change.

	//ofMesh (and quite possibly ofVboMesh) will not do anything with the GPU until you draw, 
	ofEasyCam cam;
	ofCamera reflectionCam;

	ofTexture dudvTex;
	ofTexture normalTex;
	ofTexture colTex;
	ofLight light;

	float waterHeight; //height from floor which is 000
	float floorHeight; //height from floor which is 000
	float clearAlpha;
	bool doShader;
	int index;
	int indexTemp;
	int frameNum;
	float cx, cy;
	double speed = 0.02;
	int ptSize;
	float fogBegin;
	float fogEnd;

};

