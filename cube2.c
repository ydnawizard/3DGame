#include <stdio.h>
#include <stdlib.h>
#include <SDL3/SDL.h>
#include <unistd.h>
#include <math.h>
#include <string.h>
#include "linalg2.h"
#include "render.h"

float windowWidth=1440;
float windowHeight=720;
float aspectRatio=1440/720;
float fov=.0174533*90;
float theta=.0174533;
float znear=1;
float zfar=10000;
SDL_Window* window;
SDL_Renderer* renderer;
float scene[2];
float perspective[4][4];
struct SHAPE* cubes[6];
struct SHAPE* planes[3];
struct PLAYER player;
struct MAP map;

int main(){
	float white[4]={255,255,255,255};
	float red[4]={180,0,0,255};
	float blue[4]={0,0,255,255};
	float green[4]={0,255,0,255};
	float orange[4]={255,167,14,255};
	float pink[4]={255,0,255,255};
	float cyan[4]={0,255,247,255};
	float position1[4]={360.0,360.0,100,0};
	float position2[4]={1080.0,360.0,100.0,0};
	float position3[4]={360.0,360.0,5000.0,0};
	float position4[4]={1080.0,360.0,5000.0,0};
	float position5[4]={360.0,360.0,500.0,0};
	float position6[4]={1080.0,360.0,500.0,0};
	float perspectiveVals[6]={windowWidth,windowHeight,aspectRatio,fov,znear,zfar};
	cubes[0]=generateCube(position1,100,red);
	cubes[1]=generateCube(position2,100,green);
	cubes[2]=generateCube(position3,100,blue);
	cubes[3]=generateCube(position4,100,orange);
	cubes[4]=generateCube(position5,100,pink);
	cubes[5]=generateCube(position6,100,cyan);
	map.shapesCount=6;
	map.id=0;
	map.shapes=(struct SHAPE**)malloc(sizeof(struct SHAPE*));
	player.id=0;
	float playerPosition[4]={720.0f,360.0f,0.0f,0.0f};
	for(int i=0;i<4;i++){
		player.position[i]=playerPosition[i];
	}
	for(int i=0;i<6;i++){
		map.shapes[i]=(struct SHAPE*)malloc(sizeof(struct SHAPE));
		map.shapes[i]=cubes[i];
	}
	SDL_Init(SDL_INIT_VIDEO);
	bool running=true;
	SDL_CreateWindowAndRenderer("window",windowWidth,windowHeight,SDL_WINDOW_RESIZABLE,&window,&renderer);
	SDL_Event e;
	while(running){
		while(SDL_PollEvent(&e)){
			if(e.type==SDL_EVENT_QUIT)
				running=false;
			else if(e.type==SDL_EVENT_KEY_DOWN){
				const bool* keyboardState=SDL_GetKeyboardState(NULL);
				if(keyboardState[SDL_SCANCODE_W]){
					player.position[2]+=100;
					break;
				}else if(keyboardState[SDL_SCANCODE_S]){
					player.position[2]-=100;
					break;
				}else if(keyboardState[SDL_SCANCODE_R]){
					break;
				}else if(keyboardState[SDL_SCANCODE_A]){
					player.position[0]-=10;
					break;
				}else if(keyboardState[SDL_SCANCODE_D]){
					player.position[0]+=10;
					break;
				}

			}
			else if(e.type==SDL_EVENT_MOUSE_MOTION){
				if(e.motion.xrel>0){
					rotateMap(&map,&player,'y',-theta/10,perspectiveVals);
				}
				if(e.motion.xrel<0){
					rotateMap(&map,&player,'y',theta/10,perspectiveVals);
				}
			}
		}
		SDL_SetRenderDrawColor(renderer,0,0,0,255);
		SDL_RenderClear(renderer);
		transformShape(cubes[2],"rotate",'y',theta);
		transformShape(cubes[2],"rotate",'z',theta);
		transformShape(cubes[2],"rotate",'x',theta);
		transformShape(cubes[0],"rotate",'y',theta);
		transformShape(cubes[0],"rotate",'z',theta);
		transformShape(cubes[0],"rotate",'x',theta);
		transformShape(cubes[1],"rotate",'y',theta);
		transformShape(cubes[1],"rotate",'z',theta);
		transformShape(cubes[1],"rotate",'x',theta);
		boxcast(&map,&player,perspectiveVals);
		perspectiveProjectMap(&map,&player,perspectiveVals);
		renderMap(renderer,&player,&map);
		SDL_RenderPresent(renderer);

		usleep(15000);
	}
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;

}

