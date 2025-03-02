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
float tempVec[3];

struct cube{
	float vertices[8][3];
	float center;
	char* type;
};

void vecTransform(float vec[3],float transform[3][3]){
	tempVec[0]=vec[0]*transform[0][0]+vec[1]*transform[0][1]+vec[2]*transform[0][2];
	tempVec[1]=vec[0]*transform[1][0]+vec[1]*transform[1][1]+vec[2]*transform[1][2];
	tempVec[2]=vec[0]*transform[2][0]+vec[1]*transform[2][1]+vec[2]*transform[2][2];
}

void shapeTransform(float shape[8][3],float transform[3][3]){
	for(int i=0;i<8;i++){
		vecTransform(shape[i],transform);
		for(int j=0;j<3;j++){
			shape[i][j]=tempVec[j];
		}
	}
}

void renderShape(float shape[][3],float center,char* type){
	float shapeNorm[8][3];
	int size;
	if(strcmp(type,"cube")==0)
		size=8;
	for(int i=0;i<size;i++){
		for(int j=0;j<3;j++){
			shapeNorm[i][j]=center-shape[i][j];
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
		{-100.0f,-100.0f,100.0f},
		{-100.0f,100.0f,100.0f},
		{100.0f,-100.0f,100.0f},
		{100.0f,100.0f,100.0f},
		{-100.0f,-100.0f,-100.0f},
		{-100.0f,100.0f,-100.0f},
		{100.0f,-100.0f,-100.0f},
		{100.0f,100.0f,-100.0f}},
		360.0f,"cube"};
	struct cube cube2={{
		{-100.0f,-100.0f,100.0f},
		{-100.0f,100.0f,100.0f},
		{100.0f,-100.0f,100.0f},
		{100.0f,100.0f,100.0f},
		{-100.0f,-100.0f,-100.0f},
		{-100.0f,100.0f,-100.0f},
		{100.0f,-100.0f,-100.0f},
		{100.0f,100.0f,-100.0f}},
		720.0f,"cube"};
	float theta=.01745;
	float yRotate2d[3][3]={
		{cosf(theta),0,sinf(theta)},
		{0,1,0},
		{-sinf(theta),0,cosf(theta)}
	};
	float xRotate2d[3][3]={
		{1,0,0},
		{0,cosf(theta),-sinf(theta)},
		{0,sinf(theta),cosf(theta)},
	};
	float scaleUp[3][3]={
		{1.01,0,0},
		{0,1.01,0},
		{0,0,1.01},
	};
	float scaleDown[3][3]={
		{0.99,0,0},
		{0,0.99,0},
		{0,0,0.99},
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
					for(int i=0;i<8;i++){
						printf("%d",cube2.vertices[i][2]);
						cube2.vertices[i][2]+=1;
					}
					break;
				}else if(keyboardState[SDL_SCANCODE_S]){
					for(int i=0;i<8;i++){
						cube2.vertices[i][2]+=1;
					}
					break;
				}
			}
		}
		SDL_SetRenderDrawColor(renderer,0,0,0,255);
		SDL_RenderClear(renderer);
		SDL_SetRenderDrawColor(renderer,130,0,0,255);
		//renderShape(cube1.vertices,cube1.center,cube1.type);
		renderShape(cube2.vertices,cube2.center,cube2.type);
		SDL_RenderPresent(renderer);
		//shapeTransform(cube1.vertices,yRotate2d);
		//shapeTransform(cube1.vertices,xRotate2d);
		shapeTransform(cube2.vertices,yRotate2d);
		shapeTransform(cube2.vertices,xRotate2d);
		usleep(7000);
	}
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;

}

