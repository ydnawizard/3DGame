#ifndef LINALG_H
#define LINALG_H

//Every object on the screen is a SHAPE struct
//Example parent is MAP struct
struct SHAPE{
	float position[4];
	float perspectivePos[4];
	int id;
	int type;
	int verticesCount;
	float** vertices;
	float** perspectiveVerts;
	float color[4];
};

//The screen is a rendering of a MAP struct which contains the graphical gamestate data
struct MAP{
	struct SHAPE** shapes;
	struct SHAPE** toRender;
	struct SHAPE** render;
	int shapesCount;
	int renderCount;
	int id;
};

struct PLAYER{
	float position[4];
	int id;
};

//Multiplies input 4d vector by input 4x4 matrix
void matMult3d(float vector[3],float transform[4][4]){
	float vectorBUFF[4];
	vectorBUFF[0]=vector[0]*transform[0][0]+vector[1]*transform[0][1]+vector[2]*transform[0][2]+vector[3]*transform[0][3];
	vectorBUFF[1]=vector[0]*transform[1][0]+vector[1]*transform[1][1]+vector[2]*transform[1][2]+vector[3]*transform[1][3];
	vectorBUFF[2]=vector[0]*transform[2][0]+vector[1]*transform[2][1]+vector[2]*transform[2][2]+vector[3]*transform[2][3];
	vectorBUFF[3]=vector[0]*transform[3][0]+vector[1]*transform[3][1]+vector[2]*transform[2][3]+vector[3]*transform[3][3];
	if(vectorBUFF[3]!=0.000f){
		vectorBUFF[0]/=vectorBUFF[3];
		vectorBUFF[1]/=vectorBUFF[3];
		vectorBUFF[2]/=vectorBUFF[3];
	}
	vector[0]=vectorBUFF[0];
	vector[1]=vectorBUFF[1];
	vector[2]=vectorBUFF[2];
	vector[3]=vectorBUFF[3];
}

//GENERATE CUBE: Generates a cube of specified dimensions at specified position with specified id
//Stores cube in SHAPE struct
struct SHAPE* generateCube(float position[4],float sideLength,float color[4]){
	struct SHAPE* cube=(struct SHAPE*)malloc(sizeof(struct SHAPE));
	cube->vertices=(float**)malloc(8*sizeof(float*));
	for(int i=0;i<8;i++){
		cube->vertices[i]=(float*)malloc(4*sizeof(float));
	}
	cube->vertices[0][0]=-sideLength/2,cube->vertices[0][1]=-sideLength/2,cube->vertices[0][2]=-sideLength/2,cube->vertices[0][3]=0; //(L)eft (B)ottom (F)ront
	cube->vertices[1][0]=-sideLength/2,cube->vertices[1][1]=sideLength/2,cube->vertices[1][2]=-sideLength/2,cube->vertices[1][3]=0; //LTF
	cube->vertices[2][0]=sideLength/2,cube->vertices[2][1]=-sideLength/2,cube->vertices[2][2]=-sideLength/2,cube->vertices[2][3]=0; //RBF
	cube->vertices[3][0]=sideLength/2,cube->vertices[3][1]=sideLength/2,cube->vertices[3][2]=-sideLength/2,cube->vertices[3][3]=0; //RTF
	cube->vertices[4][0]=-sideLength/2,cube->vertices[4][1]=-sideLength/2,cube->vertices[4][2]=sideLength/2,cube->vertices[4][3]=0; //LBB
	cube->vertices[5][0]=-sideLength/2,cube->vertices[5][1]=sideLength/2,cube->vertices[5][2]=sideLength/2,cube->vertices[5][3]=0; //LTB
	cube->vertices[6][0]=sideLength/2,cube->vertices[6][1]=-sideLength/2,cube->vertices[6][2]=sideLength/2,cube->vertices[6][3]=0; //RBB
	cube->vertices[7][0]=sideLength/2,cube->vertices[7][1]=sideLength/2,cube->vertices[7][2]=sideLength/2,cube->vertices[7][3]=0; //RTB
	for(int i=0;i<4;i++){
		cube->position[i]=position[i];
	}
	cube->id=1;
	cube->type=1;
	cube->verticesCount=8;
	for(int i=0;i<4;i++){
		cube->color[i]=color[i];
	}
	return cube;
}


