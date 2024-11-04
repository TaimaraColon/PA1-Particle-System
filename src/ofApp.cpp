
#include "ofApp.h"
#include "Particle.h"
using namespace std; 

ofApp rectangle;	

//--------------------------------------------------------------
void ofApp::setup(){


	ofSetVerticalSync(true);
	
	int num = 1500;
	p.assign(num, Particle());
	currentMode = PARTICLE_MODE_ATTRACT;

	currentModeStr = "1 - PARTICLE_MODE_ATTRACT: attracts to mouse";

	timer =-1;
	jump =-1; 
	

	resetParticles();
}

//--------------------------------------------------------------
void ofApp::resetParticles(){

	//these are the attraction points used in the fourth demo 
	attractPoints.clear();
	for(int i = 0; i < 4; i++){
		attractPoints.push_back( glm::vec3( ofMap(i, 0, 4, 100, ofGetWidth()-100) , ofRandom(100, ofGetHeight()-100) , 0) );
	}
	
	attractPointsWithMovement = attractPoints;
	
	for(unsigned int i = 0; i < p.size(); i++){
		p[i].setMode(currentMode);		
		p[i].setAttractPoints(&attractPointsWithMovement);;
		p[i].reset();
	}	
}

//--------------------------------------------------------------
void ofApp::update(){
	for(unsigned int i = 0; i < p.size(); i++){
		p[i].setMode(currentMode);
		p[i].setFactor(this->factor);
		p[i].update();
	}
	
	//lets add a bit of movement to the attract points
	for(unsigned int i = 0; i < attractPointsWithMovement.size(); i++){
		attractPointsWithMovement[i].x = attractPoints[i].x + ofSignedNoise(i * 10, ofGetElapsedTimef() * 0.7) * 12.0;
		attractPointsWithMovement[i].y = attractPoints[i].y + ofSignedNoise(i * -10, ofGetElapsedTimef() * 0.7) * 12.0;
	}
	
		if (isReplaying){
		
		timer+=1;
		if (timer == 300){           //set it so that each action of the replay is of about 3 seconds between keys
			jump+=1;                 //jumps to the next !
			
			lock=false;              //lock equal to false so that the action happens
			keyPressed((int)keysPressed[jump]);    // action of keys changing ! (replay)
			lock=true;               // lock is true so that no outside intervention with keys can affect it
			
			timer=0;
		}if (jump==keysPressed.size()){
			keysPressed.clear();      //clears the vector for next time it records
			currentKey =" ";         //clears key string for the next round of recording
			jump=-1;
			isReplaying=false;
			recordStateStr=" ";
		}	
	}	
}



