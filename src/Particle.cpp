#include "Particle.h"
#include "ofApp.h"

//------------------------------------------------------------------
Particle::Particle(){
	attractPoints = NULL;
}

//------------------------------------------------------------------
void Particle::setMode(particleMode newMode){
	mode = newMode;
}

//------------------------------------------------------------------
void Particle::setAttractPoints( vector <glm::vec3> * attract ){
	attractPoints = attract;
}

//------------------------------------------------------------------
void Particle::reset(){
	//the unique val allows us to set properties slightly differently for each particle
	uniqueVal = ofRandom(-10000, 10000);
	
	pos.x = ofRandomWidth();
	pos.y = ofRandomHeight();
    pos.z = 0; 
	
	vel.x = ofRandom(-3.9, 3.9);
	vel.y = ofRandom(-3.9, 3.9);
    vel.z = 0;
	
	frc   = glm::vec3(0,0,0);
	
	scale = ofRandom(0.5, 1.0);
	
	if( mode == PARTICLE_MODE_NOISE ){
		drag  = ofRandom(0.97, 0.99);
		vel.y = fabs(vel.y) * 3.0; //make the particles all be going down
	}else{
		drag  = ofRandom(0.95, 0.998);	
	}
}

void Particle::attractToPoint(int x, int y ){
	glm::vec3 attractPt(x, y, 0);
		frc = attractPt-pos; // we get the attraction force/vector by looking at the mouse pos relative to our pos
		frc = glm::normalize(frc); //by normalizing we disregard how close the particle is to the attraction point
		
		vel *= drag; //apply drag
		vel += frc * 0.6; //apply force
}

void Particle::repelFromPoint(int x, int y){
	glm::vec3 attractPt(ofGetMouseX(), ofGetMouseY(), 0);
	frc = attractPt-pos; 
	
	//let get the distance and only repel points close to the mouse
	float dist = glm::length(frc);
	frc = glm::normalize(frc);
	
	vel *= drag; 
	if( dist < 150 ){
		vel += -frc * 0.6; //notice the frc is negative 
	}else{
		//if the particles are not close to us, lets add a little bit of random movement using noise. this is where uniqueVal comes in handy. 			
		frc.x = ofSignedNoise(uniqueVal, pos.y * 0.01, ofGetElapsedTimef()*0.2);
		frc.y = ofSignedNoise(uniqueVal, pos.x * 0.01, ofGetElapsedTimef()*0.2);
		vel += frc * 0.04;
	}
}
//------------------------------------------------------------------
void Particle::update(){
	



	// APPLY THE FORCES BASED ON WHICH MODE WE ARE IN
	if( mode == PARTICLE_MODE_ATTRACT ){
		attractToPoint(ofGetMouseX(), ofGetMouseY());
	}
	else if( mode == PARTICLE_MODE_REPEL ){
		repelFromPoint(ofGetMouseX(), ofGetMouseY());
	}
	else if( mode == PARTICLE_MODE_NOISE ){
		//lets simulate falling snow 
		//the fake wind is meant to add a shift to the particles based on where in x they are
		//we add pos.y as an arg so to prevent obvious vertical banding around x values - try removing the pos.y * 0.006 to see the banding
		float fakeWindX = ofSignedNoise(pos.x * 0.003, pos.y * 0.006, ofGetElapsedTimef() * 0.6);
		
		frc.x = fakeWindX * 0.25 + ofSignedNoise(uniqueVal, pos.y * 0.04) * 0.6;
		frc.y = ofSignedNoise(uniqueVal, pos.x * 0.006, ofGetElapsedTimef()*0.2) * 0.09 + 0.18;

		vel *= drag; 
		vel += frc * 0.4;
		
		//we do this so as to skip the bounds check for the bottom and make the particles go back to the top of the screen
		if( pos.y + vel.y > ofGetHeight() ){
			pos.y -= ofGetHeight();
		}
	}
	else if( mode == PARTICLE_MODE_NEAREST_POINTS ){
		
		if( attractPoints ){

			// find closest attractPoint 
			glm::vec3 closestPt;
			int closest = -1; 
			float closestDist = 9999999;
			
			for(unsigned int i = 0; i < attractPoints->size(); i++){
				float lenSq = glm::length2( attractPoints->at(i)-pos );
				if( lenSq < closestDist ){
					closestDist = lenSq;
					closest = i;
				}
			}
			
			//if we have a closest point - lets calcuate the force towards it
			if( closest != -1 ){
				closestPt = attractPoints->at(closest);				
				float dist = sqrt(closestDist);
				
				//in this case we don't normalize as we want to have the force proportional to distance 
				attractToPoint(closestPt.x, closestPt.y);
				 
				//lets also limit our attraction to a certain distance and don't apply if 'f' key is pressed
				if( dist < 300 && dist > 40 && !ofGetKeyPressed('f') ){
					vel += frc * 0.003;
				}else{
					//if the particles are not close to us, lets add a little bit of random movement using noise. this is where uniqueVal comes in handy. 			
					frc.x = ofSignedNoise(uniqueVal, pos.y * 0.01, ofGetElapsedTimef()*0.2);
					frc.y = ofSignedNoise(uniqueVal, pos.x * 0.01, ofGetElapsedTimef()*0.2);
					vel += frc * 0.4;
				}
				
			}
		
		}
		
	}
	
	// UPDATE OUR POSITION
	
//if it is not paused, then update the position
if (pause == false) {
pos += vel * factor;    //multiplies by factor so it can increase or decrease its velocity (depending on whish key has been pressed)
}
	
	
	// LIMIT THE PARTICLES TO STAY ON SCREEN 
	//we could also pass in bounds to check - or alternatively do this at the ofApp level
	if( pos.x > ofGetWidth() ){
		pos.x = ofGetWidth();
		vel.x *= -1.0;
	}else if( pos.x < 0 ){
		pos.x = 0;
		vel.x *= -1.0;
	}
	if( pos.y > ofGetHeight() ){
		pos.y = ofGetHeight();
		vel.y *= -1.0;
	}
	else if( pos.y < 0 ){
		pos.y = 0;
		vel.y *= -1.0;
	}	
		
}


