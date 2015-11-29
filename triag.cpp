/**
 *     Simple exemple of use of 3D OpenGL and IUP.
 *
 *  Creates a dialog with one canvas and draws a rotating cube in it.
 *
 *  Author:  Marcelo Gattass, Nov09, 2009.
 *
 * /
/*- Include lib interfaces: ANSI C, IUP and OpenGL ------*/
#include <cstdio>
#include <cstdlib>
#include <iup/iup.h>        /* IUP functions*/
#include <iup/iupgl.h>      /* IUP functions related to OpenGL (IupGLCanvasOpen,IupGLMakeCurrent and IupGLSwapBuffers) */


#include <GL/gl.h>     /* OpenGL functions */
#include <GL/glu.h>    /* OpenGL utility functions */

/*- Program context: -------------------------------------------------*/
static Ihandle *canvas;                    /* canvas handle */


struct camera {
	float eye[3];
	float at[3];
	float up[3];
	float fovy,nearp,farp;
	int   wp, hp;
} camera = { { 3.f, 3.f, 3.f, }, { 0, 0, 0 }, { 0, 0, 1 }, 30.f, 1.f, 15.f, 640, 480 };

struct triang {
	float v1[3], v2[3], v3[3];
} triang = { { 1, 0, 0 }, { 0, 1, 0 }, {0,0, 10 } };


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static int first[]={1,1,1,1,1};
static double fx[5],fy[5],fz[5],fw[5],fd[5];
static double sx[5],sy[5],sz[5],sw[5],sd[5];
double X, Y, Z, W;

void ClipAtPlane( double x, double y, double z, double w, int plane );

void SaveVertex(double x, double y, double z, double w){
	X = x;
	Y = y;
	Z = z;
	W = w;
}
double Distance (double x, double y, double z, double w, int plane )
{
	switch( plane )
	{
	case 0: return( -w - x );
	case 1: return( -w + x );
	case 2: return( -w - y );
	case 3: return( -w + y );
	case 4: return( -w - z );
	case 5: return( -w + z );
	}
	return( 0.0 );
}

#define LAST_PLANE 3

void Intersect(double x, double y, double z, double w,
		int plane, double d1, double d2)
{
	double t = d1 / (d1 - d2);
	double xi = sx[plane] + t * (x - sx[plane]);
	double yi = sy[plane] + t * (y - sy[plane]);
	double zi = sz[plane] + t * (z - sz[plane]);
	double wi = sw[plane] + t * (w - sw[plane]);

	if( plane == LAST_PLANE )
		SaveVertex( xi, yi, zi, wi );
	else
		ClipAtPlane( xi, yi,zi, wi, plane+1 );
}

