#include <stdio.h>
#include <stdlib.h>
#include <SDL3/SDL.h>
#include <unistd.h>
#include <math.h>

int windowWidth=720;
int windowHeight=720;
float tempVec[3];

struct cube{
	float lbf[3];
	float ltf[3];
	float rbf[3];
	float rtf[3];
	float lbb[3];
	float ltb[3];
	float rbb[3];
	float rtb[3];
};

void transform2D(float vec[3],float transform[3][3]){
	tempVec[0]=vec[0]*transform[0][0]+vec[1]*transform[0][1]+vec[2]*transform[0][2];
	tempVec[1]=vec[0]*transform[1][0]+vec[1]*transform[1][1]+vec[2]*transform[1][2];
	tempVec[2]=vec[0]*transform[2][0]+vec[1]*transform[2][1]+vec[2]*transform[2][2];
}

int main(){
	char vertices[8][3]={"lbf","ltf","rbf","rtf","lbb","ltb","rbb","rtb"};
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
	struct cube cube1={
		{-100.0f,-100.0f,100.0f},
		{-100.0f,100.0f,100.0f},
		{100.0f,-100.0f,100.0f},
		{100.0f,100.0f,100.0f},
		{-100.0f,-100.0f,-100.0f},
		{-100.0f,100.0f,-100.0f},
		{100.0f,-100.0f,-100.0f},
		{100.0f,100.0f,-100.0f},
	};
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Event e;
	SDL_Init(SDL_INIT_VIDEO);
	bool running=true;
	SDL_CreateWindowAndRenderer("window",windowWidth,windowHeight,0,&window,&renderer);
	while(running){
		while(SDL_PollEvent(&e)){
			if(e.type==SDL_EVENT_QUIT)
				running=false;
			else if(e.type==SDL_EVENT_KEY_DOWN){
				if(e.type==SDL_SCANCODE_W){
					transform2D(cube1.lbf,yRotate2d);
					break;
				}
			}
		}
		struct cube cube1Norm={
			{360.0f-cube1.lbf[0],360.0f-cube1.lbf[1],360.0f-cube1.lbf[2]},
			{360.0f-cube1.ltf[0],360.0f-cube1.ltf[1],360.0f-cube1.ltf[2]},
			{360.0f-cube1.rbf[0],360.0f-cube1.rbf[1],360.0f-cube1.rbf[2]},
			{360.0f-cube1.rtf[0],360.0f-cube1.rtf[1],360.0f-cube1.rtf[2]},
			{360.0f-cube1.lbb[0],360.0f-cube1.lbb[1],360.0f-cube1.lbb[2]},
			{360.0f-cube1.ltb[0],360.0f-cube1.ltb[1],360.0f-cube1.ltb[2]},
			{360.0f-cube1.rbb[0],360.0f-cube1.rbb[1],360.0f-cube1.rbb[2]},
			{360.0f-cube1.rtb[0],360.0f-cube1.rtb[1],360.0f-cube1.rtb[2]},
		};
		SDL_SetRenderDrawColor(renderer,0,0,0,255);
		SDL_RenderClear(renderer);
		SDL_SetRenderDrawColor(renderer,130,0,0,255);
		//FRONT
		SDL_RenderLine(renderer,cube1Norm.lbf[0],cube1Norm.lbf[1],cube1Norm.ltf[0],cube1Norm.ltf[1]);
		SDL_RenderLine(renderer,cube1Norm.rtf[0],cube1Norm.rtf[1],cube1Norm.ltf[0],cube1Norm.ltf[1]);
		SDL_RenderLine(renderer,cube1Norm.rbf[0],cube1Norm.rbf[1],cube1Norm.rtf[0],cube1Norm.rtf[1]);
		SDL_RenderLine(renderer,cube1Norm.lbf[0],cube1Norm.lbf[1],cube1Norm.rbf[0],cube1Norm.rbf[1]);
		SDL_RenderLine(renderer,cube1Norm.lbb[0],cube1Norm.lbb[1],cube1Norm.ltb[0],cube1Norm.ltb[1]);
		SDL_RenderLine(renderer,cube1Norm.rtb[0],cube1Norm.rtb[1],cube1Norm.ltb[0],cube1Norm.ltb[1]);
		SDL_RenderLine(renderer,cube1Norm.rbb[0],cube1Norm.rbb[1],cube1Norm.rtb[0],cube1Norm.rtb[1]);
		SDL_RenderLine(renderer,cube1Norm.lbb[0],cube1Norm.lbb[1],cube1Norm.rbb[0],cube1Norm.rbb[1]);
		SDL_RenderLine(renderer,cube1Norm.lbb[0],cube1Norm.lbb[1],cube1Norm.lbf[0],cube1Norm.lbf[1]);
		SDL_RenderLine(renderer,cube1Norm.ltb[0],cube1Norm.ltb[1],cube1Norm.ltf[0],cube1Norm.ltf[1]);
		SDL_RenderLine(renderer,cube1Norm.rbb[0],cube1Norm.rbb[1],cube1Norm.rbf[0],cube1Norm.rbf[1]);
		SDL_RenderLine(renderer,cube1Norm.rtb[0],cube1Norm.rtb[1],cube1Norm.rtf[0],cube1Norm.rtf[1]);
		SDL_RenderPresent(renderer);
		transform2D(cube1.lbf,yRotate2d);
		for(int i=0;i<3;i++){
			cube1.lbf[i]=tempVec[i];
		}
		transform2D(cube1.lbf,xRotate2d);
		for(int i=0;i<3;i++){
			cube1.lbf[i]=tempVec[i];
		}
		transform2D(cube1.ltf,yRotate2d);
		for(int i=0;i<3;i++){
			cube1.ltf[i]=tempVec[i];
		}
		transform2D(cube1.ltf,xRotate2d);
		for(int i=0;i<3;i++){
			cube1.ltf[i]=tempVec[i];
		}
		transform2D(cube1.rbf,yRotate2d);
		for(int i=0;i<3;i++){
			cube1.rbf[i]=tempVec[i];
		}
		transform2D(cube1.rbf,xRotate2d);
		for(int i=0;i<3;i++){
			cube1.rbf[i]=tempVec[i];
		}
		transform2D(cube1.rtf,yRotate2d);
		for(int i=0;i<3;i++){
			cube1.rtf[i]=tempVec[i];
		}
		transform2D(cube1.rtf,xRotate2d);
		for(int i=0;i<3;i++){
			cube1.rtf[i]=tempVec[i];
		}
		transform2D(cube1.lbb,yRotate2d);
		for(int i=0;i<3;i++){
			cube1.lbb[i]=tempVec[i];
		}
		transform2D(cube1.lbb,xRotate2d);
		for(int i=0;i<3;i++){
			cube1.lbb[i]=tempVec[i];
		}
		transform2D(cube1.ltb,yRotate2d);
		for(int i=0;i<3;i++){
			cube1.ltb[i]=tempVec[i];
		}
		transform2D(cube1.ltb,xRotate2d);
		for(int i=0;i<3;i++){
			cube1.ltb[i]=tempVec[i];
		}
		transform2D(cube1.rtb,yRotate2d);
		for(int i=0;i<3;i++){
			cube1.rtb[i]=tempVec[i];
		}
		transform2D(cube1.rtb,xRotate2d);
		for(int i=0;i<3;i++){
			cube1.rtb[i]=tempVec[i];
		}
		transform2D(cube1.rbb,yRotate2d);
		for(int i=0;i<3;i++){
			cube1.rbb[i]=tempVec[i];
		}
		transform2D(cube1.rbb,xRotate2d);
		for(int i=0;i<3;i++){
			cube1.rbb[i]=tempVec[i];
		}
		usleep(7000);
	}
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;

}

