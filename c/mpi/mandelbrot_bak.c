#include <GL/glut.h>
#include <stdio.h> 
#include <stdlib.h>
#include <mpi.h>
#pragma harstop

const int depth=128;
int radius=4;
int image[600][600];
int size=6;
int width=600,height=600;
GLfloat minX=-2.2f,maxX=0.8f,minY=-1.5f,maxY=1.5f;
GLfloat stepX=(maxX-minX)/(GLfloat)width;
GLfloat stepY=(maxY-minY)/(GLfloat)height;
GLfloat black[]={0.0f,0.0f,0.0f};
GLfloat palette[depth][3];

int caldepth(GLfloat u,GLfloat v){
	GLfloat re=u;	
	GLfloat im=v; GLfloat tempRe=0.0;
	int i;	
	for(i=0;i<depth;i++){
		tempRe=re*re-im*im+u;
		im=2*re*im+v;
		re=tempRe;
		if((re*re+im*im)>radius)
			break;
	}
	return i;
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

void repaint() {// function called to repaint the window
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear the screen buffer
glBegin(GL_POINTS); // start drawing in single pixel mode
for(int i=0;i<600;i++){
	for(int j=0;j<600;j++){
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

int main(int argc,char **argv){
	int nproc;
	int rank;
	MPI_Status status;
	int err;
	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD,&nproc);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	printf("%d\n",nproc);
	if(nproc!=7){
		printf("7 processes needed\n");
		MPI_Abort(MPI_COMM_WORLD,err);
	}
	if(rank==0){
		int i;
		int windowID;
		for(i=1;i<nproc;i++){
			MPI_Recv(image[(i-1)*100],100*600,MPI_INT,i,0,MPI_COMM_WORLD,&status);
			printf("received message from proc %d\n",i);
		}
		glutInit(&argc,argv);
		createPalette();
		glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		//居中放置
		GLsizei windowX = (glutGet(GLUT_SCREEN_WIDTH)-width)/2;
		GLsizei windowY = (glutGet(GLUT_SCREEN_HEIGHT)-height)/2;
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
		glutDisplayFunc(repaint);
		glutMainLoop();
	}else{
		int limage[100][600];
		int i,j;
		int k;
		for(j=0;j<100;j++){
			k=(rank-1)*100+j;
			for(i=0;i<600;i++){
				GLfloat x=minX+i*stepX;
				GLfloat y=minY+k*stepY;
				limage[j][i]=caldepth(x,y);
			}
		}
		MPI_Send(limage,100*600,MPI_INT,0,0,MPI_COMM_WORLD);
	}
	MPI_Finalize();
}
