/*
Niniejszy program jest wolnym oprogramowaniem; możesz go
rozprowadzać dalej i / lub modyfikować na warunkach Powszechnej
Licencji Publicznej GNU, wydanej przez Fundację Wolnego
Oprogramowania - według wersji 2 tej Licencji lub(według twojego
wyboru) którejś z późniejszych wersji.

Niniejszy program rozpowszechniany jest z nadzieją, iż będzie on
użyteczny - jednak BEZ JAKIEJKOLWIEK GWARANCJI, nawet domyślnej
gwarancji PRZYDATNOŚCI HANDLOWEJ albo PRZYDATNOŚCI DO OKREŚLONYCH
ZASTOSOWAŃ.W celu uzyskania bliższych informacji sięgnij do
Powszechnej Licencji Publicznej GNU.

Z pewnością wraz z niniejszym programem otrzymałeś też egzemplarz
Powszechnej Licencji Publicznej GNU(GNU General Public License);
jeśli nie - napisz do Free Software Foundation, Inc., 59 Temple
Place, Fifth Floor, Boston, MA  02110 - 1301  USA
*/

#define GLM_FORCE_RADIANS

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdlib.h>
#include <stdio.h>
#include "constants.h"
//#include "allmodels.h"
#include "lodepng.h"
#include "shaderprogram.h"
#include "myCube.h"
#include <iostream>
#include <string>

float speed_x = 0;
float speed_y = 0;
bool minus = 0;

GLuint tex0;
GLuint tex1;
GLuint tex2;

ShaderProgram* sp;


float* vertices = myCubeVertices;
float* normals = myCubeNormals;
float* texCoords = myCubeTexCoords;
float* colors = myCubeColors;
int vertexCount = myCubeVertexCount;

//Procedura obsługi błędów
void error_callback(int error, const char* description) {
	fputs(description, stderr);
}
int variables[4] = { 0,0,0,0};
int currentVar = 0;// co kazde wywolanie keycallback czyli wcisniecie jakiegokolwiek przycisku zwieksz o 1 i % 4
float x_val[32] = { 0 };
float z_val[32] = { 0 };
float heights[8] = { 1.5,1,1.25,1.8,2.1,1.25,1,1.5 };

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS) {
		currentVar = currentVar % 4;
		if (key == GLFW_KEY_1) { if (minus && currentVar >= 2 && currentVar <= 3) { variables[currentVar] = -1; } else { variables[currentVar] = 1; } minus = 0; }
		if (key == GLFW_KEY_2) { if (minus && currentVar >= 2 && currentVar <= 3) { variables[currentVar] = -2; } else { variables[currentVar] = 2; } minus = 0; }
		if (key == GLFW_KEY_3) { if (minus && currentVar >= 2 && currentVar <= 3) { variables[currentVar] = -3; } else { variables[currentVar] = 3; } minus = 0; }
		if (key == GLFW_KEY_4) { if (minus && currentVar >= 2 && currentVar <= 3) { variables[currentVar] = -4; } else { variables[currentVar] = 4; } minus = 0; }
		if (key == GLFW_KEY_5) { if (minus && currentVar >= 2 && currentVar <= 3) { variables[currentVar] = -5; } else { variables[currentVar] = 5; } minus = 0; }
		if (key == GLFW_KEY_6) { if (minus && currentVar >= 2 && currentVar <= 3) { variables[currentVar] = -6; } else { variables[currentVar] = 6; } minus = 0; }
		if (key == GLFW_KEY_7) { if (minus && currentVar >= 2 && currentVar <= 3) { variables[currentVar] = -7; } else { variables[currentVar] = 7; } minus = 0; }
		if (key == GLFW_KEY_8) { if (minus && currentVar >= 2 && currentVar <= 3) { variables[currentVar] = -8; } else { variables[currentVar] = 8; } minus = 0; }
		if (key == GLFW_KEY_9)	variables[currentVar] = 9;
		if (key == GLFW_KEY_0)	variables[currentVar] = 0;
		if (key == GLFW_KEY_MINUS) minus=1;
		if (key == GLFW_KEY_LEFT) speed_x = -PI / 2;
		if (key == GLFW_KEY_RIGHT) speed_x = PI / 2;
		if (key == GLFW_KEY_UP) speed_y = PI / 2;
		if (key == GLFW_KEY_DOWN) speed_y = -PI / 2;
		std::cout << "Aktualna wartosc:" << variables[currentVar] << " currentvalue"<< currentVar << std::endl;

		}
	
	if (action == GLFW_RELEASE) {
		currentVar = currentVar % 4;
		if (key == GLFW_KEY_1) currentVar++;
		if (key == GLFW_KEY_2) currentVar++;
		if (key == GLFW_KEY_3) currentVar++;
		if (key == GLFW_KEY_4) currentVar++;
		if (key == GLFW_KEY_5) currentVar++;
		if (key == GLFW_KEY_6) currentVar++;
		if (key == GLFW_KEY_7) currentVar++;
		if (key == GLFW_KEY_8) currentVar++;
		if (key == GLFW_KEY_9)	currentVar++;
		if (key == GLFW_KEY_0)	currentVar++;
		if (key == GLFW_KEY_LEFT) speed_x = 0;
		if (key == GLFW_KEY_RIGHT) speed_x = 0;
		if (key == GLFW_KEY_UP) speed_y = 0;
		if (key == GLFW_KEY_DOWN) speed_y = 0;
		currentVar = currentVar % 4;
	}		
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		double xPos, yPos;
		glfwGetCursorPos(window, &xPos, &yPos);
		std::cout << "Clicked at position: " << xPos << ", " << yPos << std::endl;
	}
}

