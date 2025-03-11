#ifndef LINALG_H
#define LINALG_H

struct SQUARE{
	float position[3];
	int id;
	int type;
	int verticesCount;
	float vertices[8][3];
};
struct CUBE{
	float position[4];
	int id;
	int type;
	int verticesCount;
	float vertices[8][4];
};

struct TRIANGLE{
	float position[3];
	int id;
	int type;
	int verticesCount;
	float vertices[3][3];
};


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

//CUBE FUNCTIONS
//
//GENERATE CUBE: Generates a cube of specified dimensions at specified position with specified id
void generateCube(struct CUBE* cube,float position[4],int id,float sideLength){
	cube->vertices[0][0]=-sideLength/2,cube->vertices[0][1]=-sideLength/2,cube->vertices[0][2]=-sideLength/2,cube->vertices[0][3]=0; //(L)eft (B)ottom (F)ront
	cube->vertices[1][0]=-sideLength/2,cube->vertices[1][1]=sideLength/2,cube->vertices[1][2]=-sideLength/2,cube->vertices[1][3]=0; //LTF
	cube->vertices[2][0]=sideLength/2,cube->vertices[2][1]=-sideLength/2,cube->vertices[2][2]=-sideLength/2,cube->vertices[2][3]=0; //RBF
	cube->vertices[3][0]=sideLength/2,cube->vertices[3][1]=sideLength/2,cube->vertices[3][2]=-sideLength/2,cube->vertices[3][3]=0; //RTF
	cube->vertices[4][0]=-sideLength/2,cube->vertices[4][1]=-sideLength/2,cube->vertices[4][2]=sideLength/2,cube->vertices[4][3]=1; //LBB
	cube->vertices[5][0]=-sideLength/2,cube->vertices[5][1]=sideLength/2,cube->vertices[5][2]=sideLength/2,cube->vertices[5][3]=1; //LTB
	cube->vertices[6][0]=sideLength/2,cube->vertices[6][1]=-sideLength/2,cube->vertices[6][2]=sideLength/2,cube->vertices[6][3]=1; //RBB
	cube->vertices[7][0]=sideLength/2,cube->vertices[7][1]=sideLength/2,cube->vertices[7][2]=sideLength/2,cube->vertices[7][3]=1; //RTB
	for(int i=0;i<4;i++){
		cube->position[i]=position[i];
	}
	cube->id=id;
	cube->type=1;
	cube->verticesCount=8;
}


//CUBE TRANSFORM: Performs a specified transformation on a cube
//Takes in cube struct pointer,transformation type,aixs,and angle(theta)->Returns nothing
void transformCube(struct CUBE* cube,char* transformType,char axis,float theta){
	if(strcmp(transformType,"rotate")==0&&axis=='y'){
		float transform[4][4]={
			{cosf(theta),0,sinf(theta),0},
			{0,1,0,0},
			{-sinf(theta),0,cosf(theta),0},
			{0,0,0,1}
		};
		for(int j=0;j<cube->verticesCount;j++){
			matMult3d(cube->vertices[j],transform);
		}
	}else if(strcmp(transformType,"rotate")==0&&axis=='x'){
		float transform[4][4]={
			{1,0,0,0},
			{0,cosf(theta),-sinf(theta),0},
			{0,sinf(theta),cosf(theta),0},
			{0,0,0,1}
		};
		for(int j=0;j<cube->verticesCount;j++){
			matMult3d(cube->vertices[j],transform);
		}
	}else if(strcmp(transformType,"rotate")==0&&axis=='z'){
		float transform[4][4]={
			{cosf(theta),-sinf(theta),0,0},
			{sinf(theta),cosf(theta),0,0},
			{0,0,1,0},
			{0,0,0,1}
		};
		for(int j=0;j<cube->verticesCount;j++){
			matMult3d(cube->vertices[j],transform);
		}
	}else if(strcmp(transformType,"scale")==0&&axis=='+'){
		float transform[4][4]={
			{1.01,0,0,0},
			{0,1.01,0,0},
			{0,0,1.01,0},
			{0,0,0,1}
		};
		for(int j=0;j<cube->verticesCount;j++){
			matMult3d(cube->vertices[j],transform);
		}
	}else if(strcmp(transformType,"scale")==0&&axis=='-'){
		float transform[4][4]={
			{.99,0,0,0},
			{0,.99,0,0},
			{0,0,.99,0},
			{0,0,0,1}
		};
		for(int j=0;j<cube->verticesCount;j++){
			matMult3d(cube->vertices[j],transform);
		}
	}
}

#endif // TRIG_H
