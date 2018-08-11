#include<iostream>
#include<glm\glm.hpp>
#include<glm\ext.hpp>
using namespace glm;
using namespace std;

//find baricentric coordinates of triangle to check if a point is in the trianle
void baricentric(){
	mat3 triangle;
	vec3 point;
	const int row = 0;

	//load 3 vertex coordinates in 3x3 matrix
	for (int i = 1; i <= 3; i++){
		cout << "Upisi koordinate " << i << ". tocke:" << endl;
		scanf("%f %f %f", &triangle[i - 1][row], &triangle[i - 1][row+1], &triangle[i - 1][row+2]);
	}

	//load point coordinates in vector
	cout << "Upisi koordinate dodatne tocke:" << endl;
	scanf("%f %f %f", &point[0], &point[1], &point[2]);

	//calculate baricentric coordinates by multiplying matrix inverse with vector of the point
	cout << to_string(inverse(triangle)*point) << endl;
}