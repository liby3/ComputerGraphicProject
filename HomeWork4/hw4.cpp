// CSCI480 Lab4
// Student Name: LiBoyang
// ID#: 12353093
#include "stdafx.h"
#include <iostream>
#include "stdio.h"
#include <stdlib.h>
#include <cmath>
#include <gl/glut.h>
#include <fstream>
#include <string>
using namespace std;

const int MAX_FACE = 3800;
const int MAX_VERTEX = 2000;
struct ASCModel  {
	int num_vertex;
	int num_face;
	float vertex[MAX_VERTEX][3];
	int face[MAX_FACE][4];
};
struct LightVector  {
	int ID;
	float ip;
	float x;
	float y;
	float z;
};
struct AverageNormalVectorVetex  {
	float a;	
	float b;
	float c;
};
struct NormalVectorFace  {
	float a;
	float b;
	float c;
};
struct PlanEquation  {
	float A;
	float B;
	float C;
	float D;
};
struct CBuffer  {
	float r;
	float g;
	float b;
	int color;
};
struct ObjectLight  {
	float r;
	float g;
	float b;
	float Kd;
	float Ks;
	float N;
};
/* Parameters Added by LiBoyang */
float background_r = 0,  background_g = 0,  background_b = 0;
static int red = 1,  green = 2,  blue = 3,  yellow = 4,  white = 5,  black = 6;
int store_current_num_vertex[20] = {0};
int store_current_num_face[20] = {0};
int number_of_vertex_in_one_face[20] = {0};
int counter_of_vertex_vector = 0;
int number_of_light = 0;
int number_of_read_object = 0,  height,  width;
float h,  w,  H,  y,  theta,  Vl,  Vr,  Vb,  Vt,  Px,  Py,  Pz;
float result_vertex_point[MAX_VERTEX][4];
float vertex_point[20][MAX_VERTEX][4];
int vertex_face[20][MAX_FACE][4];
float Current_Matrix[20][4][4],  Current_Matrix_Copy[4][4];
float Mirror[4][4],  GRM[4][4],  PM[4][4],  Translate_Matrix[20][4][4],  Eye_Translate_Matrix[4][4],  Eye_Tilt_Matrix[4][4];
float zBuffer[600][600];
float I_Value[20][MAX_VERTEX];
float Ka = 0.4;
double IMAX = 0;
struct ObjectLight ol[20];
struct CBuffer cBuffer[600][600];
struct PlanEquation pe[MAX_FACE];
struct ASCModel cube;
struct LightVector light_vector[4];
struct NormalVectorFace normal_vector_face[20][MAX_FACE];
struct AverageNormalVectorVetex average_normal_vector_point[20][MAX_VERTEX];
/* Parameters Added by LiBoyang */

