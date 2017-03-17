// CSCI480 Lab2
// Student Name: LiBoyang
// ID#: 12353093
#include "stdafx.h"
#include <iostream>
#include <stdlib.h>
#include <cmath>
#include <gl/glut.h>
#include <fstream>
#include <string>

using namespace std;
/* Added parameters by LiBoyang */

/* The size of the window. */
int height, width;
static float current_transform_matrix[3][3];
static float temp_matrix[3][3];

/* The square vertice matrix 4*3 */
static float square_vertice[10][4][3];
/* The result corrdinates of the square. */
static float result_square_vertice[10][4][3];
/* The transpose of the 4*3 matrix */
static float transpose_square_vertice[3][4];
static float temp_transpose_square_vertice[3][4];

/* The triangle vertice matrix 3*3 */
static float triangle_vertice[10][3][3];
/* The result corrdinates of the square. */
static float result_triangle_vertice[10][4][3];
/* The transpose of the 3*3 matrix. */
static float transpose_triangle_vertice[3][3];
static float temp_transpose_triangle_vertice[3][3];

static float WVM[3][3];

/* Store the points of square and triangle.
 * The number 2 presents x corrdinate and y corrdinate.
 */
static int draw_square_vertice[4][2];
static int draw_triangle_vertice[3][2];
static int draw_view_port[4][2];

/* Control the color of line. */
static int red = 1,  green = 0,  blue = 0;

/* To calculate the number of suqares or triangles which has been drawn. 10 at most. */
static int counter_square = 0;
static int counter_triangle = 0;

/* Added functions by LiBoyang */
void JudgeColor(int color);
void drawDot(int x, int y, float r, float g, float b);
void drawLine1(int x0, int x1, int y0, int y1, bool xy_interchange,  int color);
void drawLine2(int x0, int x1, int y0, int y1, bool xy_interchange,  int color);
void drawLine3(int x0, int x1, int y0, int y1, bool xy_interchange,  int color);
void drawLine4(int x0, int x1, int y0, int y1, bool xy_interchange,  int color);
void ReadFile();
void displayFunc();
void Reset();
void Scale(float a,  float b);
void Rotate(float a);
void Translate(float a,  float b);
void Square();
void ViewPort(float wl,  float wr,  float wb,  float wt,  float vl,  float vr,  float vb,  float vt);
void DrawViewPort();
void DrawSquare();
void DrawTriangle();
void End();


void JudgeColor(int color)  {
	/* Red line,  the color of square. */
	if  (color == 1)  {
		red = 1;
		green = 0;
		blue = 0;
	}
	/* White line,  the color of view port. */
	else if  (color == 2)  {
		red = 255;
		green = 255;
		blue = 255;
	}
	/* Blue line,  the color of triangle. */
	else if  (color == 3)  {
		red = 0;
		green = 0;
		blue = 1;
	}
}

