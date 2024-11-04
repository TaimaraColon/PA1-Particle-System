#pragma once

#include "ofMain.h"
#include "Particle.h"




class ofApp : public ofBaseApp{
	private:
		float factor=1;

		bool MouseDragging = false; //checks if the mouse is being dragged
		bool IsRectangleDrawn = false; // checks if the rectangle is drawn or not
		//coordinates for the rectangle
		int startx = 0;
		int starty = 0;
		int endx = 0;
		int endy = 0;
		int rectx = 0;
		int recty = 0;
		int width = 0;
		int height = 0;

	

		bool isRecording =false;
		bool isReplaying =false; 
		int timer;
		int jump;


		
	public:
		void setup();
		void update();
		void draw();
		void resetParticles();

		
		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		bool getIsRecording(){return isRecording;};
		void setIsRecording(bool ISRECORDING){this->isRecording = ISRECORDING;};
		bool getIsReplaying(){return isReplaying;};
		void setIsReplaying(bool ISREPLAYING){this->isReplaying = ISREPLAYING;};
	
		
		particleMode currentMode;
		bool lock = true;
		string currentModeStr;
		string currentKey;
		string recordStateStr;
		string keysPressedStr;
		string testStrVec;
		vector <int> keysPressed;

		//getters 
		bool getRectangleDrawn(){return IsRectangleDrawn;};
		int getRectX(){return rectx;};
		int getRectY(){return recty;};
		int getWidth(){return width;};
		int getHeight(){return height;};

		

		//setters 
		void setRectangleDrawn(bool RECTANGLEDRAWN){this->IsRectangleDrawn=RECTANGLEDRAWN;};
		void setRectX(int RECTX){this->rectx=RECTX;};
		void setRectY(int RECTY){this->recty=RECTY;};
		void setWidth(int WIDTH){this->width=WIDTH;};
		void setHeight(int HEIGHT){this->height=HEIGHT;};
		


		
		vector <ofColor> colors = {ofColor::blue,ofColor::yellow,ofColor::red,ofColor::seaGreen};
		vector <Particle> p;
		vector <glm::vec3> attractPoints;
		vector <glm::vec3> attractPointsWithMovement;
		
};
extern ofApp rectangle;	//externing rectangle so i can use it in the particle.cpp file