/* Functions Added by LiBoyang */
void drawDot(int x, int y, float r, float g, float b);
void Rotate(float a,  float b,  float c);
void Scale(float a,  float b,  float c);
void Translate(float a,  float b,  float c);
void Eye_Translate(float a,  float b,  float c);
void Eye_Tilt(float a,  float b,  float c);
void displayFunc();
void Display();
void Reset();
void ReadFile();
void ReadObject(string filename,  float aa,  float bb,  float cc);
void ViewPort(float vl,  float vr,  float vb,  float vt);
void Observer(float x1,  float x2,  float x3,  float x4,  float x5,  float x6,  float x7,  float x8,  float x9,  float x10);
/* Functions Added by LiBoyang */
/* Get VL,VR, VB, VT and draw the viewport. */
void ViewPort(float vl,  float vr,  float vb,  float vt)  {
	h = vt - vb;
	w = vr - vl;
	Vl = vl;
	Vr = vr;
	Vb = vb;
	Vt = vt;
}
/* Draw point. */
void drawDot(int x, int y, float r, float g, float b)  {
  glBegin(GL_POINTS);  
  glColor3f(r, g, b);
  glVertex2i(x , y);
  glEnd();
}
/* Transform operation: scale. */
void Scale(float a,  float b,  float c)  {
	float scalling_matrix[4][4];
	float Translate_Matrix_Copy[4][4];
	int i,  j,  k;
	for (i = 0 ; i < 4 ; i++)  {
		for (j = 0 ; j < 4 ; j++)  {
			scalling_matrix[i][j] = 0;
			Translate_Matrix_Copy[i][j] = Translate_Matrix[number_of_read_object][i][j];
			Translate_Matrix[number_of_read_object][i][j] = 0;
		}
	}
	scalling_matrix[0][0] = a;
	scalling_matrix[1][1] = b;
	scalling_matrix[2][2] = c;
	scalling_matrix[3][3] = 1;
	for (i = 0 ; i < 4 ; i++)  {
		for (j = 0 ; j < 4 ; j++)  {
			for (k = 0 ; k < 4 ; k++)  {
				Translate_Matrix[number_of_read_object][i][j] += scalling_matrix[i][k] * Translate_Matrix_Copy[k][j];
			}
		}
	}
}
/* Transform operation: rotate. */
void Rotate(float a,  float b,  float c){
	float rotate_matrix[4][4];
	float Translate_Matrix_Copy[4][4];
	int i,  j,  k;
	for (i = 0 ; i < 4 ; i++)  {
		for (j = 0 ; j < 4 ; j++)  {
			rotate_matrix[i][j] = 0;
			Translate_Matrix_Copy[i][j] = Translate_Matrix[number_of_read_object][i][j];
			Translate_Matrix[number_of_read_object][i][j] = 0;
		}
	}
	if  (c != 0)  {
		rotate_matrix[0][0] = cos((c / 180.0 * 3.14159265 ));
		rotate_matrix[0][1] = -sin((c / 180.0 * 3.14159265 ));
		rotate_matrix[1][0] = sin((c / 180.0 * 3.14159265 ));
		rotate_matrix[1][1] = cos((c / 180.0 * 3.14159265 ));
		rotate_matrix[2][2] = 1;
		rotate_matrix[3][3] = 1;
	}
	else if  (a != 0)  {
		rotate_matrix[0][0] = 1;
		rotate_matrix[1][1] = cos((a / 180.0 * 3.14159265 ));
		rotate_matrix[1][2] = -sin((a / 180.0 * 3.14159265 ));
		rotate_matrix[2][1] = sin((a / 180.0 * 3.14159265 ));
		rotate_matrix[2][2] = cos((a / 180.0 * 3.14159265 ));
		rotate_matrix[3][3] = 1;
	}
	else if  (b != 0)  {
		rotate_matrix[0][0] = cos((b / 180.0 * 3.14159265 ));
		rotate_matrix[0][2] = sin((b / 180.0 * 3.14159265 ));
		rotate_matrix[1][1] = 1;
		rotate_matrix[2][0] = -sin((b / 180.0 * 3.14159265 ));
		rotate_matrix[2][2] = cos((b / 180.0 * 3.14159265 ));
		rotate_matrix[3][3] = 1;
	}
	else  {
		rotate_matrix[0][0] = 1;
		rotate_matrix[1][1] = 1;
		rotate_matrix[2][2] = 1;
		rotate_matrix[3][3] = 1;
	}
	for (i = 0 ; i < 4 ; i++)  {
		for (j = 0 ; j < 4 ; j++)  {
			for (k = 0 ; k < 4 ; k++)  {
				Translate_Matrix[number_of_read_object][i][j] += rotate_matrix[i][k] * Translate_Matrix_Copy[k][j];
			}
		}
	}
}
/* Transform operation: translate. */
void Translate(float a,  float b,  float c)  {
	float translation_matrix[4][4];
	float Translate_Matrix_Copy[4][4];
	int i,  j,  k;
	for (i = 0 ; i < 4 ; i++)  {
		for (j = 0 ; j < 4 ; j++)  {
			translation_matrix[i][j] = 0;
			Translate_Matrix_Copy[i][j] = Translate_Matrix[number_of_read_object][i][j];
			Translate_Matrix[number_of_read_object][i][j] = 0;
		}
	}
	translation_matrix[0][0] = 1;
	translation_matrix[1][1] = 1;
	translation_matrix[2][2] = 1;
	translation_matrix[3][3] = 1;
	translation_matrix[0][3] = a;
	translation_matrix[1][3] = b;
	translation_matrix[2][3] = c;
	for (i = 0 ; i < 4 ; i++)  {
		for (j = 0 ; j < 4 ; j++)  {
			for (k = 0 ; k < 4 ; k++)  {
				Translate_Matrix[number_of_read_object][i][j] += translation_matrix[i][k] * Translate_Matrix_Copy[k][j];
			}
		}
	}
}
/* Calculate and get the eye tanslate matrix. */
void Eye_Translate(float a,  float b,  float c)  {
	float translation_matrix[4][4];
	float Eye_Translate_Matrix_Copy[4][4];
	int i,  j,  k;
	for (i = 0 ; i < 4 ; i++)  {
		for (j = 0 ; j < 4 ; j++)  {
			translation_matrix[i][j] = 0;
			Eye_Translate_Matrix_Copy[i][j] = Eye_Translate_Matrix[i][j];
			Eye_Translate_Matrix[i][j] = 0;
		}
	}
	translation_matrix[0][0] = 1;
	translation_matrix[1][1] = 1;
	translation_matrix[2][2] = 1;
	translation_matrix[3][3] = 1;
	translation_matrix[0][3] = a;
	translation_matrix[1][3] = b;
	translation_matrix[2][3] = c;
	for (i = 0 ; i < 4 ; i++)  {
		for (j = 0 ; j < 4 ; j++)  {
			for (k = 0 ; k < 4 ; k++)  {
				Eye_Translate_Matrix[i][j] += translation_matrix[i][k] * Eye_Translate_Matrix_Copy[k][j];
			}
		}
	}

}
/* Calculate and get the eye tilt matrix. */
void Eye_Tilt(float c)  {
	float rotate_matrix[4][4];
	float Eye_Tilt_Matrix_Copy[4][4];
	int i,  j,  k;
	for (i = 0 ; i < 4 ; i++)  {
		for (j = 0 ; j < 4 ; j++)  {
			rotate_matrix[i][j] = 0;
			Eye_Tilt_Matrix_Copy[i][j] = Eye_Tilt_Matrix[i][j];
			Eye_Tilt_Matrix[i][j] = 0;
		}
	}
	rotate_matrix[0][0] = cos((c / 180.0 * 3.14159265 ));
	rotate_matrix[0][1] = -sin((c / 180.0 * 3.14159265 ));
	rotate_matrix[1][0] = sin((c / 180.0 * 3.14159265 ));
	rotate_matrix[1][1] = cos((c / 180.0 * 3.14159265 ));
	rotate_matrix[2][2] = 1;
	rotate_matrix[3][3] = 1;
	for (i = 0 ; i < 4 ; i++)  {
		for (j = 0 ; j < 4 ; j++)  {
			for (k = 0 ; k < 4 ; k++)  {
				Eye_Tilt_Matrix[i][j] += rotate_matrix[i][k] * Eye_Tilt_Matrix_Copy[k][j];
			}
		}
	}
}
/* Set all the transform matrix to identity matrix. */
void Reset()  {
	int i,  j;
	for (i = 0 ; i < 4 ; i++)  {
		for (j = 0 ; j < 4 ; j++)  {
			Eye_Translate_Matrix[i][j] = 0;
			Translate_Matrix[number_of_read_object][i][j] = 0;
			Current_Matrix[number_of_read_object][i][j] = 0;
			Current_Matrix_Copy[i][j] = 0;
			Eye_Tilt_Matrix[i][j] = 0;
		}
	}
	Eye_Tilt_Matrix[0][0] = 1.0;
	Eye_Tilt_Matrix[1][1] = 1.0;
	Eye_Tilt_Matrix[2][2] = 1.0;
	Eye_Tilt_Matrix[3][3] = 1.0;
	Translate_Matrix[number_of_read_object][0][0] = 1.0;
	Translate_Matrix[number_of_read_object][1][1] = 1.0;
	Translate_Matrix[number_of_read_object][2][2] = 1.0;
	Translate_Matrix[number_of_read_object][3][3] = 1.0;
	Eye_Translate_Matrix[0][0] = 1.0;
	Eye_Translate_Matrix[1][1] = 1.0;
	Eye_Translate_Matrix[2][2] = 1.0;
	Eye_Translate_Matrix[3][3] = 1.0;
	Current_Matrix[number_of_read_object][0][0] = 1.0;
	Current_Matrix[number_of_read_object][1][1] = 1.0;
	Current_Matrix[number_of_read_object][2][2] = 1.0;
	Current_Matrix[number_of_read_object][3][3] = 1.0;
}
/* Read file and do as the command. */
void ReadFile()  {
	ifstream inFile;
	inFile.open("hw4.in");
	string command,  command2,  filename = "";	
	float x,  y,  z,  VL,  VR,  VB,  VT,  PX,  PY,  PZ,  CX,  CY,  CZ,  tilt,  near,  far,  half;
	int i;
	string param1 = "",  param2 = "",  param3 = "",  param4 = "",  param5 = "",  param6 = "",  param7 = "",  param8 = "",  param9 = "",  param10 = "";
	char space = ' ';
	while(1)  {
		getline(inFile,  command);
		if  (command == "end")  {
			cout << "当前脚本执行完毕！" << endl << endl << endl << endl;
			break;
		}
		else if  (command == "display")  {
			Display();
		}
		else if  (command == "reset")  {
			Reset();
		}
		else  {
			command2 = "";
			for (i = 0 ; i < command.length() ; i++)  {
				if  (command[i] != space)  {
					command2 += command[i];
				}
				else  {
					i++;
					break;
				}
			}
			if  (command2 == "object")  {
				int j = i;
				while(1)  {
					if  (command[j] != space)  {
						param1 += command[j];
						j++;
					}
					else  {
						j++;
						break;
					}
				}
				while(1)  {
					if  (command[j] != space && j < (int)command.length())  {
						param2 += command[j];
						j++;
					}
					else  {
						j++;
						break;
					}
				}
				while(1)  {
					if  (command[j] != space && j < (int)command.length())  {
						param3 += command[j];
						j++;
					}
					else  {
						j++;
						break;
					}
				}
				while(1)  {
					if  (command[j] != space && j < (int)command.length())  {
						param4 += command[j];
						j++;
					}
					else  {
						j++;
						break;
					}
				}
				while(1)  {
					if  (command[j] != space && j < (int)command.length())  {
						param5 += command[j];
						j++;
					}
					else  {
						j++;
						break;
					}
				}
				while(1)  {
					if  (command[j] != space && j < (int)command.length())  {
						param6 += command[j];
						j++;
					}
					else  {
						j++;
						break;
					}
				}
				while(1)  {
					if  (command[j] != space && j < (int)command.length())  {
						param7 += command[j];
						j++;
					}
					else  {
						j++;
						break;
					}
				}
				ol[number_of_read_object].r = atof(param2.c_str());
				ol[number_of_read_object].g = atof(param3.c_str());
				ol[number_of_read_object].b = atof(param4.c_str());
				ReadObject(param1,  atof(param5.c_str()),  atof(param6.c_str()), atof(param7.c_str()));
				param1 = "";
				param2 = "";
				param3 = "";
				param4 = "";
				param5 = "";
				param6 = "";
				param7 = "";
			}
			/* Read the notes,  ignore it. */
			else if  (command2 == "#")  {
				;
			}
			else if  (command2 == "scale")  {
				int j = i;
				while(1)  {
					if  (command[j] != space)  {
						param1 += command[j];
						j++;
					}
					else  {
						j++;
						break;
					}
				}
				while(1)  {
					if  (command[j] != space && j < (int)command.length())  {
						param2 += command[j];
						j++;
					}
					else  {
						j++;
						break;
					}
				}
				while(1)  {
					if  (command[j] != space && j < (int)command.length())  {
						param3 += command[j];
						j++;
					}
					else  {
						j++;
						break;
					}
				}
				x = atof(param1.c_str());
				y = atof(param2.c_str());
				z = atof(param3.c_str());
				Scale(x,  y,  z);
				param1 = "";
				param2 = "";
				param3 = "";
			}
			else if  (command2 == "translate")  {
				int j = i;
				while(1)  {
					if  (command[j] != space)  {
						param1 += command[j];
						j++;
					}
					else  {
						j++;
						break;
					}
				}
				while(1)  {
					if  (command[j] != space && j < (int)command.length())  {
						param2 += command[j];
						j++;
					}
					else  {
						j++;
						break;
					}
				}
				while(1)  {
					if  (command[j] != space && j < (int)command.length())  {
						param3 += command[j];
						j++;
					}
					else  {
						j++;
						break;
					}
				}
				x = atof(param1.c_str());
				y = atof(param2.c_str());
				z = atof(param3.c_str());
				Translate(x,  y,  z);
				param1 = "";
				param2 = "";
				param3 = "";
			}
			else if  (command2 == "rotate")  {
				int j = i;
				while(1)  {
					if  (command[j] != space)  {
						param1 += command[j];
						j++;
					}
					else  {
						j++;
						break;
					}
				}
				while(1)  {
					if  (command[j] != space && j < (int)command.length())  {
						param2 += command[j];
						j++;
					}
					else  {
						j++;
						break;
					}
				}
				while(1)  {
					if  (command[j] != space && j < (int)command.length())  {
						param3 += command[j];
						j++;
					}
					else  {
						j++;
						break;
					}
				}
				x = atof(param1.c_str());
				y = atof(param2.c_str());
				z = atof(param3.c_str());
				Rotate(x,  y,  z);
				param1 = "";
				param2 = "";
				param3 = "";
			}
			else if  (command2 == "viewport")  {
				int j = i;
				while(1)  {
					if  (command[j] != space)  {
						param1 += command[j];
						j++;
					}
					else  {
						j++;
						break;
					}
				}
				while(1)  {
					if  (command[j] != space && j < (int)command.length())  {
						param2 += command[j];
						j++;
					}
					else  {
						j++;
						break;
					}
				}
				while(1)  {
					if  (command[j] != space && j < (int)command.length())  {
						param3 += command[j];
						j++;
					}
					else  {
						j++;
						break;
					}
				}
				while(1)  {
					if  (command[j] != space && j < (int)command.length())  {
						param4 += command[j];
						j++;
					}
					else  {
						j++;
						break;
					}
				}
				VL = atof(param1.c_str());
				VB = atof(param2.c_str());
				VR = atof(param3.c_str());
				VT = atof(param4.c_str());
				ViewPort(VL,  VB,  VR,  VT);
				param1 = "";
				param2 = "";
				param3 = "";
				param4 = "";
			}
			else if  (command2 == "observer")  {
				int j = i;
				while(1)  {
					if  (command[j] != space)  {
						param1 += command[j];
						j++;
					}
					else  {
						j++;
						break;
					}
				}
				while(1)  {
					if  (command[j] != space && j < (int)command.length())  {
						param2 += command[j];
						j++;
					}
					else  {
						j++;
						break;
					}
				}
				while(1)  {
					if  (command[j] != space && j < (int)command.length())  {
						param3 += command[j];
						j++;
					}
					else  {
						j++;
						break;
					}
				}
				while(1)  {
					if  (command[j] != space && j < (int)command.length())  {
						param4 += command[j];
						j++;
					}
					else  {
						j++;
						break;
					}
				}
				while(1)  {
					if  (command[j] != space && j < (int)command.length())  {
						param5 += command[j];
						j++;
					}
					else  {
						j++;
						break;
					}
				}
				while(1)  {
					if  (command[j] != space && j < (int)command.length())  {
						param6 += command[j];
						j++;
					}
					else  {
						j++;
						break;
					}
				}
				while(1)  {
					if  (command[j] != space && j < (int)command.length())  {
						param7 += command[j];
						j++;
					}
					else  {
						j++;
						break;
					}
				}
				while(1)  {
					if  (command[j] != space && j < (int)command.length())  {
						param8 += command[j];
						j++;
					}
					else  {
						j++;
						break;
					}
				}
				while(1)  {
					if  (command[j] != space && j < (int)command.length())  {
						param9 += command[j];
						j++;
					}
					else  {
						j++;
						break;
					}
				}
				while(1)  {
					if  (command[j] != space && j < (int)command.length())  {
						param10 += command[j];
						j++;
					}
					else  {
						j++;
						break;
					}
				}
				PX = atof(param1.c_str());
				PY = atof(param2.c_str());
				PZ = atof(param3.c_str());
				Px = PX;
				Py = PY;
				Pz = PZ;
				CX = atof(param4.c_str());
				CY = atof(param5.c_str());
				CZ = atof(param6.c_str());
				tilt = atof(param7.c_str());
				near = atof(param8.c_str());
				far = atof(param9.c_str());
				half = atof(param10.c_str());
				Observer(PX,  PY,  PZ,  CX,  CY,  CZ,  tilt,  near,  far,  half);
				param1 = "";
				param2 = "";
				param3 = "";
				param4 = "";
				param5 = "";
				param6 = "";
				param7 = "";
				param8 = "";
				param9 = "";
				param10 = "";
			}
			else if  (command2 == "ambient")  {
				int j = i;
				while(1)  {
					if  (command[j] != space && j < (int)command.length())  {
						param1 += command[j];
						j++;
					}
					else  {
						j++;
						break;
					}
				}
				Ka = atof(param1.c_str());
				param1 = "";
			}
			else if  (command2 == "light")  {
				int j = i;
				while(1)  {
					if  (command[j] != space)  {
						param1 += command[j];
						j++;
					}
					else  {
						j++;
						break;
					}
				}
				while(1)  {
					if  (command[j] != space && j < (int)command.length())  {
						param2 += command[j];
						j++;
					}
					else  {
						j++;
						break;
					}
				}
				while(1)  {
					if  (command[j] != space && j < (int)command.length())  {
						param3 += command[j];
						j++;
					}
					else  {
						j++;
						break;
					}
				}
				while(1)  {
					if  (command[j] != space && j < (int)command.length())  {
						param4 += command[j];
						j++;
					}
					else  {
						j++;
						break;
					}
				}
				while(1)  {
					if  (command[j] != space && j < (int)command.length())  {
						param5 += command[j];
						j++;
					}
					else  {
						j++;
						break;
					}
				}
				light_vector[number_of_light].ID = atof(param1.c_str());
				light_vector[number_of_light].ip = atof(param2.c_str());
				light_vector[number_of_light].x = atof(param3.c_str());
				light_vector[number_of_light].y = atof(param4.c_str());
				light_vector[number_of_light].z = atof(param5.c_str());
				param1 = "";
				param2 = "";
				param3 = "";
				param4 = "";
				param5 = "";
				number_of_light ++;
			}
			else if  (command2 == "background")  {
				int j = i;
				while(1)  {
					if  (command[j] != space)  {
						param1 += command[j];
						j++;
					}
					else  {
						j++;
						break;
					}
				}
				while(1)  {
					if  (command[j] != space && j < (int)command.length())  {
						param2 += command[j];
						j++;
					}
					else  {
						j++;
						break;
					}
				}
				while(1)  {
					if  (command[j] != space && j < (int)command.length())  {
						param3 += command[j];
						j++;
					}
					else  {
						j++;
						break;
					}
				}
				background_r = atof(param1.c_str());
				background_g = atof(param2.c_str());
				background_b = atof(param3.c_str());
				param1 = "";
				param2 = "";
				param3 = "";
			}
			else if (command2 == "") {
				;
			}
		}
		
	}
}
/* Read data from ASC file. */
void ReadObject(string filename,  float aa,  float bb,  float cc)  {
	ifstream fin(filename);
	fin >> cube.num_vertex >> cube.num_face;
	int i,  n,  j;
	float x1 = 0,  y1 = 0,  z1 = 0;
	float x2 = 0,  y2 = 0,  z2 = 0;
	float x3 = 0,  y3 = 0,  z3 = 0;
	float a[3],  b[3],  c[3];
	for (i = 0 ; i < cube.num_vertex ; i++)  {		
		fin >> cube.vertex[i][0] >> cube.vertex[i][1] >> cube.vertex[i][2];
		vertex_point[number_of_read_object][i][0] = cube.vertex[i][0];
		vertex_point[number_of_read_object][i][1] = cube.vertex[i][1];
		vertex_point[number_of_read_object][i][2] = cube.vertex[i][2];
		vertex_point[number_of_read_object][i][3] = 1;
		average_normal_vector_point[number_of_read_object][i].a = 0;
		average_normal_vector_point[number_of_read_object][i].b = 0;
		average_normal_vector_point[number_of_read_object][i].c = 0;
		I_Value[number_of_read_object][i] = 0;
	}
	for (i = 0 ; i < cube.num_face ; i++)  {
		fin >> n;
		if  (n == 3)  {
			number_of_vertex_in_one_face[number_of_read_object] = 3;
			fin >> cube.face[i][0] >> cube.face[i][1] >> cube.face[i][2];
			vertex_face[number_of_read_object][i][0] = cube.face[i][0];
			vertex_face[number_of_read_object][i][1] = cube.face[i][1];
			vertex_face[number_of_read_object][i][2] = cube.face[i][2];		
			x1 = vertex_point[number_of_read_object][cube.face[i][0] - 1][0];
			y1 = vertex_point[number_of_read_object][cube.face[i][0] - 1][1];
			z1 = vertex_point[number_of_read_object][cube.face[i][0] - 1][2];
			x2 = vertex_point[number_of_read_object][cube.face[i][1] - 1][0];
			y2 = vertex_point[number_of_read_object][cube.face[i][1] - 1][1];
			z2 = vertex_point[number_of_read_object][cube.face[i][1] - 1][2];
			x3 = vertex_point[number_of_read_object][cube.face[i][2] - 1][0];
			y3 = vertex_point[number_of_read_object][cube.face[i][2] - 1][1];
			z3 = vertex_point[number_of_read_object][cube.face[i][2] - 1][2];
			a[0] = x2 - x1,  a[1] = y2 - y1,  a[2] = z2 - z1;
			b[0] = x3 - x2,  b[1] = y3 - y2,  b[2] = z3 - z2;
			c[0] = a[1]*b[2] - a[2]*b[1],  c[1] = a[2]*b[0] - a[0]*b[2],  c[2] = a[0]*b[1] - a[1]*b[0];
			normal_vector_face[number_of_read_object][i].a = c[0];
			normal_vector_face[number_of_read_object][i].b = c[1];
			normal_vector_face[number_of_read_object][i].c = c[2];
		}
		else if  (n == 4)  {
			number_of_vertex_in_one_face[number_of_read_object] = 4;
			fin >> cube.face[i][0] >> cube.face[i][1] >> cube.face[i][2] >> cube.face[i][3];
			vertex_face[number_of_read_object][i][0] = cube.face[i][0];
			vertex_face[number_of_read_object][i][1] = cube.face[i][1];
			vertex_face[number_of_read_object][i][2] = cube.face[i][2];
			vertex_face[number_of_read_object][i][3] = cube.face[i][3];
			x1 = vertex_point[number_of_read_object][cube.face[i][0] - 1][0];
			y1 = vertex_point[number_of_read_object][cube.face[i][0] - 1][1];
			z1 = vertex_point[number_of_read_object][cube.face[i][0] - 1][2];
			x2 = vertex_point[number_of_read_object][cube.face[i][1] - 1][0];
			y2 = vertex_point[number_of_read_object][cube.face[i][1] - 1][1];
			z2 = vertex_point[number_of_read_object][cube.face[i][1] - 1][2];
			x3 = vertex_point[number_of_read_object][cube.face[i][2] - 1][0];
			y3 = vertex_point[number_of_read_object][cube.face[i][2] - 1][1];
			z3 = vertex_point[number_of_read_object][cube.face[i][2] - 1][2];
			a[0] = x2 - x1,  a[1] = y2 - y1,  a[2] = z2 - z1;
			b[0] = x3 - x2,  b[1] = y3 - y2,  b[2] = z3 - z2;
			c[0] = a[1]*b[2] - a[2]*b[1],  c[1] = a[2]*b[0] - a[0]*b[2],  c[2] = a[0]*b[1] - a[1]*b[0];
			normal_vector_face[number_of_read_object][i].a = c[0];
			normal_vector_face[number_of_read_object][i].b = c[1];
			normal_vector_face[number_of_read_object][i].c = c[2];
		}
	}	
	ol[number_of_read_object].Kd = aa;
	ol[number_of_read_object].Ks = bb;
	ol[number_of_read_object].N = cc;
	store_current_num_vertex[number_of_read_object] = cube.num_vertex;
	store_current_num_face[number_of_read_object] = cube.num_face;
	number_of_read_object ++;
	cout << "Finished reading object " + filename + "!" << endl << endl << endl;
}
/* Calculate and get GRM, Mirror Matrix. */
void Observer(float x1,  float x2,  float x3,  float x4,  float x5,  float x6,  float x7,  float x8,  float x9,  float x10)  {
	H = x8,  y = x9,  theta = x10;
	int i,  j;
	for (i = 0 ; i < 4 ; i++)  {
		for (j = 0 ; j < 4 ; j++)  {
			GRM[i][j] = 0;
			Mirror[i][j] = 0;
		}
	}
	Mirror[0][0] = -1,  Mirror[1][1] = 1,  Mirror[2][2] = 1,  Mirror[3][3] = 1;
	float vt[3],  vz[3],  v1[3],  v2[3],  v3[3];
	vt[0] = 0,  vt[1] = 1,  vt[2] = 0;
	vz[0] = x4 - x1,  vz[1] = x5 - x2,  vz[2] = x6 - x3;
	float length_of_vz = sqrt(vz[0] * vz[0] + vz[1] * vz[1] + vz[2] * vz[2]);
	float length_of_vt = 1;
	v3[0] = vz[0] / length_of_vz,  v3[1] = vz[1] / length_of_vz,  v3[2] = vz[2] / length_of_vz;
	float length_of_v3 = 1;
	v1[0] = vt[1] * vz[2] - vt[2] * vz[1],  v1[1] = vt[2] * vz[0] - vt[0] * vz[1],  v1[2] = vt[0] * vz[1] - vt[1] * vz[0];
	float length_of_v1 = sqrt(v1[0] * v1[0] + v1[1] * v1[1] + v1[2] * v1[2]);
	v1[0] = v1[0] / length_of_v1,  v1[1] = v1[1] / length_of_v1,  v1[2] = v1[2] / length_of_v1;
	v2[0] = v3[1] * v1[2] - v3[2] * v1[1],  v2[1] = v3[2] * v1[0] - v3[0] * v1[1],  v2[2] = v3[0] * v1[1] - v3[1] * v1[0];
	float length_of_v2 = sqrt(v2[0] * v2[0] + v2[1] * v2[1] + v2[2] * v2[2]);
	v2[0] = v2[0] / length_of_v2,  v2[1] = v2[1] / length_of_v2,  v2[2] = v2[2] / length_of_v2;
	GRM[0][0] = v1[0],  GRM[0][1] = v1[1],  GRM[0][2] = v1[2];
	GRM[1][0] = v2[0],  GRM[1][1] = v2[1],  GRM[1][2] = v2[2];
	GRM[2][0] = v3[0],  GRM[2][1] = v3[1],  GRM[2][2] = v3[2],  GRM[3][3] = 1;	
	Eye_Translate(-x1,  -x2,  -x3);
	Eye_Tilt(-x7);
}
/* 3D rendering pipeline, multiply PM, eye tilt, mirror, GRM, eye translate, translate. Then get the final vertex.*/
void Display()  {
	int i,  j,  k;
	int l = 0;
	float AR = 1;
	for (i = 0 ; i < 4 ; i++)  {
		for (j = 0 ; j < 4 ; j++)  {
			PM[i][j] = 0;
		}
	}
	PM[0][0] = 1;
	PM[1][1] = AR;
	PM[2][2] = y * tan((theta / 180.0 * 3.14159265 )) / (y - H);
	PM[2][3] = H * y *  tan((theta / 180.0 * 3.14159265 )) / (H - y);
	PM[3][2] = tan((theta / 180.0 * 3.14159265 ));
	for (i = 0 ; i < 600 ; i++)  {
		for (j = 0 ; j < 600 ; j++)  {
			zBuffer[i][j] = 12353093;
			cBuffer[i][j].r = background_r;
			cBuffer[i][j].g = background_g;
			cBuffer[i][j].b = background_b;
		}
	}
	/* L presents the number of objects.  */
	for (l = 0 ; l < number_of_read_object ; l++)  {
		for (i = 0 ; i < store_current_num_vertex[l] ; i++)  {
			for (j = 0 ; j < store_current_num_face[l] ; j++)  {
				if  (number_of_vertex_in_one_face[l] == 3)  {
					if  ((i == vertex_face[l][j][0]-1) || (i == vertex_face[l][j][1]-1) || (i == vertex_face[l][j][2]-1))  {
						average_normal_vector_point[l][i].a += normal_vector_face[l][j].a;
						average_normal_vector_point[l][i].b += normal_vector_face[l][j].b;
						average_normal_vector_point[l][i].c += normal_vector_face[l][j].c;
						counter_of_vertex_vector++;
					}
				}
				else if  (number_of_vertex_in_one_face[l] == 4)  {
					if  ((i == vertex_face[l][j][0]-1) || (i == vertex_face[l][j][1]-1) || (i == vertex_face[l][j][2]-1) || (i == vertex_face[l][j][3]-1))  {
						average_normal_vector_point[l][i].a += normal_vector_face[l][j].a;
						average_normal_vector_point[l][i].b += normal_vector_face[l][j].b;
						average_normal_vector_point[l][i].c += normal_vector_face[l][j].c;
						counter_of_vertex_vector++;
					}
				}
			}
			average_normal_vector_point[l][i].a /= counter_of_vertex_vector;
			average_normal_vector_point[l][i].b /= counter_of_vertex_vector;
			average_normal_vector_point[l][i].c /= counter_of_vertex_vector;
			float Length_Of_N = sqrt(average_normal_vector_point[l][i].a * average_normal_vector_point[l][i].a + average_normal_vector_point[l][i].b * average_normal_vector_point[l][i].b + average_normal_vector_point[l][i].c * average_normal_vector_point[l][i].c);
			
			/* N vector  */
			average_normal_vector_point[l][i].a /= Length_Of_N;
			average_normal_vector_point[l][i].b /= Length_Of_N;
			average_normal_vector_point[l][i].c /= Length_Of_N;
			counter_of_vertex_vector = 0;
			/* L vector */
			I_Value[l][i] = Ka;
			for (k = 0 ; k < number_of_light ; k++)  {
				float px,  py,  pz;
				px = vertex_point[l][i][0] - light_vector[k].x;
				py = vertex_point[l][i][1] - light_vector[k].y;
				pz = vertex_point[l][i][2] - light_vector[k].z;
				float Length_Of_L = sqrt(px*px + py*py + pz*pz);
				px /= Length_Of_L,  py /= Length_Of_L,  pz /= Length_Of_L;
				/* N*L */
				float Diffuse_Value = ol[l].Kd * light_vector[k].ip * (px * average_normal_vector_point[l][i].a + py * average_normal_vector_point[l][i].b + pz * average_normal_vector_point[l][i].c);
				if  (Diffuse_Value < 0)  {
					Diffuse_Value = 0;
				}
				I_Value[l][i] += Diffuse_Value;
			}
			for (k = 0 ; k < number_of_light ; k++)  {
				float a = 0,  b = 0,  c = 0,  d = 0,  e = 0,  f = 0,  g = 0,  h = 0,  m = 0;
				/* L vector */
				a = light_vector[k].x - vertex_point[l][i][0];
				b = light_vector[k].y - vertex_point[l][i][1];
				c = light_vector[k].z - vertex_point[l][i][2];
				/* V vector */
				d = Px - vertex_point[l][i][0];
				e = Py - vertex_point[l][i][1];
				f = Pz - vertex_point[l][i][2];
				/* H vector */
				g = a + d,  h = b + e,  m = c + f;
				float Length_Of_H = sqrt(g*g + h*h + m*m);
				g /= Length_Of_H,  h /= Length_Of_H,  m /= Length_Of_H;

				double temp = g * average_normal_vector_point[l][i].a + h * average_normal_vector_point[l][i].b + m * average_normal_vector_point[l][i].c;
				if  (temp < 0)  {
					temp = 0;
				}
				double Specular_Value = ol[l].Ks * light_vector[k].ip * pow(temp,  ol[l].N);
				I_Value[l][i] += Specular_Value;
			}
			if  (IMAX <= I_Value[l][i])  {
				IMAX = I_Value[l][i];
			}
		}
		cout << "WTF!" << endl << endl;
		if  (IMAX <= 1)  {
			IMAX = 1;
		}
		for (i = 0 ; i < 4 ; i++)  {
			for (j = 0 ; j < 4 ; j++)  {
				Current_Matrix_Copy[i][j] = 0;
			}
		}
		/* PM * Tilt */
		for (i = 0 ; i < 4 ; i++)  {
			for (j = 0 ; j < 4 ; j++)  {
				for (k = 0 ; k < 4 ; k++)  {
					Current_Matrix_Copy[i][j] += PM[i][k] * Eye_Tilt_Matrix[k][j];
				}
			}
		}
		for (i = 0 ; i < 4 ; i++)  {
			for (j = 0 ; j < 4 ; j++)  {
				Current_Matrix[l][i][j] = Current_Matrix_Copy[i][j];
				Current_Matrix_Copy[i][j] = 0;
			}
		}
		/* Mirror*/
		for (i = 0 ; i < 4 ; i++)  {
			for (j = 0 ; j < 4 ; j++)  {
				for (k = 0 ; k < 4 ; k++)  {
					Current_Matrix_Copy[i][j] += Current_Matrix[l][i][k] * Mirror[k][j];
				}
			}
		}
		for (i = 0 ; i < 4 ; i++)  {
			for (j = 0 ; j < 4 ; j++)  {
				Current_Matrix[l][i][j] = Current_Matrix_Copy[i][j];
				Current_Matrix_Copy[i][j] = 0;
			}
		}
		/* *GRM */
		for (i = 0 ; i < 4 ; i++)  {
			for (j = 0 ; j < 4 ; j++)  {
				for (k = 0 ; k < 4 ; k++)  {
					Current_Matrix_Copy[i][j] += Current_Matrix[l][i][k] * GRM[k][j];
				}
			}
		}
		for (i = 0 ; i < 4 ; i++)  {
			for (j = 0 ; j < 4 ; j++)  {
				Current_Matrix[l][i][j] = Current_Matrix_Copy[i][j];
				Current_Matrix_Copy[i][j] = 0;
			}
		}
		/* *Eye_Translate*/
		for (i = 0 ; i < 4 ; i++)  {
			for (j = 0 ; j < 4 ; j++)  {
				for (k = 0 ; k < 4 ; k++)  {
					Current_Matrix_Copy[i][j] += Current_Matrix[l][i][k] * Eye_Translate_Matrix[k][j];
				}
			}
		}
		for (i = 0 ; i < 4 ; i++)  {
			for (j = 0 ; j < 4 ; j++)  {
				Current_Matrix[l][i][j] = Current_Matrix_Copy[i][j];
				Current_Matrix_Copy[i][j] = 0;
			}
		}
		/* *Translate_Matrix */
		for (i = 0 ; i < 4 ; i++)  {
			for (j = 0 ; j < 4 ; j++)  {
				for (k = 0 ; k < 4 ; k++)  {
					Current_Matrix_Copy[i][j] += Current_Matrix[l][i][k] * Translate_Matrix[l][k][j];
				}
			}
		}
		for (i = 0 ; i < 4 ; i++)  {
			for (j = 0 ; j < 4 ; j++)  {
				Current_Matrix[l][i][j] = Current_Matrix_Copy[i][j];
				Current_Matrix_Copy[i][j] = 0;
			}
		}
		for (i = 0 ; i < store_current_num_vertex[l] ; i++)  {
			for (j = 0 ; j < 4 ; j++)  {
				float temp = 0;
				for (k = 0 ; k < 4 ; k++)  {			
					temp += Current_Matrix[l][j][k] * vertex_point[l][i][k];
				}
				result_vertex_point[i][j] = temp;
			}
		}	
		for (i = 0 ; i < store_current_num_vertex[l] ; i++)  {
			float divide = result_vertex_point[i][3];
			result_vertex_point[i][0] = result_vertex_point[i][0] / divide;
			result_vertex_point[i][1] = result_vertex_point[i][1] / divide;
			result_vertex_point[i][2] = result_vertex_point[i][2] / divide;
			result_vertex_point[i][3] = result_vertex_point[i][3] / divide;
			result_vertex_point[i][0] *= (Vr - Vl) / 2;
			result_vertex_point[i][0] += (Vr - Vl) / 2;
			result_vertex_point[i][1] *= (Vt - Vb) / 2;
			result_vertex_point[i][1] += (Vt - Vb) / 2;
		}
		for (i = 0 ; i < store_current_num_face[l] ; i++)  {
			float x1,  x2,  x3,  y1,  y2,  y3,  z1,  z2,  z3,  x4,  y4,  z4,  z;
			float a[3],  b[3], c[3];
			x1 = result_vertex_point[vertex_face[l][i][0]-1][0],  y1 = result_vertex_point[vertex_face[l][i][0]-1][1],  z1 = result_vertex_point[vertex_face[l][i][0]-1][2];
			x2 = result_vertex_point[vertex_face[l][i][1]-1][0],  y2 = result_vertex_point[vertex_face[l][i][1]-1][1],  z2 = result_vertex_point[vertex_face[l][i][1]-1][2];
			x3 = result_vertex_point[vertex_face[l][i][2]-1][0],  y3 = result_vertex_point[vertex_face[l][i][2]-1][1],  z3 = result_vertex_point[vertex_face[l][i][2]-1][2];
			
			a[0] = x2 - x1,  a[1] = y2 - y1,  a[2] = z2 - z1; 
			b[0] = x3 - x2,  b[1] = y3 - y2,  b[2] = z3 - z2;
			c[0] = a[1]*b[2] - a[2]*b[1],  c[1] = a[2]*b[0] - a[0]*b[2],  c[2] = a[0]*b[1] - a[1]*b[0];
			pe[i].A = c[0],  pe[i].B = c[1],  pe[i].C = c[2],  pe[i].D = -(c[0] * x1 + c[1] * y1 + c[2] * z1);
			for (j = 0 ; j < 600 ; j++)  {
				for (k = 0 ; k < 600 ; k++)  {
					if  (number_of_vertex_in_one_face[l] == 3)  {
						if  ((((j - x1)*(y2 - y1) - (x2 - x1)*(k - y1)) <= 0) && (((j - x2)*(y3 - y2) - (x3 - x2)*(k - y2)) <= 0) && (((j - x3)*(y1 - y3) - (x1 - x3)*(k - y3)) <= 0) || (((j - x1)*(y2 - y1) - (x2 - x1)*(k - y1)) >= 0) && (((j - x2)*(y3 - y2) - (x3 - x2)*(k - y2)) >= 0) && (((j - x3)*(y1 - y3) - (x1 - x3)*(k - y3)) >= 0))  {
							z = -(pe[i].A*j + pe[i].B*k + pe[i].D) / pe[i].C;							
							if  (z < zBuffer[j][k])  {								
								zBuffer[j][k] = z;
								cBuffer[j][k].r = ol[l].r*(I_Value[l][vertex_face[l][i][0]-1] + I_Value[l][vertex_face[l][i][1]-1] + I_Value[l][vertex_face[l][i][2]-1])/(3*IMAX);
								cBuffer[j][k].g = ol[l].g;
								cBuffer[j][k].b = ol[l].b;
							}
						}
					}
					else if  (number_of_vertex_in_one_face[l] == 4)  {
						x4 = result_vertex_point[vertex_face[l][i][3]-1][0],  y4 = result_vertex_point[vertex_face[l][i][3]-1][1],  z4 = result_vertex_point[vertex_face[l][i][3]-1][2];
						if  ((((j - x1)*(y2 - y1) - (x2 - x1)*(k - y1)) <= 0) && (((j - x2)*(y3 - y2) - (x3 - x2)*(k - y2)) <= 0) && (((j - x3)*(y4 - y3) - (x4 - x3)*(k - y3)) <= 0) && (((j - x4)*(y1 - y4) - (x1 - x4)*(k - y4)) <= 0)  || (((j - x1)*(y2 - y1) - (x2 - x1)*(k - y1)) >= 0) && (((j - x2)*(y3 - y2) - (x3 - x2)*(k - y2)) >= 0) && (((j - x3)*(y4 - y3) - (x4 - x3)*(k - y3)) >= 0) && (((j - x4)*(y1 - y4) - (x1 - x4)*(k - y4)) >= 0))  {
							z = -(pe[i].A*j + pe[i].B*k + pe[i].D) / pe[i].C;							
							if  (z < zBuffer[j][k])  {
								cBuffer[j][k].r = ol[l].r*(I_Value[l][vertex_face[l][i][0]-1] + I_Value[l][vertex_face[l][i][1]-1] + I_Value[l][vertex_face[l][i][2]-1] + I_Value[l][vertex_face[l][i][3]-1])/(4*IMAX);
								cBuffer[j][k].g = ol[l].g;
								cBuffer[j][k].b = ol[l].b;
								zBuffer[j][k] = z;
							}
						}
					}
				}
			}
		}
		for (i = 0 ; i < 600 ; i++)  {
			for (j = 0 ; j < 600 ; j++)  {
				drawDot(i,  j,  cBuffer[i][j].r,  cBuffer[i][j].g,  cBuffer[i][j].b);
			}
		}
		glFlush();
	}
}
/* Used in the main function. */
void displayFunc()  {
	// clear the entire window to the background color
	glClear(GL_COLOR_BUFFER_BIT); 
	glClearColor(0.0, 0.0, 0.0, 0.0);
	ReadFile();
	glFlush();
}

void main(int ac, char** av)  {
	int winSizeX, winSizeY;
	if(ac == 3) {
		winSizeX = atoi(av[1]);
		winSizeY = atoi(av[2]);
	}
	else {
		winSizeX = 600;
		winSizeY = 600;
	}
	width  = winSizeX;
	height = winSizeY;
	glutInit(&ac, av);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); // set display mode
	glutInitWindowSize(winSizeX, winSizeY);      // set window size
	glutInitWindowPosition(0, 0);                // set window position on screen
	glutCreateWindow("Lab4 Window");       // set window title
	glutDisplayFunc(displayFunc); // register the redraw function
	glClearColor(0.0, 0.0, 0.0, 0.0);     // set the background to black
	glClear(GL_COLOR_BUFFER_BIT); // clear the buffer
	glMatrixMode(GL_PROJECTION);  // setup coordinate system
	glLoadIdentity();
	gluOrtho2D(0, winSizeX, 0, winSizeY);
	glShadeModel(GL_FLAT);
	glFlush();
	glutMainLoop();
}