GLuint readTexture(const char* filename) {
	GLuint tex;

	glActiveTexture(GL_TEXTURE0);

	//Wczytanie do pamięci komputera
	std::vector<unsigned char> image;   //Alokuj wektor do wczytania obrazka
	unsigned width, height;   //Zmienne do których wczytamy wymiary obrazka
	//Wczytaj obrazek
	unsigned error = lodepng::decode(image, width, height, filename);
	//std::cout << filename;
	if (error) {
		printf("wystapil blad");
		exit(0);
	}
	//Import do pamięci karty graficznej
	glGenTextures(1, &tex); //Zainicjuj jeden uchwyt
	glBindTexture(GL_TEXTURE_2D, tex); //Uaktywnij uchwyt
	//Wczytaj obrazek do pamięci KG skojarzonej z uchwytem
	glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*)image.data());
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	return tex;
}


//Procedura inicjująca
void initOpenGLProgram(GLFWwindow* window) {
	//initShaders();
	//************Tutaj umieszczaj kod, który należy wykonać raz, na początku programu************
	glClearColor(0, 0, 0, 1);
	glEnable(GL_DEPTH_TEST);
	glfwSetKeyCallback(window, keyCallback);
	tex0 = readTexture("szachownica.png");
	tex1 = readTexture("nieb.png");
	tex2 = readTexture("red1.png");

	sp = new ShaderProgram("v_simplest.glsl", NULL, "f_simplest.glsl");
}

float calculate_move(int start,int end) {
	float move = end - start;
	return 4*move;
}

float calculate_moveV2(int move) {
	return float(4 * move);
}

void move(int i) {
	//figure = glm::translate(figure, glm::vec3(calculate_move(start_x,end_x), 1.0f, calculate_move(start_z, end_z)));
	x_val[i] -= calculate_moveV2(variables[2]);
	z_val[i] += calculate_moveV2(variables[3]);
}


