/****************************************************
 * File: GraphVisualizer.cpp (Linux Version)
 * Author: Keith Schwarz (htiek@cs.stanford.edu)
 *
 * Implementation of the graph visualizer module using
 * OpenGL and the GLUT framework.  This code contains
 * the main program entry point, which fires off the
 * user's code in a separate thread and continuously
 * polls to repaint the window.  This is not the most
 * elegant implementation, but it works well enough
 * for this application.
 */

#include "GraphVisualizer.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <limits>
#include <algorithm>
#include <pthread.h>

/* Forward-declare the user's "main". */
extern int main();

/* Constant defining the initial window size. */
const size_t kInitialWindowSize = 400;

/* Our cached copy of the graph. */
static SimpleGraph gGraph;

/* Our lock on that graph. */
static pthread_mutex_t gGraphMutex;

/* A flag indicating whether we should update the display. */
static bool gShouldUpdate = false;

/* A constant controlling padding on the main window. */
const double kPad = 0.05;

/* A constant controlling the number of msecs during which we should poll for
 * redraw requests when there are no known draw requests.
 */
const unsigned int kIdleMsecDelay = 50;

/* A constant controlling the number of msecs during which we should poll for
 * redraw requests when there was a draw request.
 */
const unsigned int kActiveMsecDelay = 1;

/* Computes the perpspective by making the bounding box the size of the
 * graph.  This function can only be called when we have an exclusive lock
 * on the graph.
 */
static void ComputePerspective() {
  double minX, maxX, minY, maxY;
  minX = minY = numeric_limits<double>::infinity();
  maxX = maxY = -numeric_limits<double>::infinity();

  /* Find the bounding box. */
  for (size_t i = 0; i < gGraph.nodes.size(); ++i) {
    minX = min(minX, gGraph.nodes[i].x);
    minY = min(minY, gGraph.nodes[i].y);
    maxX = max(maxX, gGraph.nodes[i].x);
    maxY = max(maxY, gGraph.nodes[i].y);
  }

  /* We want to scale everything out by some factor so that we have a padded
   * view of everything.  To do so, we'll:
   *
   * 1. Compute the center of the two, (max - min) / 2 + min.
   * 2. Compute the distance between the two, max - min.
   * 3. Scale the distance by (1 + 2k) to get a new distance of (1 + 2k)(max - min)
   * 4. Subtract half of this out from the center to get the new min.
   * 5. Add half of this to the center to get the new max.
   */
  const double xSpread = maxX - minX;
  const double ySpread = maxY - minY;

  const double xMid = xSpread / 2 + minX;
  const double yMid = ySpread / 2 + minY;

  minX = xMid - (1 + 2 * kPad) * xSpread / 2;
  maxX = xMid + (1 + 2 * kPad) * xSpread / 2;

  minY = yMid - (1 + 2 * kPad) * ySpread / 2;
  maxY = yMid + (1 + 2 * kPad) * ySpread / 2;

  /* Reset back to the default projection matrix. */
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  /* Update the display appropriately. */
  gluOrtho2D(minX, maxX, minY, maxY);
}

/* Draws all the arcs in the graph.  The graph should be locked on entry
 * to this call.
 */
static void DrawArcs() {
  /* Make everything visible */
  glColor3f(0.0f, 0.0f, -.0f);

  glBegin(GL_LINES); {
    /* Iterate over the arcs and render each of them. */
    for (size_t i = 0; i < gGraph.edges.size(); ++i) {
      glVertex2f(gGraph.nodes[gGraph.edges[i].start].x, 
                 gGraph.nodes[gGraph.edges[i].start].y);
      glVertex2f(gGraph.nodes[gGraph.edges[i].end].x, 
                 gGraph.nodes[gGraph.edges[i].end].y);
    }
  } glEnd();
}

/* Draws all the nodes in the graph.  The graph should be locked on entry
 * to this call.
 */
static void DrawNodes() {
  /* Make everything a nice blue color. */
  glColor3f(0.0f, 0.0f, 1.0f);
  glPointSize(5.0);

  glBegin(GL_POINTS); {
    /* Iterate over the nodes and render each of them. */
    for (size_t i = 0; i < gGraph.nodes.size(); ++i)
      glVertex2f(gGraph.nodes[i].x, gGraph.nodes[i].y);
  } glEnd();
}

/* Called when we need to draw something. */
static void OnDisplayWindow() {
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  /* Grab a lock on the graph so we don't end up drawing during
   * a graph update.
   */
  pthread_mutex_lock(&gGraphMutex); {
  
    /* Set the perspective so that we can see everything. */
    ComputePerspective();
    
    /* Draw all of the arcs in the graph. */
    DrawArcs();
    
    /* Draw all of the nodes in the graph. */
    DrawNodes();
  } pthread_mutex_unlock(&gGraphMutex);
  
  glutSwapBuffers();
}

static void OnResizeWindow(int width, int height) {
  /* Use the whole window as the viewport. */
  glViewport(0, 0, width, height);
}

/* When idling, block until someone tells us to redraw. */
static void OnTimer(int) {
  bool shouldUpdate;

  /* Look up whether to update or not. */
  pthread_mutex_lock(&gGraphMutex); {
    shouldUpdate = gShouldUpdate;
    gShouldUpdate = false;
  } pthread_mutex_unlock(&gGraphMutex);

  /* If so, redraw everything. */
  if (shouldUpdate)
    glutPostRedisplay(); 

  /* Fire this event again in a while if nothing updated, and quite soon
   * otherwise.
   */
  glutTimerFunc(shouldUpdate? kActiveMsecDelay : kIdleMsecDelay, OnTimer, 0);
}

/* Lock everything, update the graph, and post a redraw. */
void DrawGraph(SimpleGraph& g) {
  pthread_mutex_lock(&gGraphMutex); {
    gGraph = g;
    
    /* Flag that we're ready. */
    gShouldUpdate = true;
  } pthread_mutex_unlock(&gGraphMutex);
}

/* Main thread routine.  This calls into the student code, then calls exit
 * when the function returns.
 */
void* ThreadRoutine(void*) {
  exit(main());

  /* Never reached; silences compiler warnings. */
  return (void*)0;
}

/* This function has no implementation in this version.  It's only required
 * in the Mac/Win32 versions.
 */
void InitGraphVisualizer() {
  // Empty
}

/* Undefine main so that this ends up being the real main. */
#undef main

/* Sets up the GLUT window in a separate thread. */
int main(int argc, char** argv) {
  /* Set up the mutex we'll be using for the application. */
  pthread_mutex_init(&gGraphMutex, NULL);

  /* Set up GLUT. */
  glutInit(&argc, argv);

  /* Configure the display mode to be single-buffered and use an RGB color
   * space.
   */
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

  /* Create the main window. */
  glutCreateWindow("CS106L graphviz");

  /* Pick a nice window size. */
  glutInitWindowSize(kInitialWindowSize, kInitialWindowSize);

  /* Position the window at the origin. */
  glutInitWindowPosition(0, 0);

  /* Create the window and wire in the display function. */
  glutDisplayFunc(OnDisplayWindow);

  /* Wire in the resize function. */
  glutReshapeFunc(OnResizeWindow);

  /* Wire in the code to execute while idle (i.e. the repainter) */
  glutTimerFunc(kIdleMsecDelay, OnTimer, 0);

  /* Fire off the main loop in a separate thread. */
  pthread_t remoteThread;
  pthread_create(&remoteThread, NULL, ThreadRoutine, NULL);

  /* Run the GLUT loop. */
  glutMainLoop();

  /* Control never reaches here. */
}
