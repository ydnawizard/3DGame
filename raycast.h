#ifndef RAYCAST_H
#define RAYCAST_H

void raycast(struct MAP* map,float vals[6]){
	map->renderCount=0;
	for(int i=0;i<map->shapesCount;i++){
		if(map->shapes[i]->position[0]>0&&map->shapes[i]->position[0]<vals[0]){
			map->toRender=(struct SHAPE**)realloc(map->toRender,(i+1)*sizeof(struct SHAPE*));
			map->toRender[i]=(struct SHAPE*)malloc(sizeof(struct SHAPE));
			map->toRender[i]=map->shapes[i];
			map->renderCount+=1;
		}
	}
}

#endif