void Reset()  {
	int i,  j;	
	for (i = 0 ; i < 3 ; i++)  {
		for (j = 0 ; j < 3 ; j++)  {
			current_transform_matrix[i][j] = 0;
		}
	}
	current_transform_matrix[0][0] = 1;
	current_transform_matrix[1][1] = 1;
	current_transform_matrix[2][2] = 1;
	cout << "After Reset, Current Transform matrix:" << endl;
	for (i = 0 ; i < 3 ; i++)  {
		for (j = 0 ; j < 3 ; j++)  {
			cout << current_transform_matrix[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;
}

void Scale(float a,  float b)  {
	float scaling_matrix[3][3];
	int i,  j,  k;
	for (i = 0 ; i < 3 ; i++)  {
		for (j = 0 ; j < 3 ; j++)  {
			scaling_matrix[i][j] = 0;
			temp_matrix[i][j] = current_transform_matrix[i][j];
			current_transform_matrix[i][j] = 0;
		}
	}
	scaling_matrix[0][0] = a;
	scaling_matrix[1][1] = b;
	scaling_matrix[2][2] = 1;
	for (i = 0 ; i < 3 ; i++)  {
		for (j = 0 ; j < 3 ; j++)  {
			for (k = 0 ; k < 3 ; k++)  {
				current_transform_matrix[i][j] += scaling_matrix[i][k] * temp_matrix[k][j];
			}				
		}
	}
	cout << "After Scale, Current Transform matrix:" << endl;
	for (i = 0 ; i < 3 ; i++)  {
		for (j = 0 ; j < 3 ; j++)  {
			cout << current_transform_matrix[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;
}

void Rotate(float a)  {
	float rotation_matrix[3][3];
	int i,  j,  k;
	for (i = 0 ; i < 3 ; i++)  {
		for (j = 0 ; j < 3 ; j++)  {
			rotation_matrix[i][j] = 0;
			temp_matrix[i][j] = current_transform_matrix[i][j];
			current_transform_matrix[i][j] = 0;
		}
	}
	rotation_matrix[0][0] = cos((a / 180.0 * 3.14159265 ));
	rotation_matrix[0][1] = -sin((a / 180.0 * 3.14159265 ));
	rotation_matrix[1][0] = sin((a / 180.0 * 3.14159265 ));
	rotation_matrix[1][1] = cos((a / 180.0 * 3.14159265 ));
	rotation_matrix[2][2] = 1;
	for (i = 0 ; i < 3 ; i++)  {
		for (j = 0 ; j < 3 ; j++)  {
			for (k = 0 ; k < 3 ; k++)  {
				current_transform_matrix[i][j] += rotation_matrix[i][k] * temp_matrix[k][j];
			}
		}
	}
	cout << "After Rotate, Current Transform matrix:" << endl;
	for (i = 0 ; i < 3 ; i++)  {
		for (j = 0 ; j < 3 ; j++)  {
			cout << current_transform_matrix[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;
}

void Translate(float a,  float b)  {
	int i,  j,  k;
	float translation_matrix[3][3];
	for (i = 0 ; i < 3 ; i++)  {
		for (j = 0 ; j < 3 ; j++)  {
			temp_matrix[i][j] = current_transform_matrix[i][j];
			current_transform_matrix[i][j] = 0;
			translation_matrix[i][j] = 0;
		}
	}
	translation_matrix[0][2] = a;
	translation_matrix[1][2] = b;
	translation_matrix[0][0] = 1;
	translation_matrix[1][1] = 1;
	translation_matrix[2][2] = 1;
	for (i = 0 ; i < 3 ; i++)  {
		for (j = 0 ; j < 3 ; j++)  {
			for (k = 0 ; k < 3 ; k++)  {
				current_transform_matrix[i][j] += translation_matrix[i][k] * temp_matrix[k][j];
			}
		}
	}
	cout << "After Translate, Current Transform matrix:" << endl;
	for (i = 0 ; i < 3 ; i++)  {
		for (j = 0 ; j < 3 ; j++)  {
			cout << current_transform_matrix[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;
}

void Square()  {
	int i,  j,  k;
	for (i = 0 ; i < 4 ; i++)  {
		for (j = 0 ; j < 3 ; j++)  {
			square_vertice[counter_square][i][j] = 1;
		}
	}
	square_vertice[counter_square][1][0] = -1;
	square_vertice[counter_square][2][0] = -1;
	square_vertice[counter_square][2][1] = -1;
	square_vertice[counter_square][3][1] = -1;
	
	/* Transpose the vertice [4][3] to [3][4]. */
	for (i = 0 ; i < 4 ; i++)  {
		for (j = 0 ; j < 3 ; j++)  {
			transpose_square_vertice[j][i] = square_vertice[counter_square][i][j];
		}
	}
	/* Multiply transposed vertice by current_transform_matrix. */
	for (i = 0 ; i < 3 ; i++)  {
		for (j = 0 ; j < 4 ; j++)  {
			temp_transpose_square_vertice[i][j] = 0;
			for (k = 0 ; k < 3 ; k++)  {
				temp_transpose_square_vertice[i][j] += current_transform_matrix[i][k] * transpose_square_vertice[k][j]; 
			}
		}
	}
	// Transpose the result vertice [3][4] back to [4][3]. 
	for (i = 0 ; i <  4 ; i++)  {
		for (j = 0 ; j <  3 ; j++)  {
			square_vertice[counter_square][i][j] = temp_transpose_square_vertice[j][i];
		}
	}
	counter_square++;
}

void Triangle()  {
	int i,  j,  k;
	triangle_vertice[counter_triangle][0][0] = -1;
	triangle_vertice[counter_triangle][0][1] = -1;
	triangle_vertice[counter_triangle][0][2] = 1;
	triangle_vertice[counter_triangle][1][0] = 1;
	triangle_vertice[counter_triangle][1][1] = -1;
	triangle_vertice[counter_triangle][1][2] = 1;
	triangle_vertice[counter_triangle][2][0] = 0;
	triangle_vertice[counter_triangle][2][1] = 1;
	triangle_vertice[counter_triangle][2][2] = 1;
	/* Transpose the vertice [3][3] to [3][3]. */
	for (i = 0 ; i < 3 ; i++)  {
		for (j = 0 ; j < 3 ; j++)  {
			transpose_triangle_vertice[j][i] = triangle_vertice[counter_triangle][i][j];
		}
	}
	/* Multiply transposed vertice by current_transform_matrix. */
	for (i = 0 ; i < 3 ; i++)  {
		for (j = 0 ; j < 3 ; j++)  {
			temp_transpose_triangle_vertice[i][j] = 0;
			for (k = 0 ; k < 3 ; k++)  {
				temp_transpose_triangle_vertice[i][j] += current_transform_matrix[i][k] * transpose_triangle_vertice[k][j]; 
			}
		}
	}
	// Transpose the result vertice [3][3] back to [3][3]. 
	for (i = 0 ; i <  3 ; i++)  {
		for (j = 0 ; j <  3 ; j++)  {
			triangle_vertice[counter_triangle][i][j] = temp_transpose_triangle_vertice[j][i];
		}
	}
	counter_triangle++;
}

void ReadFile()  {
	float sx,  sy,  degree,  wl,  wr,  wb,  wt,  vl,  vr,  vt,  vb;
	ifstream inFile;
	inFile.open("hw2E.in");
	string command;
	char space = ' ';
	string second_command = "";
	string first_param = "";
	string second_param = "";
	string third_param = "";
	string forth_param = "";
	string fifth_param = "";
	string sixth_param = "";
	string seventh_param = "";
	string eighth_param = "";
	while(1)  {
		getline(inFile,  command);
		if  (command == "end")  {
			cout << "当前脚本执行完毕！" << endl << endl << endl << endl;
			End();
			break;
		}
		else if  (command == "square")  {
			Square();
		}
		else if  (command == "triangle")  {
			Triangle();
		}
		else if  (command == "reset")  {
			Reset();
		}
		else  {
			int i;
			second_command = "";
			for (i = 0 ; i < (int)command.length() ; i++)  {
				if  (command[i] != space)  {
					second_command += command[i];
				}
				else  {
					i++;
					break;
				}
			}
			/* Read the notes,  ignore it. */
			if  (second_command == "#")  {
				;
			}
			/* Read the blank line,  igonre it. */
			else if  (second_command == "")  {
				;
			}
			else if  (second_command == "translate")  {
				int j = i;
				while(1)  {
					if  (command[j] != space)  {
						first_param += command[j];
						j++;
					}
					else  {
						j++;
						break;
					}
				}
				while(1)  {
					if  (command[j] != space && j < (int)command.length())  {
						second_param += command[j];
						j++;
					}
					else  {
						j++;
						break;
					}
				}
				sx = atof(first_param.c_str());
				sy = atof(second_param.c_str());
				Translate(sx,  sy);
				first_param = "";
				second_param = "";
			}
			else if  (second_command == "rotate")  {
				int j = i;
				while(1)  {
					if  (command[j] != space && j < (int)command.length())  {
						first_param += command[j];
						j++;
					}
					else  {
						j++;
						break;
					}
				}
				degree = atof(first_param.c_str());
				Rotate(degree);
				first_param = "";
			}
			else if  (second_command == "scale")  {
				int j = i;
				while(1)  {
					if  (command[j] != space)  {
						first_param += command[j];
						j++;
					}
					else  {
						j++;
						break;
					}
				}
				while(1)  {
					if  (command[j] != space && j < (int)command.length())  {
						second_param += command[j];
						j++;
					}
					else  {
						j++;
						break;
					}
				}
				sx = atof(first_param.c_str());
				sy = atof(second_param.c_str());
				Scale(sx,  sy);
				first_param = "";
				second_param = "";
			}
			else if  (second_command == "view")  {
				int j = i;
				while(1)  {
					if  (command[j] != space)  {
						first_param += command[j];
						j++;
					}
					else  {
						j++;
						break;
					}
				}
				while(1)  {
					if  (command[j] != space)  {
						second_param += command[j];
						j++;
					}
					else  {
						j++;
						break;
					}
				}
				while(1)  {
					if  (command[j] != space)  {
						third_param += command[j];
						j++;
					}
					else  {
						j++;
						break;
					}
				}
				while(1)  {
					if  (command[j] != space)  {
						forth_param += command[j];
						j++;
					}
					else  {
						j++;
						break;
					}
				}
				while(1)  {
					if  (command[j] != space)  {
						fifth_param += command[j];
						j++;
					}
					else  {
						j++;
						break;
					}
				}
				while(1)  {
					if  (command[j] != space)  {
						sixth_param += command[j];
						j++;
					}
					else  {
						j++;
						break;
					}
				}
				while(1)  {
					if  (command[j] != space)  {
						seventh_param += command[j];
						j++;
					}
					else  {
						j++;
						break;
					}
				}
				while(1)  {
					if  (command[j] != space && j < (int)command.length())  {
						eighth_param += command[j];
						j++;
					}
					else  {
						j++;
						break;
					}
				}
				wl = atof(first_param.c_str());
				wr = atof(second_param.c_str());
				wb = atof(third_param.c_str());
				wt = atof(forth_param.c_str());
				vl = atof(fifth_param.c_str());
				vr = atof(sixth_param.c_str());
				vb = atof(seventh_param.c_str());
				vt = atof(eighth_param.c_str());
				ViewPort(wl,  wr,  wb,  wt,  vl,  vr,  vb,  vt);
				first_param = "";
				second_param = "";
				third_param = "";
				forth_param = "";
				fifth_param = "";
				sixth_param = "";
				seventh_param = "";
				eighth_param = "";
			}
		}
	}
}

/* Acordding to the parameters, drawing the suqare and the triangle. */
void ViewPort(float wl,  float wr,  float wb,  float wt,  float vl,  float vr,  float vb,  float vt)  {
	draw_view_port[0][0] = (int)vl;
	draw_view_port[0][1] = (int)vb;
	draw_view_port[1][0] = (int)vr;
	draw_view_port[1][1] = (int)vb;
	draw_view_port[2][0] = (int)vr;
	draw_view_port[2][1] = (int)vt;
	draw_view_port[3][0] = (int)vl;
	draw_view_port[3][1] = (int)vt;
		
	int i,  j,  k,  m;
	for (i = 0 ; i < 3 ; i++)  {
		for (j = 0 ; j < 3 ; j++)  {
			WVM[i][j] = 0;
		}
	}
	WVM[2][2] = 1;
	WVM[0][0] = (vr - vl) / (wr - wl);
	WVM[0][2] = vl - wl * ((vr - vl) / (wr - wl));
	WVM[1][1] = (vt - vb) / (wt - wb);
	WVM[1][2] = vb - wb * ((vt - vb) / (wt - wb));

	/* Transpose the vertice [4][3] to [3][4]. */
	for (m = 0 ; m < counter_square ; m++)  {
		for (i = 0 ; i < 4 ; i++)  {
			for (j = 0 ; j < 3 ; j++)  {
				transpose_square_vertice[j][i] = square_vertice[m][i][j];
			}
		}
		/* Multiply transposed vertice by current_transform_matrix. */
		for (i = 0 ; i < 3 ; i++)  {
			for (j = 0 ; j < 4 ; j++)  {
				temp_transpose_square_vertice[i][j] = 0;
				for (k = 0 ; k < 3 ; k++)  {
					temp_transpose_square_vertice[i][j] += WVM[i][k] * transpose_square_vertice[k][j]; 
				}
			}
		}
		/* Transpose the result vertice [3][4] back to [4][3]. */
		for (i = 0 ; i <  4 ; i++)  {
			for (j = 0 ; j <  3 ; j++)  {
				result_square_vertice[m][i][j] = temp_transpose_square_vertice[j][i];
			}
		}
		/* Began to draw square. */
		for (i = 0 ; i < 4 ; i++)  {
			draw_square_vertice[i][0] = (int)result_square_vertice[m][i][0];
			draw_square_vertice[i][1] = (int)result_square_vertice[m][i][1];
		}
		DrawSquare();
	}		
		
	/* Transpose the vertice [3][3] to [3][3]. */
	
	for (m = 0 ; m < counter_triangle ; m++)  {
		for (i = 0 ; i < 3 ; i++)  {
			for (j = 0 ; j < 3 ; j++)  {
				transpose_triangle_vertice[j][i] = triangle_vertice[m][i][j];
			}
		}
		
		/* Multiply transposed vertice by current_transform_matrix. */
		for (i = 0 ; i < 3 ; i++)  {
			for (j = 0 ; j < 3 ; j++)  {
				temp_transpose_triangle_vertice[i][j] = 0;
				for (k = 0 ; k < 3 ; k++)  {
					temp_transpose_triangle_vertice[i][j] += WVM[i][k] * transpose_triangle_vertice[k][j]; 
				}
			}
		}
		// Transpose the result vertice [3][3] back to [3][3]. 
		for (i = 0 ; i <  3 ; i++)  {
			for (j = 0 ; j <  3 ; j++)  {
				result_triangle_vertice[m][i][j] = temp_transpose_triangle_vertice[j][i];
			}
		}
		for (i = 0 ; i < 3 ; i++)  {
			draw_triangle_vertice[i][0] = (int)(result_triangle_vertice[m][i][0]);
			draw_triangle_vertice[i][1] = (int)(result_triangle_vertice[m][i][1]);
		}
		DrawTriangle();
		
	}
	DrawViewPort();
}

void DrawViewPort()  {
	drawLine1(draw_view_port[0][0],  draw_view_port[1][0],  draw_view_port[0][1],  draw_view_port[1][1],  true,  2);
	drawLine1(draw_view_port[1][0],  draw_view_port[2][0],  draw_view_port[1][1],  draw_view_port[2][1],  false,  2);
	drawLine3(draw_view_port[2][0],  draw_view_port[3][0],  draw_view_port[2][1],  draw_view_port[3][1],  true,  2);
	drawLine4(draw_view_port[3][0],  draw_view_port[0][0],  draw_view_port[3][1],  draw_view_port[0][1],  false,  2);
}

void DrawSquare()  {
	int i;
	for (i = 0 ; i < 4 ; i++)  {
		if  ((draw_square_vertice[i][0] <= draw_square_vertice[(i+1)%4][0]) && (draw_square_vertice[i][1] <= draw_square_vertice[(i+1)%4][1]))  {
			if  ((draw_square_vertice[(i+1)%4][0] - draw_square_vertice[i][0]) > (draw_square_vertice[(i+1)%4][1] - draw_square_vertice[i][1]))  {
				drawLine1(draw_square_vertice[i][0],  draw_square_vertice[(i+1)%4][0],  draw_square_vertice[i][1],  draw_square_vertice[(i+1)%4][1],  true,  1);					
			}
			else  {
				drawLine1(draw_square_vertice[i][0],  draw_square_vertice[(i+1)%4][0],  draw_square_vertice[i][1],  draw_square_vertice[(i+1)%4][1],  false,  1);
			}
		}
		else if  ((draw_square_vertice[i][0] <= draw_square_vertice[(i+1)%4][0]) && (draw_square_vertice[i][1] >= draw_square_vertice[(i+1)%4][1]))  {
			if  ((draw_square_vertice[(i+1)%4][0] - draw_square_vertice[i][0]) > (draw_square_vertice[i][1] - draw_square_vertice[(i+1)%4][1]))  {
				drawLine2(draw_square_vertice[i][0],  draw_square_vertice[(i+1)%4][0],  draw_square_vertice[i][1],  draw_square_vertice[(i+1)%4][1],  true,  1);
			}
			else  {
				drawLine2(draw_square_vertice[i][0],  draw_square_vertice[(i+1)%4][0],  draw_square_vertice[i][1],  draw_square_vertice[(i+1)%4][1],  false,  1);
			}
		}
		else if  ((draw_square_vertice[i][0] >= draw_square_vertice[(i+1)%4][0]) && (draw_square_vertice[i][1] <= draw_square_vertice[(i+1)%4][1]))  {
			if  ((draw_square_vertice[i][0] - draw_square_vertice[(i+1)%4][0]) > (draw_square_vertice[(i+1)%4][1] - draw_square_vertice[i][1]))  {
				drawLine3(draw_square_vertice[i][0],  draw_square_vertice[(i+1)%4][0],  draw_square_vertice[i][1],  draw_square_vertice[(i+1)%4][1],  true,  1);
			}
			else  {
				drawLine3(draw_square_vertice[i][0],  draw_square_vertice[(i+1)%4][0],  draw_square_vertice[i][1],  draw_square_vertice[(i+1)%4][1],  false,  1);
			}
		}
		else if  ((draw_square_vertice[i][0] >= draw_square_vertice[(i+1)%4][0]) && (draw_square_vertice[i][1] >= draw_square_vertice[(i+1)%4][1]))  {
			if  ((draw_square_vertice[i][0] - draw_square_vertice[(i+1)%4][0]) > (draw_square_vertice[i][1] - draw_square_vertice[(i+1)%4][1]))  {
				drawLine4(draw_square_vertice[i][0],  draw_square_vertice[(i+1)%4][0],  draw_square_vertice[i][1],  draw_square_vertice[(i+1)%4][1],  true,  1);
			}
			else  {
				drawLine4(draw_square_vertice[i][0],  draw_square_vertice[(i+1)%4][0],  draw_square_vertice[i][1],  draw_square_vertice[(i+1)%4][1],  false,  1);
			}
		}
	}
} 

void DrawTriangle()  {
	int i;
	for (i = 0 ; i < 3 ; i++)  {
		if  ((draw_triangle_vertice[i][0] <= draw_triangle_vertice[(i+1)%3][0]) && (draw_triangle_vertice[i][1] <= draw_triangle_vertice[(i+1)%3][1]))  {
			if  ((draw_triangle_vertice[(i+1)%3][0] - draw_triangle_vertice[i][0]) > (draw_triangle_vertice[(i+1)%3][1] - draw_triangle_vertice[i][1]))  {
				drawLine1(draw_triangle_vertice[i][0],  draw_triangle_vertice[(i+1)%3][0],  draw_triangle_vertice[i][1],  draw_triangle_vertice[(i+1)%3][1],  true,  3);					
			}
			else  {
				drawLine1(draw_triangle_vertice[i][0],  draw_triangle_vertice[(i+1)%3][0],  draw_triangle_vertice[i][1],  draw_triangle_vertice[(i+1)%3][1],  false,  3);
			}
		}
		else if  ((draw_triangle_vertice[i][0] <= draw_triangle_vertice[(i+1)%3][0]) && (draw_triangle_vertice[i][1] >= draw_triangle_vertice[(i+1)%3][1]))  {
			if  ((draw_triangle_vertice[(i+1)%3][0] - draw_triangle_vertice[i][0]) > (draw_triangle_vertice[i][1] - draw_triangle_vertice[(i+1)%3][1]))  {
				drawLine2(draw_triangle_vertice[i][0],  draw_triangle_vertice[(i+1)%3][0],  draw_triangle_vertice[i][1],  draw_triangle_vertice[(i+1)%3][1],  true,  3);
			}
			else  {
				drawLine2(draw_triangle_vertice[i][0],  draw_triangle_vertice[(i+1)%3][0],  draw_triangle_vertice[i][1],  draw_triangle_vertice[(i+1)%3][1],  false,  3);
			}
		}
		else if  ((draw_triangle_vertice[i][0] >= draw_triangle_vertice[(i+1)%3][0]) && (draw_triangle_vertice[i][1] <= draw_triangle_vertice[(i+1)%3][1]))  {
			if  ((draw_triangle_vertice[i][0] - draw_triangle_vertice[(i+1)%3][0]) > (draw_triangle_vertice[(i+1)%3][1] - draw_triangle_vertice[i][1]))  {
				drawLine3(draw_triangle_vertice[i][0],  draw_triangle_vertice[(i+1)%3][0],  draw_triangle_vertice[i][1],  draw_triangle_vertice[(i+1)%3][1],  true,  3);
			}
			else  {
				drawLine3(draw_triangle_vertice[i][0],  draw_triangle_vertice[(i+1)%3][0],  draw_triangle_vertice[i][1],  draw_triangle_vertice[(i+1)%3][1],  false,  3);
			}
		}
		else if  ((draw_triangle_vertice[i][0] >= draw_triangle_vertice[(i+1)%3][0]) && (draw_triangle_vertice[i][1] >= draw_triangle_vertice[(i+1)%3][1]))  {
			if  ((draw_triangle_vertice[i][0] - draw_triangle_vertice[(i+1)%3][0]) > (draw_triangle_vertice[i][1] - draw_triangle_vertice[(i+1)%3][1]))  {
				drawLine4(draw_triangle_vertice[i][0],  draw_triangle_vertice[(i+1)%3][0],  draw_triangle_vertice[i][1],  draw_triangle_vertice[(i+1)%3][1],  true,  3);
			}
			else  {
				drawLine4(draw_triangle_vertice[i][0],  draw_triangle_vertice[(i+1)%3][0],  draw_triangle_vertice[i][1],  draw_triangle_vertice[(i+1)%3][1],  false,  3);
			}
		}
	}
}

/* Stop the current command,  the array which contains data should be empty. */
void End()  {
	counter_square = 0;
	counter_triangle = 0;
}

// draw a dot at location with integer coordinates (x,y), and with color (r,g,b)
void drawDot(int x, int y, float r, float g, float b) 
{
  glBegin(GL_POINTS);
  
  // set the color of dot
  glColor3f(r, g, b);
  
  // invert height because the opengl origin is at top-left instead of bottom-left
  glVertex2i(x , y);
  //glVertex2i(x , height - y);
  
  glEnd();
}
// Draw line for dx>0 and dy>0
// It means x0 < x1,  y0 < y1,  a > 0 and b < 0
void drawLine1(int x0, int x1, int y0, int y1, bool xy_interchange,  int color)  {
	bool ifoverflow = false;
	JudgeColor(color);
	int x;
	int y;
	// invert height because the opengl origin is at top-left instead of bottom-left
	int a = y1 - y0;
	int b = x0 -x1;
	
	x = x0;
	y = y0;
	if  (x >= draw_view_port[0][0] && x <= draw_view_port[2][0] && y >= draw_view_port[0][1] && y <= draw_view_port[2][1])  {
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
			// We choose E
			if  (dint <= 0)  {
				x++;
				dint += IncE;
			}
			// We choose NE
			else  {
				x++;
				y++;
				dint += IncNE;
			}
			if  (x >= draw_view_port[0][0] && x <= draw_view_port[2][0] && y >= draw_view_port[0][1] && y <= draw_view_port[2][1])  {
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
			// We choose N
			if  (dint > 0)  {
				y++;
				dint += IncN;
			}
			// We choose NE
			else  {
				x++;
				y++;
				dint += IncNE;
			}
			if  (x >= draw_view_port[0][0] && x <= draw_view_port[2][0] && y >= draw_view_port[0][1] && y <= draw_view_port[2][1])  {
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

// Draw line for dx>0 and dy<0
// It means x0 < x1,  y0 > y1,  a < 0 and b < 0
void drawLine2(int x0, int x1, int y0, int y1, bool xy_interchange,  int color)
{
	bool ifoverflow = false;
	JudgeColor(color);

	int x;
	int y;
	// invert height because the opengl origin is at top-left instead of bottom-left
	int a = y1 - y0;
	int b = x0 -x1;	
	x = x0;
	y = y0;
	if  (x >= draw_view_port[0][0] && x <= draw_view_port[2][0] && y >= draw_view_port[0][1] && y <= draw_view_port[2][1])  {
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
			// We choose E
			if  (dint > 0)  {
				x++;
				dint += IncE;
			}
			// We choose SE
			else  {
				x++;
				y--;
				dint += IncSE;
			}
			if  (x >= draw_view_port[0][0] && x <= draw_view_port[2][0] && y >= draw_view_port[0][1] && y <= draw_view_port[2][1])  {
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
			// We choose S
			if  (dint <= 0)  {
				y--;
				dint += IncS;
			}
			// We choose SE
			else  {
				x++;
				y--;
				dint += IncSE;
			}
			if  (x >= draw_view_port[0][0] && x <= draw_view_port[2][0] && y >= draw_view_port[0][1] && y <= draw_view_port[2][1])  {
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

// Draw line for dx<0 and dy>0
// It means x0 > x1,  y0 < y1,  a > 0 and b > 0
void drawLine3(int x0, int x1, int y0, int y1, bool xy_interchange,  int color)
{
	bool ifoverflow = false;
	JudgeColor(color);
	int x;
	int y;
	// invert height because the opengl origin is at top-left instead of bottom-left
	int a = y1 - y0;
	int b = x0 -x1;	
	x = x0;
	y = y0;
	if  (x >= draw_view_port[0][0] && x <= draw_view_port[2][0] && y >= draw_view_port[0][1] && y <= draw_view_port[2][1])  {
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
			// We choose NW
			if  (dint <= 0)  {
				x--;
				y++;
				dint += IncNW;
			}
			// We choose W
			else  {
				x--;
				dint += IncW;
			}
			if  (x >= draw_view_port[0][0] && x <= draw_view_port[2][0] && y >= draw_view_port[0][1] && y <= draw_view_port[2][1])  {
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
			// We choose NW
			if  (dint > 0)  {
				x--;
				y++;
				dint += IncNW;
			}
			// We choose N
			else  {
				y++;
				dint += IncN;
			}
			if  (x >= draw_view_port[0][0] && x <= draw_view_port[2][0] && y >= draw_view_port[0][1] && y <= draw_view_port[2][1])  {
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

// Draw line for dx<0 and dy<0
// It means x0 > x1,  y0 > y1,  a < 0 and b > 0
void drawLine4(int x0, int x1, int y0, int y1, bool xy_interchange,  int color)
{
	bool ifoverflow = false;
	JudgeColor(color);
	int x;
	int y;
	// invert height because the opengl origin is at top-left instead of bottom-left
	int a = y1 - y0;
	int b = x0 -x1;
	x = x0;
	y = y0;
	if  (x >= draw_view_port[0][0] && x <= draw_view_port[2][0] && y >= draw_view_port[0][1] && y <= draw_view_port[2][1])  {
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
			// We choose SW
			if  (dint > 0)  {
				x--;
				y--;
				dint += IncSW;
			}
			// We choose W
			else  {
				x--;
				dint += IncW;
			}
			if  (x >= draw_view_port[0][0] && x <= draw_view_port[2][0] && y >= draw_view_port[0][1] && y <= draw_view_port[2][1])  {
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
			// We choose SW
			if  (dint <= 0)  {
				x--;
				y--;
				dint += IncSW;
			}
			// We choose S
			else  {
				y--;
				dint += IncS;
			}
			if  (x >= draw_view_port[0][0] && x <= draw_view_port[2][0] && y >= draw_view_port[0][1] && y <= draw_view_port[2][1])  {
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


// Display function
void displayFunc(void){
  // clear the entire window to the background color
  glClear(GL_COLOR_BUFFER_BIT); 
  glClearColor(0.0, 0.0, 0.0, 0.0); 
  ReadFile();
  glFlush();
}


void main(int ac, char** av) {
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

  // initialize OpenGL utility toolkit (glut)
  glutInit(&ac, av);

  // single disply and RGB color mapping
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); // set display mode
  glutInitWindowSize(winSizeX, winSizeY);      // set window size
  glutInitWindowPosition(0, 0);                // set window position on screen
  glutCreateWindow("Lab2 Window");       // set window title
  // displayFunc is called whenever there is a need to redisplay the window,
  // e.g., when the window is exposed from under another window or when the window is de-iconified
  glutDisplayFunc(displayFunc); // register the redraw function

  // set background color
  glClearColor(0.0, 0.0, 0.0, 0.0);     // set the background to black
  glClear(GL_COLOR_BUFFER_BIT); // clear the buffer

  // misc setup
  glMatrixMode(GL_PROJECTION);  // setup coordinate system
  glLoadIdentity();
  gluOrtho2D(0, winSizeX, 0, winSizeY);
  glShadeModel(GL_FLAT);
  glFlush();
  glutMainLoop();
}
