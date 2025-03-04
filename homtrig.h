#ifndef HOMTRIG_H
#define HOMTRIG_H
#include <math.h>
#include <string.h>

struct CUBE_2D{
	float position[3];
	int type;
	int verticesCount;
	float vertices[8][3];
};
struct CUBE_3D{
	float position[4];
	int type;
	int verticesCount;
	float vertices[8][4];
};

struct TRI_2D{
	float position[3];
	int type;
	int verticesCount;
	float vertices[3][3];
};
struct TRI_3D{
	float position[3];
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

void cubeTransform3d(struct CUBE_3D* shape,char* transformType,char axis,float theta){
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
			{1.01,0,0,0},
			{0,1.01,0,0},
			{0,0,1.01,0},
			{0,0,0,1}
		};
		for(int j=0;j<shape->verticesCount;j++){
			matMult3d(shape->vertices[j],transform);
		}
	}else if(strcmp(transformType,"scale")==0&&axis=='-'){
		float transform[4][4]={
			{.99,0,0,0},
			{0,.99,0,0},
			{0,0,.99,0},
			{0,0,0,1}
		};
		for(int j=0;j<shape->verticesCount;j++){
			matMult3d(shape->vertices[j],transform);
		}
	}
}

#endif // TRIG_H
