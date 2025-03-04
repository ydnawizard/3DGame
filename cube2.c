#include <stdio.h>
#include <stdlib.h>
#include <SDL3/SDL.h>
#include <unistd.h>
#include <math.h>
#include <string.h>
#include "homtrig.h"

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
	struct CUBE_3D cube2={
		{720.0,360.0,200.0,1},
		1,
		8,
		{{-100.0f,-100.0f,-100.0f,0},
		{-100.0f,100.0f,-100.0f,0},
		{100.0f,-100.0f,-100.0f,0},
		{100.0f,100.0f,-100.0f,0},
		{-100.0f,-100.0f,100.0f,1},
		{-100.0f,100.0f,100.0f,1},
		{100.0f,-100.0f,100.0f,1},
		{100.0f,100.0f,100.0f,1}},
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
					cubeTransform3d(&cube2,"scale",'+',fov);
					cube2.position[3]-=10;
					break;
				}else if(keyboardState[SDL_SCANCODE_S]){
					cubeTransform3d(&cube2,"scale",'-',fov);
					cube2.position[4]+=10;
					break;
				}else if(keyboardState[SDL_SCANCODE_R]){
					cubeTransform3d(&cube2,"rotate",'x',fov);
					break;
				}else if(keyboardState[SDL_SCANCODE_A]){
					cube2.position[0]-=10;
					break;
				}else if(keyboardState[SDL_SCANCODE_D]){
					cube2.position[0]+=10;
				}

			}
		}
		SDL_SetRenderDrawColor(renderer,0,0,0,255);
		SDL_RenderClear(renderer);
		SDL_SetRenderDrawColor(renderer,180,0,0,255);
		renderShape(cube2.vertices,cube2.position,"cube");
		//renderShape(cube1.vertices,cube1.position,cube1.type)
		cubeTransform3d(&cube2,"rotate",'y',fov);
		cubeTransform3d(&cube2,"rotate",'z',fov);
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

