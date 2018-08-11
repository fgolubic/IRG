#include<iostream>
#include<glm\glm.hpp>
#include<stdio.h>
#include<glm/ext.hpp>
using namespace std;
using namespace glm;

//calculates solution of a 3 equation system using Gauss Elimination
void gElimination(){ 

	cout << "Unesite podatke o sustavu 3 jednadzbe s 3 nepoznanice: " << endl;
	
	mat3 coef;
	vec3 free;
	int freeIndex=0;
	int row=0;
	int temp;

	//load coefs of 3 equations in matrix 3x3 and one 3 dimensional vector
	for (int i = 1; i <= 12; i++){
		cin >> temp;
		if (i % 4){
			coef[i % 4 - 1][row] = temp;
		}
		else{
			row++;
			free[freeIndex] = temp;
			freeIndex++;
		}
	}

	//calculate result of 3 variables by multiplying matrix inverse with vector
	cout << to_string(inverse(coef)*free) << endl << endl;
	
}