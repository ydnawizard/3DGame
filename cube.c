#include <stdio.h>
#include <stdlib.h>
#include <SDL3/SDL.h>
#include <unistd.h>
#include <math.h>
#include <string.h>

int windowWidth=1440;
int windowHeight=1440;
SDL_Window* window;
SDL_Renderer* renderer;
float tempVec[4];

struct cube{
	float vertices[8][4];
	float position[4];
	char* type;
	int vertsLength;
};
struct scene{
	int shapesLength;
	int floor[4];
	int ceiling[4];
};

struct camera{
	float position[4];
	float FOV;
};

//4x1 vector X 4x4 matrix multiplier
void vecTransform(float vec[3],float transform[4][4]){
	tempVec[0]=vec[0]*transform[0][0]+vec[1]*transform[0][1]+vec[2]*transform[0][2]+vec[3]*transform[0][3];
	tempVec[1]=vec[0]*transform[1][0]+vec[1]*transform[1][1]+vec[2]*transform[1][2]+vec[3]*transform[1][3];
	tempVec[2]=vec[0]*transform[2][0]+vec[1]*transform[2][1]+vec[2]*transform[2][2]+vec[3]*transform[2][3];
	tempVec[3]=vec[0]*transform[3][0]+vec[1]*transform[3][1]+vec[2]*transform[3][2]+vec[3]*transform[3][3];
}

//Multiplies each vertice vector in shape by transformation matrix 
void shapeTransform(float shape[8][4],float transform[4][4]){
	for(int i=0;i<8;i++){
		vecTransform(shape[i],transform);
		for(int j=0;j<4;j++){
			shape[i][j]=tempVec[j];
		}
	}
}

//Converts vertice coordinates from cartesian plane where all math is done to the sdl plane where theres no negatives
//uses the sdl renderer to draw the cubes on the screen
void renderShape(float shape[][4],float position[4],char* type){
	int size;
	if(strcmp(type,"cube")==0)
		size=8;	
	float shapeNorm[size][4];
	for(int i=0;i<size;i++){
		for(int j=0;j<4;j++){
			shapeNorm[i][j]=position[j]-shape[i][j];
		}
	}
	SDL_RenderLine(renderer,shapeNorm[0][0],shapeNorm[0][1],shapeNorm[1][0],shapeNorm[1][1]);
	SDL_RenderLine(renderer,shapeNorm[3][0],shapeNorm[3][1],shapeNorm[1][0],shapeNorm[1][1]);
	SDL_RenderLine(renderer,shapeNorm[2][0],shapeNorm[2][1],shapeNorm[3][0],shapeNorm[3][1]);
	SDL_RenderLine(renderer,shapeNorm[0][0],shapeNorm[0][1],shapeNorm[2][0],shapeNorm[2][1]);
	SDL_RenderLine(renderer,shapeNorm[4][0],shapeNorm[4][1],shapeNorm[5][0],shapeNorm[5][1]);
	SDL_RenderLine(renderer,shapeNorm[7][0],shapeNorm[7][1],shapeNorm[5][0],shapeNorm[5][1]);
	SDL_RenderLine(renderer,shapeNorm[6][0],shapeNorm[6][1],shapeNorm[7][0],shapeNorm[7][1]);
	SDL_RenderLine(renderer,shapeNorm[4][0],shapeNorm[4][1],shapeNorm[6][0],shapeNorm[6][1]);
	SDL_RenderLine(renderer,shapeNorm[4][0],shapeNorm[4][1],shapeNorm[0][0],shapeNorm[0][1]);
	SDL_RenderLine(renderer,shapeNorm[5][0],shapeNorm[5][1],shapeNorm[1][0],shapeNorm[1][1]);
	SDL_RenderLine(renderer,shapeNorm[6][0],shapeNorm[6][1],shapeNorm[2][0],shapeNorm[2][1]);
	SDL_RenderLine(renderer,shapeNorm[7][0],shapeNorm[7][1],shapeNorm[3][0],shapeNorm[3][1]);
}

void perspectiveScaleUp(float shape[][4],float position[4], struct camera player){
	for(int i=0;i<2;i++){
		float temp[8][4];
		float scaleFactor=1/position[2];
		float scale[4][4]={
			{1+scaleFactor,0,0,0},
			{0,1+scaleFactor,0,0},
			{0,0,1+scaleFactor,0},
			{0,0,0,1+scaleFactor},
		};
		//for(int j=0;j<8;j++){
		//	shape[j][2]+=100;
		//}
		shapeTransform(shape,scale);
	}
}

void perspectiveScaleDown(float shape[][4],float position[4], struct camera player){
	for(int i=0;i<2;i++){
		float temp[8][4];
		float scaleFactor=1/position[2];
		float scale[4][4]={
			{1-scaleFactor,0,0,0},
			{0,1-scaleFactor,0,0},
			{0,0,1-scaleFactor,0},
			{0,0,0,1-scaleFactor},
		};
		//for(int j=0;j<8;j++){
		//	shape[j][2]+=100;
		//}
		shapeTransform(shape,scale);
	}
}