void createObject(glm::mat4 P, glm::mat4 V, glm::mat4 M, GLuint texture) {
	//Przeslij parametry programu cieniującego do karty graficznej
	glUniformMatrix4fv(sp->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(sp->u("V"), 1, false, glm::value_ptr(V));
	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M));
	
	//glUniform4f(spLambertTextured->u("lp"), 0, 0, -6, 1);

	glEnableVertexAttribArray(sp->a("vertex"));  //Włącz przesyłanie danych do atrybutu vertex
	glVertexAttribPointer(sp->a("vertex"), 4, GL_FLOAT, false, 0, vertices); //Wskaż tablicę z danymi dla atrybutu vertex

	//glEnableVertexAttribArray(sp->a("color"));  //Włącz przesyłanie danych do atrybutu color
	//glVertexAttribPointer(sp->a("color"), 4, GL_FLOAT, false, 0, colors); //Wskaż tablicę z danymi dla atrybutu color

	glEnableVertexAttribArray(sp->a("texCoord0"));
	glVertexAttribPointer(sp->a("texCoord0"), 2, GL_FLOAT, false, 0, texCoords);

	glEnableVertexAttribArray(sp->a("normal"));  //Włącz przesyłanie danych do atrybutu normal
	glVertexAttribPointer(sp->a("normal"), 4, GL_FLOAT, false, 0, normals); //Wskaż tablicę z danymi dla atrybutu normal

	//Współrzędne teksturowania bierz z tablicy myCubeTexCoords
	glUniform1i(sp->u("textureMap0"), 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	glDrawArrays(GL_TRIANGLES, 0, vertexCount);//Narysuj obiekt

	glDisableVertexAttribArray(sp->a("vertex"));   //Wyłącz przesyłanie danych do atrybutu vertex
	glDisableVertexAttribArray(sp->a("texCoord0"));
	//glDisableVertexAttribArray(sp->a("color"));  //Wyłącz przesyłanie danych do atrybutu color
	glDisableVertexAttribArray(sp->a("normal"));
}

//Zwolnienie zasobów zajętych przez program
void freeOpenGLProgram(GLFWwindow* window) {
	//freeShaders();
	glDeleteTextures(1, &tex0);
	glDeleteTextures(1, &tex1);
	glDeleteTextures(1, &tex2);

	//************Tutaj umieszczaj kod, który należy wykonać po zakończeniu pętli głównej************
}

