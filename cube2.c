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
float fov=.0174533*200;
float theta=.0174533;
float znear=0;
float zfar=1000;
SDL_Window* window;
SDL_Renderer* renderer;
float scene[2];
float perspective[4][4];
struct SHAPE* cubes[3];
struct SHAPE* planes[3];

int main(){
	float white[4]={255,255,255,255};
	float red[4]={180,0,0,255};
	float blue[4]={0,0,255,255};
	float green[4]={0,255,0,255};
	float position1[4]={360.0,360.0,300,1};
	float position2[4]={720.0,360.0,500.0,1};
	float position3[4]={1080.0,360.0,700.0,1};
	float perspectiveVals[6]={windowWidth,windowHeight,aspectRatio,fov,znear,zfar};
	cubes[0]=generateCube(position1,10,red);
	cubes[1]=generateCube(position2,10,green);
	cubes[2]=generateCube(position3,10,blue);
	struct MAP map;
	map.shapesCount=3;
	map.id=0;
	map.shapes=(struct SHAPE**)malloc(sizeof(struct SHAPE*));
	SDL_Event e;
	SDL_Init(SDL_INIT_VIDEO);
	bool running=true;
	SDL_CreateWindowAndRenderer("window",windowWidth,windowHeight,SDL_WINDOW_RESIZABLE,&window,&renderer);
	for(int i=0;i<3;i++){
		map.shapes[i]=(struct SHAPE*)malloc(sizeof(struct SHAPE));
		map.shapes[i]=cubes[i];
		transformShape(cubes[i],"rotate",'x',5*theta);
		transformShape(cubes[i],"rotate",'y',5*theta);
	}
	while(running){
		while(SDL_PollEvent(&e)){
			if(e.type==SDL_EVENT_QUIT)
				running=false;
			else if(e.type==SDL_EVENT_KEY_DOWN){
				const bool* keyboardState=SDL_GetKeyboardState(NULL);
				if(keyboardState[SDL_SCANCODE_W]){
					transformShape(cubes[0],"scale",'+',theta);
					transformShape(cubes[1],"scale",'+',theta);
					transformShape(cubes[2],"scale",'+',theta);
					cubes[0]->position[2]-=10;
					cubes[1]->position[2]-=10;
					cubes[2]->position[2]-=10;
					break;
				}else if(keyboardState[SDL_SCANCODE_S]){
					transformShape(cubes[0],"scale",'-',theta);
					transformShape(cubes[1],"scale",'-',theta);
					transformShape(cubes[2],"scale",'-',theta);
					cubes[0]->position[2]+=10;
					cubes[1]->position[2]+=10;
					cubes[2]->position[2]+=10;
					break;
				}else if(keyboardState[SDL_SCANCODE_R]){
					break;
				}else if(keyboardState[SDL_SCANCODE_A]){
					transformShape(cubes[0],"rotate",'y',theta);
					transformPosition(cubes[0],"rotate",'y',theta,perspectiveVals);
					transformShape(cubes[1],"rotate",'y',theta);
					transformShape(cubes[2],"rotate",'y',theta);
					transformPosition(cubes[1],"rotate",'y',theta,perspectiveVals);
					transformPosition(cubes[2],"rotate",'y',theta,perspectiveVals);
					//cubes[0]->position[0]-=50;
					break;
				}else if(keyboardState[SDL_SCANCODE_D]){
					transformShape(cubes[0],"rotate",'y',-theta);
					transformPosition(cubes[0],"rotate",'y',-theta,perspectiveVals);
					transformShape(cubes[1],"rotate",'y',-theta);
					transformShape(cubes[2],"rotate",'y',-theta);
					transformPosition(cubes[1],"rotate",'y',-theta,perspectiveVals);
					transformPosition(cubes[2],"rotate",'y',-theta,perspectiveVals);
				}

			}
		}
		SDL_SetRenderDrawColor(renderer,0,0,0,255);
		SDL_RenderClear(renderer);
		/*transformShape(cubes[2],"rotate",'y',theta);
		transformShape(cubes[2],"rotate",'z',theta);
		transformShape(cubes[2],"rotate",'x',theta);
		transformShape(cubes[0],"rotate",'y',theta);
		transformShape(cubes[0],"rotate",'z',theta);
		transformShape(cubes[0],"rotate",'x',theta);
		transformShape(cubes[1],"rotate",'y',theta);
		transformShape(cubes[1],"rotate",'z',theta);
		transformShape(cubes[1],"rotate",'x',theta);*/
		boxcast(&map,perspectiveVals);
		perspectiveProjectMap(&map,perspectiveVals);
		renderMap(renderer,&map);
		SDL_RenderPresent(renderer);

		usleep(15000);
	}
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;

}

