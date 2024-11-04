#pragma once
#include "ofMain.h"

enum particleMode{
	PARTICLE_MODE_ATTRACT = 0,
	PARTICLE_MODE_REPEL,
	PARTICLE_MODE_NEAREST_POINTS,
	PARTICLE_MODE_NOISE
};



class Particle{
	private:
		bool flag =false; //set flag to check what color to change to to false at first
		bool pause= false; //set pause to false by default unless key is pressed to set it to true
		float factor =1;
		
		
		

		

	

	public:
		Particle();
		
		void setMode(particleMode newMode);	
		void setAttractPoints( vector <glm::vec3> * attract );
		void attractToPoint(int, int);
		void repelFromPoint(int, int);
		void reset();
		void update();
		void draw();

		// !!GETTERS
		bool getFlag(){return flag;};
		bool getPause(){return pause;};
		float getFactor(){return factor;};
		
		

		// !!SETTERS
		void setFlag(bool FLAG){this->flag=FLAG;};
		void setPause(bool PAUSE){this->pause=PAUSE;};
		void setFactor(float FACTOR){this->factor=FACTOR;};
		
			
		
		glm::vec3 pos;
		glm::vec3 vel;
		glm::vec3 frc;
		
		float drag; 
		float uniqueVal;
		float scale;
		int count=0;


	
		

		
		particleMode mode;
	
		
		vector <ofColor> colors = {ofColor::blue,ofColor::yellow,ofColor::red,ofColor::green};
		vector <glm::vec3> * attractPoints; 
};
