#ifndef RENDER_H
#define RENDER_H
void boxcast(struct MAP* map,float vals[6]){
	map->renderCount=0;
	for(int i=0;i<map->shapesCount;i++){
		if(map->shapes[i]->position[0]>=0&&map->shapes[i]->position[0]<=vals[0]){
			if(map->shapes[i]->position[1]>=0&&map->shapes[i]->position[1]<=vals[1]){
				if(map->shapes[i]->position[2]>=0&&map->shapes[i]->position[2]<=vals[5]){
					map->toRender=(struct SHAPE**)realloc(map->toRender,(i+1)*sizeof(struct SHAPE*));
					map->toRender[map->renderCount]=(struct SHAPE*)malloc(sizeof(struct SHAPE));
					map->toRender[map->renderCount]=map->shapes[i];
					map->renderCount+=1;
				}
			}
		}
	}
}


void renderCube(SDL_Renderer* renderer, struct SHAPE* cube){
	float normalizedCube[8][4];
	for(int i=0;i<8;i++){
		for(int j=0;j<4;j++){
			normalizedCube[i][j]=cube->position[j]-cube->perspectiveVerts[i][j];
		}
	}
	SDL_SetRenderDrawColor(renderer,cube->color[0],cube->color[1],cube->color[2],cube->color[3]);
	SDL_RenderLine(renderer,normalizedCube[0][0],normalizedCube[0][1],normalizedCube[1][0],normalizedCube[1][1]);
	SDL_RenderLine(renderer,normalizedCube[3][0],normalizedCube[3][1],normalizedCube[1][0],normalizedCube[1][1]);
	SDL_RenderLine(renderer,normalizedCube[2][0],normalizedCube[2][1],normalizedCube[3][0],normalizedCube[3][1]);
	SDL_RenderLine(renderer,normalizedCube[0][0],normalizedCube[0][1],normalizedCube[2][0],normalizedCube[2][1]);
	SDL_RenderLine(renderer,normalizedCube[4][0],normalizedCube[4][1],normalizedCube[5][0],normalizedCube[5][1]);
	SDL_RenderLine(renderer,normalizedCube[7][0],normalizedCube[7][1],normalizedCube[5][0],normalizedCube[5][1]);
	SDL_RenderLine(renderer,normalizedCube[6][0],normalizedCube[6][1],normalizedCube[7][0],normalizedCube[7][1]);
	SDL_RenderLine(renderer,normalizedCube[4][0],normalizedCube[4][1],normalizedCube[6][0],normalizedCube[6][1]);
	SDL_RenderLine(renderer,normalizedCube[4][0],normalizedCube[4][1],normalizedCube[0][0],normalizedCube[0][1]);
	SDL_RenderLine(renderer,normalizedCube[5][0],normalizedCube[5][1],normalizedCube[1][0],normalizedCube[1][1]);
	SDL_RenderLine(renderer,normalizedCube[6][0],normalizedCube[6][1],normalizedCube[2][0],normalizedCube[2][1]);
	SDL_RenderLine(renderer,normalizedCube[7][0],normalizedCube[7][1],normalizedCube[3][0],normalizedCube[3][1]);
}

void renderPlane(SDL_Renderer* renderer,struct SHAPE* plane,float color[4]){
	SDL_SetRenderDrawColor(renderer,color[0],color[1],color[2],color[4]);
	for(int i=0;i<plane->verticesCount;i+=2){
		SDL_RenderLine(renderer,plane->vertices[i][0]-720,plane->vertices[i][1],plane->vertices[i+1][0]-720,plane->vertices[i+1][1]+360);
	}
}


void renderMap(SDL_Renderer* renderer, struct MAP* map){
	for(int i=0;i<map->renderCount;i++){
		renderCube(renderer,map->toRender[i]);
	}
}

#endif // RENDER_H