//--------------------------------------------------------------
void ofApp::draw(){
	
    ofBackgroundGradient(ofColor(60,60,60), ofColor(10,10,10));
	
	
	
	for(unsigned int i = 0; i < p.size(); i++){
		p[i].draw();
	}
	
	ofSetColor(190);
	if( currentMode == PARTICLE_MODE_NEAREST_POINTS ){
		for(unsigned int i = 0; i < attractPoints.size(); i++){
			ofNoFill();
			ofDrawCircle(attractPointsWithMovement[i], 10);
			ofFill();
			ofDrawCircle(attractPointsWithMovement[i], 4);
		}
	}

	ofSetColor(230);	
	ofDrawBitmapString(currentModeStr + "\n\nSpacebar to reset. \nKeys 1-4 to change mode.\n\nKey D to make particles move faster\nKey A to make particles move slower\nKey T to change colors\nKey S to pause\nKey R to record\nKey P to replay", 10, 20);
	
	//Draws the Magnifying Rectangle
	if (IsRectangleDrawn){
		ofSetColor(0,250,255,60); //makes the rectangle light blue-ish and semi transparent.
		ofFill(); //Makes sure the bubbles spec doesn't make the inside of the rectangle dissapear.
		ofDrawRectangle(rectx,recty,width,height);
	
	}
	
	ofDrawBitmapString(recordStateStr,750,40); //Recording or replaying state indicator !
	
	ofSetColor(ofColor::red);
	if (isRecording){ofDrawCircle(728,35,7);}  
	//ofDrawBitmapString(keysPressed,750,80);}       <---- this was only used to see what keys where being pressed (if it was recording all of them properly)
	ofSetColor(ofColor::white);
	if (isReplaying){
		ofDrawBitmapString("Key pressed: " + currentKey,750,100);  //Shows what key is being replayed
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if( key == '1'){
		
		if (isRecording==true){         //if it is recording then add the key pressed to the vector
		keysPressed.push_back('1');
		}
		if (isReplaying){               //if it is replaying and lock is false it means the current mode can be accessed and replayed (since it is not the user physically typing the key)
			if (lock==false){
			currentMode = PARTICLE_MODE_ATTRACT;
			currentModeStr = "1 - PARTICLE_MODE_ATTRACT: attracts to mouse";
			currentKey = "1";           // tells the key we're at so that it can be shown on screen
			}
		}else{                          // if it not replaying then it means the user is trying to type the key manually (intentionally and outside of when the program is replaying)
		currentMode = PARTICLE_MODE_ATTRACT;
		currentModeStr = "1 - PARTICLE_MODE_ATTRACT: attracts to mouse";
		}
	}


	if( key == '2'){
		
		if (isRecording==true){
		keysPressed.push_back('2');
		}
		
		if (isReplaying){
			if (lock==false){
			currentMode = PARTICLE_MODE_REPEL;
			currentModeStr = "2 - PARTICLE_MODE_REPEL: repels from mouse";
			currentKey = "2";
			}  				
		}else{
			currentMode = PARTICLE_MODE_REPEL;
			currentModeStr = "2 - PARTICLE_MODE_REPEL: repels from mouse";
		}
		
	}

	if( key == '3'){
		
		if (isRecording==true){
		keysPressed.push_back('3');
		}
		
		if (isReplaying){
			if (lock==false){
			currentMode = PARTICLE_MODE_NEAREST_POINTS;
			currentModeStr = "3 - PARTICLE_MODE_NEAREST_POINTS: hold 'f' to disable force";
			currentKey = "3";
			}
		}else{
		 	currentMode = PARTICLE_MODE_NEAREST_POINTS;
		 	currentModeStr = "3 - PARTICLE_MODE_NEAREST_POINTS: hold 'f' to disable force";
		}
					
	}


	if( key == '4'){
		
		if (isRecording==true){
		keysPressed.push_back('4');
		}

		if (isReplaying){
			if (lock==false){
			currentMode = PARTICLE_MODE_NOISE;
			currentModeStr = "4 - PARTICLE_MODE_NOISE: snow particle simulation";
			currentKey = "4"; 						
			resetParticles();
			}
		}else{
			currentMode = PARTICLE_MODE_NOISE;
			currentModeStr = "4 - PARTICLE_MODE_NOISE: snow particle simulation"; 						
			resetParticles();
		}
	}
	


	if( key == ' ' ){
		
		if (isRecording==true){
		keysPressed.push_back(' ');
		}
		
		if (isReplaying){
			if (lock==false){
			currentKey = "SPACE";
			resetParticles();
			}
		}else{
			resetParticles();
		}
	}
	


	if (key == 't' || key == 'T') {
		
		if (isRecording==true){
		keysPressed.push_back('t');
		}
		
		if (isReplaying){
			if (lock==false){
			currentKey = "T";
			for(unsigned int i = 0; i < p.size(); i++){
				p[i].setFlag(true);         //turns each particle to red by setting the flag as true
			}
			}
		}else{
			for(unsigned int i = 0; i < p.size(); i++){
				p[i].setFlag(true);
			}
		}
    }


	if (key == 's' || key == 'S'){
		
		if (isRecording==true){
		keysPressed.push_back('s');
		}
		
		if(isReplaying){
			if (lock==false){
			currentKey = "S";
			for(unsigned int i = 0; i < p.size(); i++){
				if (p[i].getPause()==false){           //check if particles are not paused
					p[i].setPause(true);               //if they are not paused then pause them by pressing key 's'
					
				}else{
					p[i].setPause(false);              // if particles are paused then unpause (or resume) them by pressing key 's'
					
				}
			}
		}
		
		}else{
			for(unsigned int i = 0; i < p.size(); i++){
				if (p[i].getPause()==false){           //check if particles are not paused
					p[i].setPause(true);               //if they are not paused then pause them by pressing key 's'
					
				}else{
					p[i].setPause(false);              // if particles are paused then unpause (or resume) them by pressing key 's'
					
				}
			}
		}
	}


	if (key == 'd' || key == 'D'){
		
		if (isRecording==true){
		keysPressed.push_back('d');
		}
		
		
		if (isReplaying){
			if (lock==false){
			currentKey = "D";
			factor = factor * 2;   //keeps updating the factor
			}
		}else{
			factor = factor * 2;   //keeps updating the factor
		}   
	}
		
		
	if (key == 'a' || key == 'A'){
		
		if (isRecording==true){
		keysPressed.push_back('a');
		}
		
		if (isReplaying){
			if (lock==false){
			currentKey = "A";
			factor = factor * 0.5;  //keeps updating the factor
			}
		}else{
			factor = factor * 0.5;  //keeps updating the factor
		}	
	}


	if (key == 'r' || key == 'R'){
		if (isRecording==false){        //checks if recording is on or off
			setIsRecording(true);      // if it is off, it turns recording on
			recordStateStr = "Recording !! \n";		
		}
		else{
			setIsRecording(false);    // if it is on then it turns it off
			recordStateStr = " ";
		}    	
	}


	if (key == 'p' || key == 'P'){
		setIsReplaying(true);        //sets replay state to true ! it replays !!
		recordStateStr = "Replaying !!  ";
	}
	
	if (key == 'c'||key =='C'){
		setIsReplaying(false);	//sets replay state to false and cancels the replay
		recordStateStr = " ";
		keysPressed.clear(); //clears the vector for the next time it records
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
	if (button == OF_MOUSE_BUTTON_LEFT){
		//gets the x and y coordinates of the mouse and assigns them to the second set of coordinates for the rectangle
		endx = x; 
		endy = y;

		rectx=(min(startx, endx));
		recty=(min(starty, endy));
		width=(abs(endx-startx));
		height=(abs(endy - starty));

		MouseDragging = false; //changes the variable to false in order to Always reset the rectangle everytime you drag again 
		IsRectangleDrawn = true; //changes the variable to true so it actually draws it and deletes it when you right click

		//setting all the variables for rectangle
		rectangle.setRectangleDrawn(IsRectangleDrawn);
		rectangle.setRectX(rectx);
		rectangle.setRectY(recty);
		rectangle.setWidth(width);
		rectangle.setHeight(height);

	}
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	if (button == OF_MOUSE_BUTTON_LEFT){
		MouseDragging = true;
		//gets the x and y coordinates of the mouse and assigns them to the first set of coordinates for the rectangle
		startx = x; 
		starty = y;
	}
	if (button == OF_MOUSE_BUTTON_RIGHT){
		//makes sure the mouse is inside the rectangle before erasing the the rectangle
		if (IsRectangleDrawn && x>=rectx && x <= rectx + width && y >= recty && y <= recty + height){
			IsRectangleDrawn = false;
			rectangle.setRectangleDrawn(IsRectangleDrawn);
			}
	}
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