//------------------------------------------------------------------
void Particle::draw(){
	
	if( mode == PARTICLE_MODE_ATTRACT ){
		ofSetColor(255, 63, 180);
	}
	else if( mode == PARTICLE_MODE_REPEL ){
		ofSetColor(208, 255, 63);
	}
	else if( mode == PARTICLE_MODE_NOISE ){
		ofSetColor(99, 63, 255);
	}
	else if( mode == PARTICLE_MODE_NEAREST_POINTS ){
		ofSetColor(103, 160, 237);

	//when flag is true it keeps adding up +1 to set it to the corresponding color on the sequence (vector)
	}if (flag){
		count+=1;
	}if (count ==1){
		ofSetColor(colors[0]);
		flag = false;
	}
	if (count ==2){
		ofSetColor(colors[1]);
		flag = false;
	}
	if (count ==3){
		ofSetColor(colors[2]);
		flag = false;
	}
	if (count ==4){
		ofSetColor(colors[3]);
		flag = false;
	} if (count >4){
		count=0;
		flag=false;
	}
	//It makes it so if its in the chosen color, the inside will not be filled in.
	if (count==4){
		ofNoFill();
	}
	else {ofFill();}
	

	//checks if the particles are inside the square
	if (rectangle.getRectangleDrawn()==true && pos.x>=rectangle.getRectX() && pos.x <= rectangle.getRectX() + rectangle.getWidth() &&
		pos.y >= rectangle.getRectY() && pos.y <= rectangle.getHeight()+rectangle.getRectY()){		
		
		ofDrawCircle(pos.x, pos.y, (scale * 4.0)*5);  // Enlarge the particles when inside the rectangle
	
	}

	else {ofDrawCircle(pos.x, pos.y, scale * 4.0);}
	
}