void perspectiveShiftLeft(float shape[][4],float position[4],struct camera player){
	float scaleFactor=1.0f+(1/position[2]);
	position[0]-=(10*scaleFactor);
	for(int i;i<8;i++){
		shape[i][0]-=(10*scaleFactor);
	}
}

void perspectiveShiftRight(float shape[][4],float position[4],struct camera player){
	float scaleFactor=1.0f+(1/position[2]);
	position[0]+=(10*scaleFactor);
	for(int i;i<8;i++){
		shape[i][0]-=(10*scaleFactor);
	}
}
int main(){
	struct cube cube1={{
		{-100.0f,-100.0f,100.0f,1},
		{-100.0f,100.0f,100.0f,1},
		{100.0f,-100.0f,100.0f,1},
		{100.0f,100.0f,100.0f,1},
		{-100.0f,-100.0f,-100.0,1},
		{-100.0f,100.0f,-100.0f,1},
		{100.0f,-100.0f,-100.0f,1},
		{100.0f,100.0f,-100.0f,1}},
		{360.0,720.0,100.0f,1},"cube",8};
	struct cube cube2={{
		{-100.0f,-100.0f,100.0f,1},
		{-100.0f,100.0f,100.0f,1},
		{100.0f,-100.0f,100.0f,1},
		{100.0f,100.0f,100.0f,1},
		{-100.0f,-100.0f,-100.0f,1},
		{-100.0f,100.0f,-100.0f,1},
		{100.0f,-100.0f,-100.0f,1},
		{100.0f,100.0f,-100.0f,1}},
		{900.0,720.0,200,1},"cube",8};
	struct cube shapes[2]={cube1,cube2};
	struct scene frame={
		2,
		(255,255,255,255),
		(0,0,0,255),
	};
	struct camera player={{720,720,0,1},(windowWidth/windowHeight)};
	float theta=.01745;
	float yrotate[4][4]={
		{cosf(theta),0,sinf(theta),0},
		{0,1,0,0},
		{-sinf(theta),0,cosf(theta),0},
		{0,0,0,1}
	};
	float xrotate[4][4]={
		{1,0,0,0},
		{0,cosf(theta),-sinf(theta),0},
		{0,sinf(theta),cosf(theta),0},
		{0,0,0,1}
	};
	float zrotate[4][4]={
		{cosf(theta),-sinf(theta),0,0},
		{sinf(theta),cosf(theta),0,0},
		{0,0,1,0},
		{0,0,0,1}
	};
	float scaleUp[4][4]={
		{1.01,0,0,0},
		{0,1.01,0,0},
		{0,0,1.01,0},
		{0,0,0,1},
	};
	float scaleDown[4][4]={
		{0.99,0,0,0},
		{0,0.99,0,0},
		{0,0,0.99,0},
		{0,0,0,1}
	};
	SDL_Event e;
	SDL_Init(SDL_INIT_VIDEO);
	bool running=true;
	SDL_CreateWindowAndRenderer("window",windowWidth,windowHeight,0,&window,&renderer);
	while(running){
		while(SDL_PollEvent(&e)){
			if(e.type==SDL_EVENT_QUIT)
				running=false;
			else if(e.type==SDL_EVENT_KEY_DOWN){
				const bool* keyboardState=SDL_GetKeyboardState(NULL);
				if(keyboardState[SDL_SCANCODE_W]){
					cube1.position[2]+=1;
					cube2.position[2]+=1;
					perspectiveScaleUp(cube1.vertices,cube1.position,player);
					perspectiveScaleUp(cube2.vertices,cube2.position,player);
					break;
				}else if(keyboardState[SDL_SCANCODE_S]){
					cube1.position[2]-=1;
					cube2.position[2]-=1;
					perspectiveScaleDown(cube1.vertices,cube1.position,player);
					perspectiveScaleDown(cube2.vertices,cube2.position,player);
					break;
				}else if(keyboardState[SDL_SCANCODE_A]){
					perspectiveShiftLeft(cube1.vertices,cube1.position,player);
					perspectiveShiftLeft(cube2.vertices,cube2.position,player);
				}else if(keyboardState[SDL_SCANCODE_D]){
					perspectiveShiftRight(cube1.vertices,cube1.position,player);
					perspectiveShiftRight(cube2.vertices,cube2.position,player);
				}

			}
		}
		SDL_SetRenderDrawColor(renderer,0,0,0,255);
		SDL_RenderClear(renderer);
		SDL_SetRenderDrawColor(renderer,130,0,0,255);
		//renderShape(cube1.vertices,cube1.position,cube1.type);

		renderShape(cube2.vertices,cube2.position,cube2.type);
		renderShape(cube1.vertices,cube1.position,cube1.type);
		SDL_RenderPresent(renderer);
		shapeTransform(cube1.vertices,yrotate);
		shapeTransform(cube1.vertices,xrotate);
		shapeTransform(cube1.vertices,zrotate);
		shapeTransform(cube2.vertices,yrotate);
		shapeTransform(cube2.vertices,xrotate);
		shapeTransform(cube2.vertices,zrotate);
		usleep(10000);
	}
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;

}

