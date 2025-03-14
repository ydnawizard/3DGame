#include <stdio.h>
#include <stdlib.h>
#include <SDL3/SDL.h>
#include <unistd.h>
#include <math.h>
#include <string.h>

int windowWidth=1440;
int windowHeight=720;
float aspectRatio=1440/720;
float fov=.0174533*2;
float znear=0;
float zfar=2;
SDL_Window* window;
SDL_Renderer* renderer;
float scene[2];
float perspective[4][4];

struct cube{
	float vertices[8][4];
	float position[4];
	char* type;
	int vertsLength;
};


//4x1 vector X 4x4 matrix multiplier
void vecTransform(float vec[3],float transform[4][4]){
	float tempVec[4];
	tempVec[0]=vec[0]*transform[0][0]+vec[1]*transform[0][1]+vec[2]*transform[0][2]+vec[3]*transform[0][3];
	tempVec[1]=vec[0]*transform[1][0]+vec[1]*transform[1][1]+vec[2]*transform[1][2]+vec[3]*transform[1][3];
	tempVec[2]=vec[0]*transform[2][0]+vec[1]*transform[2][1]+vec[2]*transform[2][2]+vec[3]*transform[2][3];
	tempVec[3]=vec[0]*transform[3][0]+vec[1]*transform[3][1]+vec[2]*transform[3][2]+vec[3]*transform[3][3];
	if(tempVec[3]!=0.0f){
		tempVec[0]/=tempVec[3];
		tempVec[1]/=tempVec[3];
		tempVec[2]/=tempVec[3];
		tempVec[3]=vec[3];
	}
	vec[0]=tempVec[0];
	vec[1]=tempVec[1];
	vec[2]=tempVec[2];
	vec[3]=tempVec[3];
}

//Multiplies each vertice vector in shape by transformation matrix 
void shapeTransform(float shape[8][4],float transform[4][4]){
	float tempVec[4];
	for(int i=0;i<8;i++){
		vecTransform(shape[i],transform);
	}
}

//Converts vertice coordinates from cartesian plane where all math is done to the sdl plane where theres no negatives
//uses the sdl renderer to draw the cubes on the screen
void renderShape(float shape[][4],float position[4],char* type){
	/*float perspective[4][4]={
		{(aspectRatio/(tanf(fov/2.0000f))),0,0,0},
		{0,(1/tanf(fov/2)),0,0},
		{0,0,(zfar)/(zfar-znear),-(zfar*znear)/(zfar-znear)},
		{0,0,1,0}
	};*/
	int size;
	if(strcmp(type,"cube")==0)
		size=8;	
	/*float tempShape[size][4];
	memcpy(tempShape,shape,sizeof(float)*size*4);
	shapeTransform(tempShape,perspective);*/
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
		{0.0,360.0,360.0f,1},"cube",8};
	struct cube cube2={{
		{-100.0f,-100.0f,-100.0f,0},
		{-100.0f,100.0f,-100.0f,0},
		{100.0f,-100.0f,-100.0f,0},
		{100.0f,100.0f,-100.0f,0},
		{-100.0f,-100.0f,100.0f,1},
		{-100.0f,100.0f,100.0f,1},
		{100.0f,-100.0f,100.0f,1},
		{100.0f,100.0f,100.0f,1}},
		{720.0,360.0,200.0,1},"cube",8};
	struct cube shapes[2]={cube1,cube2};
	float yrotate[4][4]={
		{cosf(fov),0,sinf(fov),0},
		{0,1,0,0},
		{-sinf(fov),0,cosf(fov),0},
		{0,0,0,1}
	};
	float xrotate[4][4]={
		{1,0,0,0},
		{0,cosf(fov),-sinf(fov),0},
		{0,sinf(fov),cosf(fov),0},
		{0,0,0,1}
	};
	float zrotate[4][4]={
		{cosf(fov),-sinf(fov),0,0},
		{sinf(fov),cosf(fov),0,0},
		{0,0,1,0},
		{0,0,0,1}
	};
	float scaleDown[4][4]={
		{1.01,0,0,0},
		{0,1.01,0,0},
		{0,0,1.01,0},
		{0,0,0,1},
	};
	float scaleUp[4][4]={
		{.99,0,0,0},
		{0,.99,0,0},
		{0,0,.99,0},
		{0,0,0,1}
	};
	float camera=0.0000;
	SDL_Event e;
	SDL_Init(SDL_INIT_VIDEO);
	bool running=true;
	SDL_FRect rect={0,0,10000,720};
	SDL_CreateWindowAndRenderer("window",windowWidth,windowHeight,0,&window,&renderer);
	while(running){
		while(SDL_PollEvent(&e)){
			if(e.type==SDL_EVENT_QUIT)
				running=false;
			else if(e.type==SDL_EVENT_KEY_DOWN){
				const bool* keyboardState=SDL_GetKeyboardState(NULL);
				if(keyboardState[SDL_SCANCODE_W]){
					shapeTransform(cube1.vertices,scaleUp);
					shapeTransform(cube2.vertices,scaleUp);
					break;
				}else if(keyboardState[SDL_SCANCODE_S]){
				        shapeTransform(cube1.vertices,scaleDown);
					shapeTransform(cube2.vertices,scaleDown);
					break;
				}else if(keyboardState[SDL_SCANCODE_R]){
					shapeTransform(cube2.vertices,xrotate);
					break;
				}else if(keyboardState[SDL_SCANCODE_A]){
				}else if(keyboardState[SDL_SCANCODE_D]){
				}

			}
		}
		SDL_SetRenderDrawColor(renderer,0,0,0,255);
		SDL_RenderClear(renderer);
		SDL_SetRenderDrawColor(renderer,255,255,255,255);
		SDL_RenderFillRect(renderer,&rect);
		SDL_SetRenderDrawColor(renderer,180,0,0,255);
		renderShape(cube2.vertices,cube2.position,cube2.type);
		//renderShape(cube1.vertices,cube1.position,cube1.type)
		shapeTransform(cube2.vertices,yrotate);
		//shapeTransform(cube2.vertices,xrotate);
		//shapeTransform(cube2.vertices,zrotate);
		//renderShape(cube1.vertices,cube1.position,cube1.type);
		SDL_RenderPresent(renderer);

		usleep(15000);
	}
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;

}

