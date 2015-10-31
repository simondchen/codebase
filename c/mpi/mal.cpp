#include <stdlib.h>
#include <GL/glut.h> // include GLUT library header
#include <stdio.h>
#pragma hdrstop

int width=600, height=600; // window size
int windowID;

GLfloat minX = -2.2f, maxX = 0.8f, minY = -1.5f, maxY = 1.5; // complex plane boundaries
GLfloat stepX = (maxX - minX)/(GLfloat)width;
GLfloat stepY = (maxY - minY)/(GLfloat)height;

GLfloat black[] = {0.0f, 0.0f, 0.0f}; // black color
const int paletteSize = 128;
GLfloat palette[paletteSize][3];

int image[601][601];

const GLfloat radius = 4.0f;
bool fullScreen=false;

//****************************************
int calculateColor(GLfloat u, GLfloat v){
GLfloat re = u;
GLfloat im = v;
GLfloat tempRe=0.0;
int i;
for(i=0; i < paletteSize; i++){
tempRe = re*re - im*im + u;
im = re * im * 2 + v;
re = tempRe;
if( (re*re + im*im) > radius ){
	break;
}
}
return i;
}

void compute(){
for(GLfloat y = minY; y <=maxY; y += stepY){
for(GLfloat x = minX; x <=maxX; x += stepX){
//printf("%d\n",(GLint)((x-minX)/stepX));
//printf("%d\n",(GLint)((y-minY)/stepY));
image[(GLint)((x-minX)/stepX)][(GLint)((y-minY)/stepY)]=calculateColor(x,y);
}
}
}

void repaint() {// function called to repaint the window
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear the screen buffer
glBegin(GL_POINTS); // start drawing in single pixel mode
for(int i=0;i<=600;i++){
for(int j=0;j<=600;j++){
int depth=image[i][j];
if(depth==128)
{
glColor3fv(black);
}else{
glColor3fv(palette[depth]);
}
glVertex3f(i,j,0.0f);
}
}
glEnd(); // end drawing
glutSwapBuffers(); // swap the buffers - [ 2 ]
}
//****************************************
/*void repaint() {// function called to repaint the window
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear the screen buffer
glBegin(GL_POINTS); // start drawing in single pixel mode
for(GLfloat y = maxY; y > minY; y -= stepY){
for(GLfloat x = minX; x < maxX; x += stepX){
//glColor3fv(calculateColor(x,y)); // set color
//glVertex3f(x, y, 0.0f); // put pixel on screen (buffer) - [ 1 ]
//glVertex3f((x-minX)/stepX, (y-minY)/stepY, 0.0f); // put pixel on screen (buffer) - [ 1 ]
}
}
glEnd(); // end drawing
glutSwapBuffers(); // swap the buffers - [ 2 ]
}*/

//****************************************
void reshape (int w, int h){ // function called when window size is changed
stepX = (maxX-minX)/(GLfloat)w; // calculate new value of step along X axis
stepY = (maxY-minY)/(GLfloat)h; // calculate new value of step along Y axis
glViewport (0, 0, (GLsizei)w, (GLsizei)h); // set new dimension of viewable screen
glutPostRedisplay(); // repaint the window
}

//****************************************
void keyFunction(unsigned char key, int x, int y){ // function to handle key pressing
switch(key){
case 'F': // pressing F is turning on/off fullscreen mode
case 'f':
if(fullScreen){
glutReshapeWindow(width,height); // sets default window size
GLsizei windowX = (glutGet(GLUT_SCREEN_WIDTH)-width)/2;
GLsizei windowY = (glutGet(GLUT_SCREEN_HEIGHT)-height)/2;
glutPositionWindow(windowX, windowY); // centers window on the screen
fullScreen = false;
}
else{
fullScreen = true;
glutFullScreen(); // go to fullscreen mode
}
glutPostRedisplay();
break;
case 27 : // escape key - close the program
glutDestroyWindow(windowID);
exit(0);
break;
}
}

//****************************************
void createPalette(){
for(int i=0; i < 32; i++){
palette[i][0] = (8*i)/(GLfloat)255;
palette[i][1] = (128-4*i)/(GLfloat)255;
palette[i][2] = (255-8*i)/(GLfloat)255;
}
for(int i=0; i < 32; i++){
palette[32+i][0] = (GLfloat)1;
palette[32+i][1] = (8*i)/(GLfloat)255;
palette[32+i][2] = (GLfloat)0;
}
for(int i=0; i < 32; i++){
palette[64+i][0] = (128-4*i)/(GLfloat)255;
palette[64+i][1] = (GLfloat)1;
palette[64+i][2] = (8*i)/(GLfloat)255;
}
for(int i=0; i < 32; i++){
palette[96+i][0] = (GLfloat)0;
palette[96+i][1] = (255-8*i)/(GLfloat)255;
palette[96+i][2] = (8*i)/(GLfloat)255; 
}
}

//****************************************
int main(int argc, char** argv){
glutInit(&argc, argv);
createPalette();
glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
//居中放置
GLsizei windowX = (glutGet(GLUT_SCREEN_WIDTH)-width)/2;
GLsizei windowY = (glutGet(GLUT_SCREEN_HEIGHT)-height)/2;
printf("%d\n",glutGet(GLUT_SCREEN_WIDTH));
printf("%d\n",glutGet(GLUT_SCREEN_HEIGHT));
glutInitWindowPosition(windowX, windowY);
glutInitWindowSize(width, height);
windowID = glutCreateWindow("FRAKTALE");

glShadeModel(GL_SMOOTH);
glEnable(GL_DEPTH_TEST);
glViewport (0, 0, (GLsizei) width, (GLsizei) height);
glMatrixMode (GL_PROJECTION);
glLoadIdentity();
//glOrtho(minX, maxX, minY, maxY, ((GLfloat)-1), (GLfloat)1);
glOrtho(0, 600, 0, 600, ((GLfloat)-1), (GLfloat)1);

// set the event handling methods
compute();
glutDisplayFunc(repaint);
glutReshapeFunc(reshape);
glutKeyboardFunc(keyFunction);

glutMainLoop();

return 0;
}