//Imposes a perspective upon a SHAPE, Perspective relevant data is passed in via vals array
//The function uses a perspective projection matrix and a scaling matrix that is constructed in 
//relation to the z coordinate of the shapes position (scales by a factor of 1+(100/z) 
void perspectiveProjectShape(struct SHAPE* shape,struct PLAYER* player,float vals[6]){
	float perspective[4][4]={
		{1/-tanf(vals[3]/2),0,0,0},
		{0,1/-tanf(vals[3]/2),0,0},
		{0,0,1,0},
		{0,0,-1,0}
	};
	/*float perspective[4][4]={
		{vals[2]/tanf(vals[3]/2),0,0,0},
		{0,1/tanf(vals[3]/2),0,0},
		{0,0,vals[5]/(vals[5]-vals[4]),-(vals[5]/(vals[5]-vals[4]))*vals[5]},
		{0,0,1,0}
	};*/
	/*float perspective[4][4]={
		{.5/tanf(vals[3]/2),0,0,0},
		{0,1/tanf(vals[3]/2),0,0},
		{0,0,vals[5]/(vals[5]-vals[4]),1},
		{0,0,(-vals[5]*vals[4])/(vals[5]-vals[4]),0}
	};*/
	float xDiff=shape->position[0]-player->position[0];
	float zDiff=shape->position[2]-player->position[2];
	float depthFactor=1-(.0001*sqrt((xDiff*xDiff)+(zDiff*zDiff)));
	float depth[4][4]={
		{depthFactor,0,0,0},
		{0,depthFactor,0,0},
		{0,0,depthFactor,0},
		{0,0,0,1}
	};
	for(int i=0;i<4;i++){
		shape->perspectivePos[i]=shape->position[i];
	}
	shape->perspectiveVerts=(float**)malloc(shape->verticesCount*sizeof(float*));
	for(int i=0;i<shape->verticesCount;i++){
		shape->perspectiveVerts[i]=(float*)malloc(4*sizeof(float));
		for(int j=0;j<4;j++){
			shape->perspectiveVerts[i][j]=shape->vertices[i][j];
		}
		matMult3d(shape->perspectiveVerts[i],depth);
		matMult3d(shape->perspectiveVerts[i],perspective);
	}
	//matMult3d(shape->perspectivePos,depth);
}

void perspectiveProjectMap(struct MAP* map,struct PLAYER* player,float vals[6]){
	for(int i=0;i<map->renderCount;i++){
		perspectiveProjectShape(map->toRender[i],player,vals);
	}
}


//CUBE TRANSFORM: Performs a specified transformation on a shape
//Takes in shape struct pointer,transformation type,aixs,and angle(theta)->Returns nothing
void transformShape(struct SHAPE* shape,char* transformType,char axis,float theta){
	if(strcmp(transformType,"rotate")==0&&axis=='y'){
		float transform[4][4]={
			{cosf(theta),0,sinf(theta),0},
			{0,1,0,0},
			{-sinf(theta),0,cosf(theta),0},
			{0,0,0,1}
		};
		for(int j=0;j<shape->verticesCount;j++){
			matMult3d(shape->vertices[j],transform);
		}
	}else if(strcmp(transformType,"rotate")==0&&axis=='x'){
		float transform[4][4]={
			{1,0,0,0},
			{0,cosf(theta),-sinf(theta),0},
			{0,sinf(theta),cosf(theta),0},
			{0,0,0,1}
		};
		for(int j=0;j<shape->verticesCount;j++){
			matMult3d(shape->vertices[j],transform);
		}
	}else if(strcmp(transformType,"rotate")==0&&axis=='z'){
		float transform[4][4]={
			{cosf(theta),-sinf(theta),0,0},
			{sinf(theta),cosf(theta),0,0},
			{0,0,1,0},
			{0,0,0,1}
		};
		for(int j=0;j<shape->verticesCount;j++){
			matMult3d(shape->vertices[j],transform);
		}
	}else if(strcmp(transformType,"scale")==0&&axis=='+'){
		float transform[4][4]={
			{1.1,0,0,0},
			{0,1.1,0,0},
			{0,0,1.1,0},
			{0,0,0,1}
		};
		for(int j=0;j<shape->verticesCount;j++){
			matMult3d(shape->vertices[j],transform);
		}
	}else if(strcmp(transformType,"scale")==0&&axis=='-'){
		float transform[4][4]={
			{.9,0,0,0},
			{0,.9,0,0},
			{0,0,.9,0},
			{0,0,0,1}
		};
		for(int j=0;j<shape->verticesCount;j++){
			matMult3d(shape->vertices[j],transform);
		}
	}
}

//TODO: THIS NEEDS TO ROTATE AROUND LOCAL COORDINATES, NOT WINDOW COORDINATES!
void transformPosition(struct SHAPE* shape,struct PLAYER* player,char* transformType,char axis,float angle,float vars[4]){
	float vectorBUFF[4];
	if(strcmp(transformType,"rotate")==0&&axis=='y'){
		float transform[4][4]={
			{cosf(angle),0,sinf(angle),0},
			{0,1,0,0},
			{-sinf(angle),0,cosf(angle),0},
			{0,0,0,1}
		};
		for(int i=0;i<4;i++){
			if(i!=1){
				vectorBUFF[i]=shape->position[i]-player->position[i];
			}else{
				vectorBUFF[i]=shape->position[i];
			}
		}
		matMult3d(vectorBUFF,transform);
		for(int i=0;i<4;i++){
			if(i!=1){
				shape->position[i]=vectorBUFF[i]+player->position[i];
			}else{
				shape->position[i]=vectorBUFF[i];
			}
		}
	}
}

void rotateMap(struct MAP* map,struct PLAYER* player,char axis,float angle,float vals[4]){
	for(int i=0;i<map->shapesCount;i++){
		transformShape(map->shapes[i],"rotate",'y',angle);
		transformPosition(map->shapes[i],player,"rotate",'y',angle,vals);
	}
}

#endif // TRIG_H