//Procedura rysująca zawartość sceny
void drawScene(GLFWwindow* window, float angle_x, float angle_y) {
	//************Tutaj umieszczaj kod rysujący obraz******************l
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 V = glm::lookAt(
	glm::vec3(0.0f, 2.0f, -20.0f),
	glm::vec3(0.0f, 0.0f, 0.0f),
	glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 P = glm::perspective(50.0f * PI / 180.0f, 1.0f, 0.01f, 50.0f);


	glm::mat4 M = glm::mat4(1.0f);
	glm::mat4 Decha = M;
	Decha = glm::rotate(Decha, angle_y, glm::vec3(1.0f, 0.0f, 0.0f));
	Decha = glm::rotate(Decha, angle_x, glm::vec3(0.0f, 1.0f, 0.0f));
	
	float z = -14.0f;
	glm::mat4 Figura[32];
	int x = 14;
	for (int i = 0; i <= 7; i++) {
		Figura[i] = Decha;
		Figura[i]=glm::scale(Figura[i], glm::vec3(0.5f, heights[i], 0.5f));
	
		Figura[i] = glm::translate(Figura[i], glm::vec3(float(x), 1.0f, z));
		x -= 4;			
	}
	z += 4.0f;
	x = 14;
	for (int i = 8; i <= 15; i++) {
		Figura[i] = Decha;
		Figura[i] = glm::scale(Figura[i], glm::vec3(0.5f, 0.7f, 0.5f));

		Figura[i] = glm::translate(Figura[i], glm::vec3(float(x), 1.0f, z));
		x -= 4;
	}
	z += 20.0f;
	x = 14;
	for (int i = 16; i <= 23; i++) {
		Figura[i] = Decha;
		Figura[i] = glm::scale(Figura[i], glm::vec3(0.5f, 0.7f, 0.5f));
		
		Figura[i] = glm::translate(Figura[i], glm::vec3(float(x), 1.0f, z));
		x -= 4;
	}
	z += 4.0f;
	x = 14;
	for (int i = 24; i <= 31; i++) {
		Figura[i] = Decha;
		Figura[i] = glm::scale(Figura[i], glm::vec3(0.5f, heights[i-24], 0.5f));

		Figura[i] = glm::translate(Figura[i], glm::vec3(float(x), 1.0f, z));
		x -= 4;	
	}
	//["BWieza1", "BSkok1", "BGon1", "BKrol", "BHet", "BGon2", "BSkok2", "BWieza2"]
	//String tabpomFigura[32] = { "BPion1", "BPion2", "BPion3", "BPion4", "BPion5", "BPion6", "BPion7", "BPion8","BWieza1", "BSkok1", "BGon1", "BKrol", "BHet", "BGon2", "BSkok2", "BWieza2","CPion1", CPion2, CPion3, CPion4, CPion5, CPion6, CPion7, CPion8,CWieza1, CSkok1, CGon1, CKrol, CHet, CGon2, CSkok2, CWieza2};
		//[CPion1, CPion2, CPion3, CPion4, CPion5, CPion6, CPion7, CPion8] [CWieza1, CSkok1, CGon1, CKrol, CHet, CGon2, CSkok2, CWieza2]
	//[BPion1, BPion2, BPion3, BPion4, BPion5, BPion6, BPion7, BPion8] [BWieza1, BSkok1, BGon1, BKrol, BHet, BGon2, BSkok2, BWieza2]
	//[CPion1, CPion2, CPion3, CPion4, CPion5, CPion6, CPion7, CPion8] [CWieza1, CSkok1, CGon1, CKrol, CHet, CGon2, CSkok2, CWieza2]
	
	
	for (int i = 0; i <= 31; i++) {
		Figura[i] = glm::translate(Figura[i], glm::vec3(x_val[i], 0.0f,z_val[i]));	
	}

	Decha = glm::scale(Decha, glm::vec3(8.0f, 0.1f, 8.0f));


	sp->use();

	createObject(P,V,Decha,tex0);
	for (int i = 0; i <= 15; i++) {
		createObject(P, V, Figura[i],tex1);
	}
	for (int i = 16; i <= 31; i++) {
		createObject(P, V, Figura[i], tex2);
	}

	glfwSwapBuffers(window); //Przerzuć tylny bufor na przedni
}


int main(void)
{
	GLFWwindow* window; //Wskaźnik na obiekt reprezentujący okno

	glfwSetErrorCallback(error_callback);//Zarejestruj procedurę obsługi błędów

	if (!glfwInit()) { //Zainicjuj bibliotekę GLFW
		fprintf(stderr, "Nie można zainicjować GLFW.\n");
		exit(EXIT_FAILURE);
	}

	window = glfwCreateWindow(800, 800, "OpenGL", NULL, NULL);  //Utwórz okno 500x500 o tytule "OpenGL" i kontekst OpenGL.

	if (!window) //Jeżeli okna nie udało się utworzyć, to zamknij program
	{
		fprintf(stderr, "Nie można utworzyć okna.\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window); //Od tego momentu kontekst okna staje się aktywny i polecenia OpenGL będą dotyczyć właśnie jego.
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetKeyCallback(window, keyCallback);
	glfwSwapInterval(1); //Czekaj na 1 powrót plamki przed pokazaniem ukrytego bufora

	if (glewInit() != GLEW_OK) { //Zainicjuj bibliotekę GLEW
		fprintf(stderr, "Nie można zainicjować GLEW.\n");
		exit(EXIT_FAILURE);
	}

	initOpenGLProgram(window); //Operacje inicjujące

	float angle_x = 0; //Aktualny kąt obrotu obiektu
	float angle_y = 0; //Aktualny kąt obrotu obiektu

	glfwSetTime(0); //Wyzeruj timer
	int a;
	//Główna pętla
	while (!glfwWindowShouldClose(window)) //Tak długo jak okno nie powinno zostać zamknięte
	{
		angle_x += speed_x * glfwGetTime(); //Oblicz przyrost kąta po obrocie
		angle_y += speed_y * glfwGetTime(); //Oblicz przyrost kąta po obrocie
		
		glfwSetTime(0); //Wyzeruj timer
		if (variables[3] != 0) {
			move(variables[0]*10+variables[1]);
			for (int i = 0; i <= 3; i++) {
				variables[i] = 0;
			}
		}
		drawScene(window, angle_x, angle_y); //Wykonaj procedurę rysującą
		glfwPollEvents(); //Wykonaj procedury callback w zalezności od zdarzeń jakie zaszły.
		
	}

	freeOpenGLProgram(window);

	glfwDestroyWindow(window); //Usuń kontekst OpenGL i okno
	glfwTerminate(); //Zwolnij zasoby zajęte przez GLFW
	exit(EXIT_SUCCESS);
}