void ClipAtPlane( double x, double y, double z, double w, int plane )
{
	double d = Distance(x, y, z, w, plane);

	/* Check whether it is the first point */
	if( first[plane] ) {
		fx[plane]=x; fy[plane]=y; fz[plane]=z; fw[plane]=w;
		fd[plane]=d; first[plane]=0;
	} else if ((sd[plane] < 0)^(d < 0))
		Intersect( x, y, z, w, plane, sd[plane], d );
	/* Save as previous */
	sx[plane]=x; sy[plane]=y; sz[plane]=z; sw[plane]=w; sd[plane]=d;

	/* Check whether it is a visible point */
	if ( d <= 0.0 ) {
		if ( plane == LAST_PLANE )
			SaveVertex( x, y, z, w );
		else
			ClipAtPlane( x, y, z, w, plane+1 );
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*------------------------------------------*/
/* Auxiliary functions to draw a color cube */
/*------------------------------------------*/

static void show(char* title, float m[16]){
	printf("%s\n", title);
	printf("| %.3f  %.3f %.3f  %.3f|\n", m[0], m[4], m[8], m[12]);
	printf("| %.3f  %.3f %.3f  %.3f|\n", m[1], m[5], m[9], m[13]);
	printf("| %.3f  %.3f %.3f  %.3f|\n", m[2], m[6], m[10], m[14]);
	printf("| %.3f  %.3f %.3f  %.3f|\n", m[3], m[7], m[11], m[15]);
	printf("\n");
}

static void cartesianFromNatural(float v[3], float v1[3], float v2[3], float v3[3], float L[3])
{
	v[0] = L[0] * v1[0] + L[1] * v2[0] + L[2] * v3[0];
	v[1] = L[0] * v1[1] + L[1] * v2[1] + L[2] * v3[1];
	v[2] = L[0] * v1[2] + L[1] * v2[2] + L[2] * v3[2];
}


void drawTriangle( ){
	glBegin(GL_POLYGON);
	glColor3f(1, 0, 0);
	glVertex3fv(triang.v1);
	glColor3f(0, 1, 0);
	glVertex3fv(triang.v2);
	glColor3f(0, 0, 1);
	glVertex3fv(triang.v3);
	glEnd();

}

/* function called when the canvas is exposed in the screen */
static int repaint_cb(Ihandle *self)
{
	float delta;
	float vesq[3], vdir[3];
	IupGLMakeCurrent(self);
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);  /* White */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	drawTriangle();

//	ClipAtPlane(0,0,1, 4, 3);
//	printf("%f %f %f %f\n", X, Y, Z, W);

	//	glColor3f(1, 1, 1);
	//	glBegin(GL_LINES);
	//	glVertex3f(0,0,0);
	//	glVertex3f(5,0,0);
	//	glEnd();
	//
	//	glColor3f(1, 1, 1);
	//	glBegin(GL_LINES);
	//	glVertex3f(0,0,0);
	//	glVertex3f(0,5,0);
	//	glEnd();
	//
	//	glColor3f(1, 1, 1);
	//	glBegin(GL_LINES);
	//	glVertex3f(0,0,0);
	//	glVertex3f(0,0,5);
	//	glEnd();

		glColor3f(0, 0, 0);
			glBegin(GL_POINTS);
			glVertex3d(0.83489895152010529,-0.00000000000000089 ,1.6510104847989284);
			glVertex3d(-0.00000000000000178,0.83489895152010574 ,1.6510104847989284);
		glEnd();

//		glColor3f(1, 1, 1);
//			glBegin(GL_LINES);
//			glVertex3d(0,0,0);
//			glVertex3d(-0.00000000000000178,0.83489895152010574 ,1.6510104847989284);
//		glEnd();
//
//		glColor3f(1, 1, 1);
//			glBegin(GL_LINES);
//			glVertex3d(0,0,0);
//			glVertex3d(0.83489895152010529,-0.00000000000000089 ,1.6510104847989284);
//		glEnd();

	for (delta = 0.2; delta < 1.f; delta += 0.2f){
		float Lesq[3] = { 1 - delta, 0, delta }, Ldir[3] = { 0, 1 - delta, delta };
		cartesianFromNatural(vesq, triang.v1, triang.v2, triang.v3, Lesq);
		cartesianFromNatural(vdir, triang.v1, triang.v2, triang.v3, Ldir);
		glColor3f(0, 0, 0);
		glBegin(GL_LINES);
		glVertex3fv(vesq);
		glVertex3fv(vdir);
		glEnd();
	}



	IupGLSwapBuffers(self);  /* change the back buffer with the front buffer */

	return IUP_DEFAULT; /* returns the control to the main loop */
}

/* function called in the event of changes in the width or in the height of the canvas */
static int resize_cb(Ihandle *self, int width, int height)
{
	float m[16], aspect = (1.0*camera.wp)/camera.hp;
	IupGLMakeCurrent(self);  /* Make the canvas current in OpenGL */

	/* define the entire canvas as the viewport  */
	glViewport(0, 0, width, height);

	/* transformation applied to each vertex */
	glMatrixMode(GL_MODELVIEW);

	glGetFloatv(GL_MODELVIEW_MATRIX, m);
	show("Model View", m);

	glLoadIdentity();           /* identity, i. e. no transformation */

	glGetFloatv(GL_MODELVIEW_MATRIX, m);
	show("Model View", m);

	gluLookAt(camera.eye[0], camera.eye[1], camera.eye[2],
			camera.at[0], camera.at[1], camera.at[2],
			camera.up[0], camera.up[1], camera.up[2]);

	glGetFloatv(GL_MODELVIEW_MATRIX, m);
	show("Model View", m);

	/* projection transformation (orthographic in the xy plane) */
	glMatrixMode(GL_PROJECTION);

	glGetFloatv(GL_PROJECTION_MATRIX, m);
	show("Projection", m);

	glLoadIdentity();

	glGetFloatv(GL_PROJECTION_MATRIX, m);
	show("Projection", m);

	gluPerspective(camera.fovy, aspect, camera.nearp, camera.farp);

	glGetFloatv(GL_PROJECTION_MATRIX, m);
	show("Projection", m);

	return IUP_DEFAULT; /* return to the IUP main loop */
}

static int click_cb(Ihandle* ih, int button, int pressed, int x, int y, char* status){
	printf("x=%d y=%dx\n", x, 480 - y);
	fflush(stdout);
}

int exit_cb(void)
{
	printf("Function to free memory and do finalizations...\n");
	return IUP_CLOSE;
}

/*-------------------------------------------------------------------------*/
/* Incializa o programa.                                                   */
/*-------------------------------------------------------------------------*/

Ihandle* initDialog(void)
{
	Ihandle* dialog;   /* dialog containing the canvas */

	canvas = IupGLCanvas(NULL); /* create _canvas */

	IupSetAttribute(canvas, "RASTERSIZE", "640x480");   /* define the initial size in pixels */
	IupSetAttribute(canvas, "BUFFER", "DOUBLE");      /* define that this OpenGL _canvas has double buffer (front and back) */

	/* bind callback actions with callback functions */
	IupSetCallback(canvas, "ACTION", (Icallback)repaint_cb);
	IupSetCallback(canvas, "RESIZE_CB", (Icallback)resize_cb);
	IupSetCallback(canvas, IUP_BUTTON_CB, (Icallback)click_cb);

	/* create the dialog and set its attributes */
	dialog = IupDialog(canvas);
	IupSetAttribute(dialog, "TITLE", "IUP 3D OpenGL");

	IupSetCallback(dialog, "CLOSE_CB", (Icallback)exit_cb);
	IupSetFunction("IDLE_ACTION", NULL);

	return dialog;
}

/*-----------------------*/
/* Main function.        */
/*-----------------------*/
int main(int argc, char* argv[])
{
	Ihandle* dialog;

	IupOpen(&argc, &argv);                       /* opens the IUP lib */
	IupGLCanvasOpen();                          /* enable the use of OpenGL to draw in canvas */

	dialog = initDialog();                      /* local function to create a dialog with buttons and canvas */
	IupShowXY(dialog, IUP_CENTER, IUP_CENTER);  /* shows dialog in the center of screen */

	IupMainLoop();                              /* handle the program control to the IUP lib until a return IUP_CLOSE */

	IupClose();                               /* closes the IUP lib */
}

