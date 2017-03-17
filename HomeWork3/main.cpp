// CSCI480 Lab3
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

const int MAX = 5000;
struct ASCModel  {
	int num_vertex;
	int num_face;
	float vertex[MAX][3];
	int face[MAX][4];
};
/* Parameters Added by LiBoyang */
int number_of_vertex_in_one_face = 3,  number_of_translate_matrix = 0,  height,  width;
float h,  w,  H,  y,  theta,  Vl,  Vr,  Vb,  Vt;
static int red = 1,  green = 0,  blue = 0;
struct ASCModel cube;
float result_vertex_point[MAX][4];
float vertex_point[20][MAX][5];
float Current_Matrix[20][4][4],  Current_Matrix_Copy[4][4];
float Mirror[4][4],  GRM[4][4],  PM[4][4],  Translate_Matrix[20][4][4],  Eye_Translate_Matrix[4][4],  Eye_Tilt_Matrix[4][4];
static int draw_view_port[4];
/* Parameters Added by LiBoyang */

/* Functions Added by LiBoyang */
void JudgeColor(int color);
void drawDot(int x, int y, float r, float g, float b);
void drawLine1(int x0, int x1, int y0, int y1, bool xy_interchange,  int color);
void drawLine2(int x0, int x1, int y0, int y1, bool xy_interchange,  int color);
void drawLine3(int x0, int x1, int y0, int y1, bool xy_interchange,  int color);
void drawLine4(int x0, int x1, int y0, int y1, bool xy_interchange,  int color);
void drawLine(int px0,  int px1,  int py0,  int py1,  int color);
void Rotate(float a,  float b,  float c);
void Scale(float a,  float b,  float c);
void Translate(float a,  float b,  float c);
void Eye_Translate(float a,  float b,  float c);
void Eye_Tilt(float a,  float b,  float c);
void displayFunc();
void Display();
void Reset();
void ReadFile();
void ShowCurrentMatrix();
void ReadObject(string filename);
void ViewPort(float vl,  float vr,  float vb,  float vt);
void Observer(float x1,  float x2,  float x3,  float x4,  float x5,  float x6,  float x7,  float x8,  float x9,  float x10);
/* Functions Added by LiBoyang */
/* Get VL,VR, VB, VT and draw the viewport. */
void ViewPort(float vl,  float vr,  float vb,  float vt)  {
	draw_view_port[0] = (int)vl;
	draw_view_port[1] = (int)vr;
	draw_view_port[2] = (int)vb;
	draw_view_port[3] = (int)vt;
	h = vt - vb;
	w = vr - vl;
	Vl = vl;
	Vr = vr;
	Vb = vb;
	Vt = vt;
	drawLine(draw_view_port[0],  draw_view_port[1],  draw_view_port[2],  draw_view_port[2],  2);
	drawLine(draw_view_port[1],  draw_view_port[1],  draw_view_port[2],  draw_view_port[3],  2);
	drawLine(draw_view_port[1],  draw_view_port[0],  draw_view_port[3],  draw_view_port[3],  2);
	drawLine(draw_view_port[0],  draw_view_port[0],  draw_view_port[3],  draw_view_port[2],  2);
}
void drawDot(int x, int y, float r, float g, float b)  {
  glBegin(GL_POINTS);  
  glColor3f(r, g, b);
  glVertex2i(x , y);
  glEnd();
}
void drawLine1(int x0, int x1, int y0, int y1, bool xy_interchange,  int color)  {
	bool ifoverflow = false;
	JudgeColor(color);
	int x;
	int y;
	int a = y1 - y0;
	int b = x0 -x1;	
	x = x0;
	y = y0;
	if  (x >= draw_view_port[0] && x <= draw_view_port[1] && y >= draw_view_port[2] && y <= draw_view_port[3])  {
		ifoverflow = true;
	}
	else  {
		ifoverflow = false;
	}
	if  (ifoverflow == true)  {
		drawDot(x,  y,  red,  green,  blue);
	}
	else  {
		;
	}
	if  (xy_interchange == true)  {
		int dint = 2 * a + b;
		int IncE = 2 * a;
		int IncNE = 2 * a + 2 * b;
		while (x <= x1)  {
			if  (dint <= 0)  {
				x++;
				dint += IncE;
			}
			else  {
				x++;
				y++;
				dint += IncNE;
			}
			if  (x >= draw_view_port[0] && x <= draw_view_port[1] && y >= draw_view_port[2] && y <= draw_view_port[3])  {
				ifoverflow = true;
			}
			else  {
				ifoverflow = false;
			}
			if  (ifoverflow == true)  {
				drawDot(x,  y,  red,  green,  blue);
			}
			else  {
				;
			}
		}	
	}
	else  {
		int dint = a + 2 * b;
		int IncN = 2 * b;
		int IncNE = 2 * a + 2 * b;
		while (y <= y1)  {
			if  (dint > 0)  {
				y++;
				dint += IncN;
			}
			else  {
				x++;
				y++;
				dint += IncNE;
			}
			if  (x >= draw_view_port[0] && x <= draw_view_port[1] && y >= draw_view_port[2] && y <= draw_view_port[3])  {
				ifoverflow = true;
			}
			else  {
				ifoverflow = false;
			}
			if  (ifoverflow == true)  {
				drawDot(x,  y,  red,  green,  blue);
			}
			else  {
				;
			}
		}
	}
	glFlush();
}
void drawLine2(int x0, int x1, int y0, int y1, bool xy_interchange,  int color)  {
	bool ifoverflow = false;
	JudgeColor(color);
	int x;
	int y;
	int a = y1 - y0;
	int b = x0 -x1;	
	x = x0;
	y = y0;
	if  (x >= draw_view_port[0] && x <= draw_view_port[1] && y >= draw_view_port[2] && y <= draw_view_port[3])  {
		ifoverflow = true;
	}
	else  {
		ifoverflow = false;
	}
	if  (ifoverflow == true)  {
		drawDot(x,  y,  red,  green,  blue);
	}
	else  {
		;
	}
	if  (xy_interchange == true)  {
		int dint = 2 * a - b;
		int IncE = 2 * a;
		int IncSE = 2 * a - 2 * b;
		while (x <= x1)  {
			if  (dint > 0)  {
				x++;
				dint += IncE;
			}
			else  {
				x++;
				y--;
				dint += IncSE;
			}
			if  (x >= draw_view_port[0] && x <= draw_view_port[1] && y >= draw_view_port[2] && y <= draw_view_port[3])  {
				ifoverflow = true;
			}
			else  {
				ifoverflow = false;
			}
			if  (ifoverflow == true)  {
				drawDot(x,  y,  red,  green,  blue);
			}
			else  {
				;
			}
		}
	}
	else  {
		int dint = a - 2 * b;
		int IncS = (-2) * b;
		int IncSE = 2 * a - 2 * b;
		while (y >= y1)  {
			if  (dint <= 0)  {
				y--;
				dint += IncS;
			}
			else  {
				x++;
				y--;
				dint += IncSE;
			}
			if  (x >= draw_view_port[0] && x <= draw_view_port[1] && y >= draw_view_port[2] && y <= draw_view_port[3])  {
				ifoverflow = true;
			}
			else  {
				ifoverflow = false;
			}
			if  (ifoverflow == true)  {
				drawDot(x,  y,  red,  green,  blue);
			}
			else  {
				;
			}
		}
	}
	glFlush();
}
void drawLine3(int x0, int x1, int y0, int y1, bool xy_interchange,  int color)  {
	bool ifoverflow = false;
	JudgeColor(color);
	int x;
	int y;
	int a = y1 - y0;
	int b = x0 -x1;	
	x = x0;
	y = y0;
	if  (x >= draw_view_port[0] && x <= draw_view_port[1] && y >= draw_view_port[2] && y <= draw_view_port[3])  {
		ifoverflow = true;
	}
	else  {
		ifoverflow = false;
	}
	if  (ifoverflow == true)  {
		drawDot(x,  y,  red,  green,  blue);
	}
	else  {
		;
	}
	if  (xy_interchange == true)  {
		int dint = (-2) * a + b;
		int IncW = (-2) * a;
		int IncNW = (-2) * a + 2 * b;
		while (x >= x1)  {
			if  (dint <= 0)  {
				x--;
				y++;
				dint += IncNW;
			}
			else  {
				x--;
				dint += IncW;
			}
			if  (x >= draw_view_port[0] && x <= draw_view_port[1] && y >= draw_view_port[2] && y <= draw_view_port[3])  {
				ifoverflow = true;
			}
			else  {
				ifoverflow = false;
			}
			if  (ifoverflow == true)  {
				drawDot(x,  y,  red,  green,  blue);
			}
			else  {
				;
			}
		}
	}
	else  {
		int dint = (-1) * a + 2 * b;
		int IncN = 2 * b;
		int IncNW = (-2) * a + 2 * b;
		while (y <= y1)  {
			if  (dint > 0)  {
				x--;
				y++;
				dint += IncNW;
			}
			else  {
				y++;
				dint += IncN;
			}
			if  (x >= draw_view_port[0] && x <= draw_view_port[1] && y >= draw_view_port[2] && y <= draw_view_port[3])  {
				ifoverflow = true;
			}
			else  {
				ifoverflow = false;
			}
			if  (ifoverflow == true)  {
				drawDot(x,  y,  red,  green,  blue);
			}
			else  {
				;
			}
		}
	}
	glFlush();
}
void drawLine4(int x0, int x1, int y0, int y1, bool xy_interchange,  int color)  {
	bool ifoverflow = false;
	JudgeColor(color);
	int x;
	int y;
	int a = y1 - y0;
	int b = x0 -x1;
	x = x0;
	y = y0;
	if  (x >= draw_view_port[0] && x <= draw_view_port[1] && y >= draw_view_port[2] && y <= draw_view_port[3])  {
		ifoverflow = true;
	}
	else  {
		ifoverflow = false;
	}
	if  (ifoverflow == true)  {
		drawDot(x,  y,  red,  green,  blue);
	}
	else  {
		;
	}
	if  (xy_interchange == true)  {
		int dint = (-2) * a - b;
		int IncW = (-2) * a;
		int IncSW = (-2) * a + (-2) * b;
		while (x >= x1)  {
			if  (dint > 0)  {
				x--;
				y--;
				dint += IncSW;
			}
			else  {
				x--;
				dint += IncW;
			}
			if  (x >= draw_view_port[0] && x <= draw_view_port[1] && y >= draw_view_port[2] && y <= draw_view_port[3])  {
				ifoverflow = true;
			}
			else  {
				ifoverflow = false;
			}
			if  (ifoverflow == true)  {
				drawDot(x,  y,  red,  green,  blue);
			}
			else  {
				;
			}
		}
	}
	else  {
		int dint = (-1) * a - 2 * b;
		int IncS = (-2) * b;
		int IncSW = (-2) * a + (-2) * b;
		while (y >= y1)  {
			if  (dint <= 0)  {
				x--;
				y--;
				dint += IncSW;
			}
			else  {
				y--;
				dint += IncS;
			}
			if  (x >= draw_view_port[0] && x <= draw_view_port[1] && y >= draw_view_port[2] && y <= draw_view_port[3])  {
				ifoverflow = true;
			}
			else  {
				ifoverflow = false;
			}
			if  (ifoverflow == true)  {
				drawDot(x,  y,  red,  green,  blue);
			}
			else  {
				;
			}
		}
	}
	glFlush();
}
void drawLine(int px0,  int px1,  int py0,  int py1,  int color)  {
	if  ((px0 <= px1) && (py0 <= py1))  {
		if  ((px1 - px0) > (py1 - py0))  {
			drawLine1(px0,  px1,  py0,  py1,  true,  color);
		}
		else  {
			drawLine1(px0,  px1,  py0,  py1,  false,  color);
		}
	}
	if  ((px0 <= px1) && (py0 >= py1))  {
		if  ((px1 - px0) > (py0 - py1))  {
			drawLine2(px0,  px1,  py0,  py1,  true,  color);
		}
		else  {
			drawLine2(px0,  px1,  py0,  py1,  false,  color);
		}
	}
	if  ((px0 >= px1) && (py0 <= py1))  {
		if  ((px0 - px1) > (py1 - py0))  {
			drawLine3(px0,  px1,  py0,  py1,  true,  color);
		}
		else  {
			drawLine3(px0,  px1,  py0,  py1,  false,  color);
		}
	}
	if  ((px0 >= px1) && (py0 >= py1))  {
		if  ((px0 - px1) > (py0 - py1))  {
			drawLine4(px0,  px1,  py0,  py1,  true,  color);
		}
		else  {
			drawLine4(px0,  px1,  py0,  py1,  false,  color);
		}
	}
}
/* Jude the color of the line. */
void JudgeColor(int color)  {
	if  (color == 1)  {
		red = 1;
		green = 0;
		blue = 0;
	}
	else if  (color == 2)  {
		red = 255;
		green = 255;
		blue = 255;
	}
	else if  (color == 3)  {
		red = 0;
		green = 0;
		blue = 1;
	}
}
/* Print the current transform matrix to help debug. */
void ShowCurrentMatrix()  {
	int i,  j;
	cout << "Current Transform Matrix:" << endl ;
	for (i = 0 ; i < 4 ; i++)  {
		for (j = 0 ; j < 4 ; j++)  {
			printf("%.2f ",  Current_Matrix[number_of_translate_matrix][i][j]);
		}
		cout << endl;
	}
	cout << "------------------------------------------------------------" << endl << endl;
}
/* Transform operation: scale. */
void Scale(float a,  float b,  float c)  {
	float scalling_matrix[4][4];
	float Translate_Matrix_Copy[4][4];
	int i,  j,  k;
	for (i = 0 ; i < 4 ; i++)  {
		for (j = 0 ; j < 4 ; j++)  {
			scalling_matrix[i][j] = 0;
			Translate_Matrix_Copy[i][j] = Translate_Matrix[number_of_translate_matrix][i][j];
			Translate_Matrix[number_of_translate_matrix][i][j] = 0;
		}
	}
	scalling_matrix[0][0] = a;
	scalling_matrix[1][1] = b;
	scalling_matrix[2][2] = c;
	scalling_matrix[3][3] = 1;
	for (i = 0 ; i < 4 ; i++)  {
		for (j = 0 ; j < 4 ; j++)  {
			for (k = 0 ; k < 4 ; k++)  {
				Translate_Matrix[number_of_translate_matrix][i][j] += scalling_matrix[i][k] * Translate_Matrix_Copy[k][j];
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
			Translate_Matrix_Copy[i][j] = Translate_Matrix[number_of_translate_matrix][i][j];
			Translate_Matrix[number_of_translate_matrix][i][j] = 0;
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
				Translate_Matrix[number_of_translate_matrix][i][j] += rotate_matrix[i][k] * Translate_Matrix_Copy[k][j];
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
			Translate_Matrix_Copy[i][j] = Translate_Matrix[number_of_translate_matrix][i][j];
			Translate_Matrix[number_of_translate_matrix][i][j] = 0;
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
				Translate_Matrix[number_of_translate_matrix][i][j] += translation_matrix[i][k] * Translate_Matrix_Copy[k][j];
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
			Translate_Matrix[number_of_translate_matrix][i][j] = 0;
			Current_Matrix[number_of_translate_matrix][i][j] = 0;
			Current_Matrix_Copy[i][j] = Current_Matrix[number_of_translate_matrix][i][j];
			Eye_Tilt_Matrix[i][j] = 0;
		}
	}
	Eye_Tilt_Matrix[0][0] = 1.0;
	Eye_Tilt_Matrix[1][1] = 1.0;
	Eye_Tilt_Matrix[2][2] = 1.0;
	Eye_Tilt_Matrix[3][3] = 1.0;
	Translate_Matrix[number_of_translate_matrix][0][0] = 1.0;
	Translate_Matrix[number_of_translate_matrix][1][1] = 1.0;
	Translate_Matrix[number_of_translate_matrix][2][2] = 1.0;
	Translate_Matrix[number_of_translate_matrix][3][3] = 1.0;
	Eye_Translate_Matrix[0][0] = 1.0;
	Eye_Translate_Matrix[1][1] = 1.0;
	Eye_Translate_Matrix[2][2] = 1.0;
	Eye_Translate_Matrix[3][3] = 1.0;
	Current_Matrix[number_of_translate_matrix][0][0] = 1.0;
	Current_Matrix[number_of_translate_matrix][1][1] = 1.0;
	Current_Matrix[number_of_translate_matrix][2][2] = 1.0;
	Current_Matrix[number_of_translate_matrix][3][3] = 1.0;
}
/* Read file and do as the command. */
void ReadFile()  {
	ifstream inFile;
	inFile.open("hw3.in");
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
					if  (command[j] != space && j < (int)command.length())  {
						param1 += command[j];
						j++;
					}
					else  {
						j++;
						break;
					}
				}
				cout << "Object's parameter is: " << param1 << endl << endl;
				ReadObject(param1);
				param1 = "";
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
				cout << "Scale's parameter is: " << x << " " << y << " " << z << endl << endl;
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
				cout << "Translate's parameter is: " << x << " " << y << " " << z << endl << endl;
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
				cout << "Rotate's parameter is: " << x << " " << y << " " << z << endl << endl;
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
				cout << "Viewport's parameter is: " << VL << " " << VB << " " << VR << " " << VT << endl << endl;
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
				cout << "Observer's parameter is: " << PX << " " << PY << " " << PZ << " " << CX << " " << 
					CY << " " << CZ << " " << tilt << " " << near << " " << far << " " << half << endl << endl;
			}
			/* Read the blank line,  igonre it. */
			
			else if (command2 == "") {
				;
			}
		}
		
	}
}
/* Read data from ASC file. */
void ReadObject(string filename)  {
	ifstream fin(filename);
	fin >> cube.num_vertex >> cube.num_face;
	int i,  n;
	for (i = 0 ; i < cube.num_vertex ; i++)  {		
		fin >> cube.vertex[i][0] >> cube.vertex[i][1] >> cube.vertex[i][2];
		vertex_point[number_of_translate_matrix][i][0] = cube.vertex[i][0];
		vertex_point[number_of_translate_matrix][i][1] = cube.vertex[i][1];
		vertex_point[number_of_translate_matrix][i][2] = cube.vertex[i][2];
		vertex_point[number_of_translate_matrix][i][3] = 1;
	}
	for (i = 0 ; i < cube.num_face ; i++)  {
		fin >> n;
		if  (n == 4)  {
			number_of_vertex_in_one_face = 4;
			fin >> cube.face[i][0] >> cube.face[i][1] >> cube.face[i][2] >> cube.face[i][3];
		}
		else if  (n == 3)  {
			number_of_vertex_in_one_face = 3;
			fin >> cube.face[i][0] >> cube.face[i][1] >> cube.face[i][2];
		}		
	}
	number_of_translate_matrix ++;
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
	cout << "GRM Matrix:" << endl;
	for (i = 0 ; i < 4 ; i++)  {
		for (j = 0 ; j < 4 ; j++)  {
			printf("%.2f ",  GRM[i][j]);
		}
		cout << endl;
	}
	cout << "---------------------------------------------------------" <<endl << endl;
	cout << "Eye Mirror Matrix:" << endl;
	for (i = 0 ; i < 4 ; i++)  {
		for (j = 0 ; j < 4 ; j++)  {
			printf("%.2f ",  Mirror[i][j]);
		}
		cout << endl;
	}
	cout << "---------------------------------------------------------" <<endl << endl;
	Eye_Translate(-x1,  -x2,  -x3);
	Eye_Tilt(-x7);
	cout << "Eye Tilt Matrix: "<< endl;
	for (i = 0 ; i < 4 ; i++)  {
		for (j = 0 ; j < 4 ; j++)  {
			printf("%.2f ",  Eye_Tilt_Matrix[i][j]);
		}

		cout << endl;
	}
	cout << "---------------------------------------------------------" <<endl << endl;
	cout << "Eye Translate Matrix: "<< endl;
	for (i = 0 ; i < 4 ; i++)  {
		for (j = 0 ; j < 4 ; j++)  {
			printf("%.2f ",  Eye_Translate_Matrix[i][j]);
		}

		cout << endl;
	}
	cout << "---------------------------------------------------------" <<endl << endl;
}
/* 3D rendering pipeline, multiply PM, eye tilt, mirror, GRM, eye translate, translate. Then get the final vertex.*/
void Display()  {
	int i,  j,  k;
	float AR = w / h;
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
	cout << "Projection Matrix: " << endl;
	for (i = 0 ; i < 4 ; i++)  {
		for (j = 0 ; j < 4 ; j++)  {
			printf("%.2f ",  PM[i][j]);
		}
		cout << endl;
	}
	cout << "---------------------------------------------------------" <<endl << endl;
	int l = 0;
	for (l = 0 ; l < number_of_translate_matrix ; l++)  {
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
		for (i = 0 ; i < cube.num_vertex ; i++)  {
			for (j = 0 ; j < 4 ; j++)  {
				float temp = 0;
				for (k = 0 ; k < 4 ; k++)  {			
					temp += Current_Matrix[l][j][k] * vertex_point[l][i][k];
				}
				result_vertex_point[i][j] = temp;
			}
		}	
		for (i = 0 ; i < cube.num_vertex ; i++)  {
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
		for (j = 0 ; j < cube.num_face ; j++)  {
			if  (number_of_vertex_in_one_face == 3)  {
				drawLine(result_vertex_point[cube.face[j][0] - 1][0],  result_vertex_point[cube.face[j][1] - 1][0],  result_vertex_point[cube.face[j][0] - 1][1],  result_vertex_point[cube.face[j][1] - 1][1],  2);
				drawLine(result_vertex_point[cube.face[j][1] - 1][0],  result_vertex_point[cube.face[j][2] - 1][0],  result_vertex_point[cube.face[j][1] - 1][1],  result_vertex_point[cube.face[j][2] - 1][1],  2);
				drawLine(result_vertex_point[cube.face[j][2] - 1][0],  result_vertex_point[cube.face[j][0] - 1][0],  result_vertex_point[cube.face[j][2] - 1][1],  result_vertex_point[cube.face[j][0] - 1][1],  2);
			}
			else if  (number_of_vertex_in_one_face == 4)  {
				drawLine(result_vertex_point[cube.face[j][0] - 1][0],  result_vertex_point[cube.face[j][1] - 1][0],  result_vertex_point[cube.face[j][0] - 1][1],  result_vertex_point[cube.face[j][1] - 1][1],  2);
				drawLine(result_vertex_point[cube.face[j][1] - 1][0],  result_vertex_point[cube.face[j][2] - 1][0],  result_vertex_point[cube.face[j][1] - 1][1],  result_vertex_point[cube.face[j][2] - 1][1],  2);
				drawLine(result_vertex_point[cube.face[j][2] - 1][0],  result_vertex_point[cube.face[j][3] - 1][0],  result_vertex_point[cube.face[j][2] - 1][1],  result_vertex_point[cube.face[j][3] - 1][1],  2);
				drawLine(result_vertex_point[cube.face[j][3] - 1][0],  result_vertex_point[cube.face[j][0] - 1][0],  result_vertex_point[cube.face[j][3] - 1][1],  result_vertex_point[cube.face[j][0] - 1][1],  2);
			}
		}
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
		winSizeX = 800;
		winSizeY = 600;
	}
	width  = winSizeX;
	height = winSizeY;
	glutInit(&ac, av);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); // set display mode
	glutInitWindowSize(winSizeX, winSizeY);      // set window size
	glutInitWindowPosition(0, 0);                // set window position on screen
	glutCreateWindow("Lab3 Window");       // set window title
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

