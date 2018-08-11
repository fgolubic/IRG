#include<stdio.h>
#include<iostream>
#include<cmath>
#include<glm/glm.hpp>
#include<glm/gtc/type_precision.hpp>
#include<glm/ext.hpp>


using namespace std;
using namespace glm;



void matAndVec(){
	
	dvec3 vector1 = dvec3(2.0, 3.0, -4.0);
	dvec3 temp = dvec3(-1.0, 4.0, -1.0);

	//calculate the sum of two vectors 
	vector1 = vector1 + temp;
	cout <<"v1= "<< vector1.x <<"i+"<< vector1.y  <<"j+"<< vector1.z <<"k\n";

	//find scalar product of 2 vectors using function dot
	double s = dot(vector1,temp);
	cout <<"s= "<< s << "\n";

	//find vector product of 2 vectors using cross function
	dvec3 vector2 = cross(vector1, dvec3(2.0, 2.0, 4.0));
	cout << "v2= " << vector2.x << "i+" << vector2.y << "j+" << vector2.z << "k\n";

	//find ort vector of another vector using function normalize
	dvec3 vector3 = normalize(vector2);
	cout << "v3= " << vector3.x << "i+" << vector3.y << "j+" << vector3.z << "k\n";

	//calculate reverse vector
	dvec3 vector4 = -vector2;
	cout << "v4= " << vector4.x << "i+" << vector4.y << "j+" << vector4.z << "k\n\n\n";


	mat3 mat1 = mat3(1, 2, 3,
					 2, 1, 3,
					 4, 5, 1);

	mat3 mat2 = mat3(-1, 2, -3,
					  5,-2, 7,
					 -4, -1, 3);
	//calculate the sum of two matrix
	cout <<"M1= "<< to_string(mat1+mat2)<<endl<<endl;

	//calculate product of matrix and transposed matrix
	cout <<"M2= "<< to_string(matrixCompMult(mat1, transpose(mat2)))<<endl<<endl;

	//calculate product of matrix and inverted matrix
	cout <<"M3= "<< to_string(matrixCompMult(mat1, inverse(mat2))) << endl<<endl;

}

