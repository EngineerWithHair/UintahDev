

#include <Packages/rtrt/Core/VolumeVisDpy.h>
#include <Packages/rtrt/Core/VolumeVis.h>
#include <Core/Thread/Thread.h>
#include <Core/Thread/Mutex.h>
#include <Core/Thread/Time.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <stdlib.h>
#include <Packages/rtrt/visinfo/visinfo.h>
#include <stdio.h>
#include <X11/keysym.h>
#include <algorithm>
#ifdef __GNUG__
#include <values.h>
#endif
#include "FontString.h"

using namespace rtrt;
using SCIRun::Mutex;
using SCIRun::Thread;
using namespace std;

namespace rtrt {
  extern Mutex xlock;
} // end namespace rtrt

static void printString(GLuint fontbase, double x, double y,
			const char *s, const Color& c);
static int calc_width(XFontStruct* font_struct, const char* str);
static void draw_circle(int radius, int x_center, int y_center);
static void circle_points(int x, int y, int x_center, int y_center);

VolumeVisDpy::VolumeVisDpy(Array1<Color> &matls, Array1<AlphaPos> &alphas,
			   int ncolors, float t_inc, char *in_file):
  hist(0), xres(500), yres(500), colors_index(matls), alpha_list(alphas),
  ncolors(ncolors), nalphas(ncolors),
  original_t_inc(0.01), current_t_inc(t_inc), t_inc(t_inc),
  in_file(in_file), data_min(MAXFLOAT), data_max(-MAXFLOAT)
{
  // need to allocate memory for alpha_transform and color_transform
  Array1<Color*> *c = new Array1<Color*>(ncolors);
  for(unsigned int i = 0; i < ncolors; i++)
    (*c)[i] = new Color();
  color_transform.set_results_ptr(c);
  alpha_transform.set_results_ptr(new Array1<float>(nalphas));
  alpha_stripes.set_results_ptr(new Array1<float>(nalphas));

  // create the colors for the color transform
  create_color_transfer();
  // rescale the alpha values
  //  current_t_inc = t_inc;
  // create the alphas for the alpha transform
  create_alpha_transfer();
}
  
VolumeVisDpy::~VolumeVisDpy()
{
  if (hist)
    delete[] hist;
}

void VolumeVisDpy::attach(VolumeVis *volume) {
  volumes.add(volume);

  // this needs to be done here, because we can't guarantee that setup_vars
  // will get called before VolumeVis starts cranking!
  data_min = min(data_min, volume->data_min);
  data_max = max(data_max, volume->data_max);
  scale = 1/(data_max - data_min);

  color_transform.scale(data_min,data_max);
  alpha_transform.scale(data_min,data_max);
}

void VolumeVisDpy::setup_vars() {
  //  cerr << "VolumeVisDpy::setup_vars:start\n";
}

void VolumeVisDpy::run() {
  //cerr << "VolumeVisDpy:run\n";
  xlock.lock();
  // Open an OpenGL window
  Display* dpy=XOpenDisplay(NULL);
  if(!dpy){
    cerr << "Cannot open display\n";
    Thread::exitAll(1);
  }
  int error, event;
  if ( !glXQueryExtension( dpy, &error, &event) ) {
    cerr << "GL extension NOT available!\n";
    XCloseDisplay(dpy);
    dpy=0;
    Thread::exitAll(1);
  }
  int screen=DefaultScreen(dpy);
  
  // sb - single buffered
  // db - double buffered
  char* criteria="db, max rgb";
  if(!visPixelFormat(criteria)){
    cerr << "Error setting pixel format for visinfo\n";
    cerr << "Syntax error in criteria: " << criteria << '\n';
    Thread::exitAll(1);
  }
  int nvinfo;
  XVisualInfo* vi=visGetGLXVisualInfo(dpy, screen, &nvinfo);
  if(!vi || nvinfo == 0){
    cerr << "Error matching OpenGL Visual: " << criteria << '\n';
    Thread::exitAll(1);
  }
  Colormap cmap = XCreateColormap(dpy, RootWindow(dpy, screen),
				  vi->visual, AllocNone);
  XSetWindowAttributes atts;
  int flags=CWColormap|CWEventMask|CWBackPixmap|CWBorderPixel;
  atts.background_pixmap = None;
  atts.border_pixmap = None;
  atts.border_pixel = 0;
  atts.colormap=cmap;
  atts.event_mask=StructureNotifyMask|ExposureMask|ButtonPressMask|ButtonReleaseMask|ButtonMotionMask|KeyPressMask|KeyReleaseMask;
  Window win=XCreateWindow(dpy, RootWindow(dpy, screen),
			   0, 0, xres, yres, 0, vi->depth,
			   InputOutput, vi->visual, flags, &atts);
  char* p="VolumeVis GUI";
  XTextProperty tp;
  XStringListToTextProperty(&p, 1, &tp);
  XSizeHints sh;
  sh.flags = USSize;
  XSetWMProperties(dpy, win, &tp, &tp, 0, 0, &sh, 0, 0);
  
  XMapWindow(dpy, win);
  
  GLXContext cx=glXCreateContext(dpy, vi, NULL, True);
  if(!glXMakeCurrent(dpy, win, cx)){
    cerr << "glXMakeCurrent failed!\n";
  }
  glShadeModel(GL_FLAT);
  for(;;){
    XEvent e;
    XNextEvent(dpy, &e);
    if(e.type == MapNotify)
      break;
  }
  XFontStruct* fontInfo = XLoadQueryFont(dpy, __FONTSTRING__);

  if (fontInfo == NULL) {
    cerr << "no font found\n";
    Thread::exitAll(1);
  }

  Font id = fontInfo->fid;
  unsigned int first = fontInfo->min_char_or_byte2;
  unsigned int last = fontInfo->max_char_or_byte2;
  GLuint fontbase = glGenLists((GLuint) last+1);
  if (fontbase == 0) {
    printf ("out of display lists\n");
    exit (0);
  }
  glXUseXFont(id, first, last-first+1, fontbase+first);
  xlock.unlock();
  

  setup_vars();
  bool need_hist=true;
  bool redraw=true;
  bool control_pressed=false;
  bool shift_pressed=false;

  int selected_point = -1;
  // these are used to keep the points from moving too much
  
  for(;;){
    //cerr << "VolumeVisDpy:run:eventloop\n";
    if(need_hist){
      need_hist=false;
      compute_hist(fontbase);
      redraw=true;
    }
    if(redraw){
      draw_hist(fontbase, fontInfo);
      draw_alpha_curve(fontbase, fontInfo);
      glFinish();
      glXSwapBuffers(dpy, win);
      XFlush(dpy);
      redraw=false;
    }
    // We should try to consume all the queued events before we redraw.
    // That way we don't waste time redrawing after each event
    while (XEventsQueued(dpy, QueuedAfterReading)) {
      XEvent e;
      XNextEvent(dpy, &e);	
      switch(e.type){
      case Expose:
	redraw=true;
	break;
      case ConfigureNotify:
	yres=e.xconfigure.height;
	if(e.xconfigure.width != xres){
	  xres=e.xconfigure.width;
	  need_hist=true;
	} else {
	  redraw=true;
	}
	break;
      case KeyPress:
	switch(XKeycodeToKeysym(dpy, e.xkey.keycode, 0)){
	case XK_Control_L:
	case XK_Control_R:
	  //cerr << "Pressed control\n";
	  control_pressed = true;
	  break;
	case XK_Shift_L:
	case XK_Shift_R:
	  //cerr << "Pressed shift\n";
	  shift_pressed = true;
	  break;
	case XK_Page_Up:
	case XK_plus:
	  rescale_alphas(current_t_inc/2);
	  cout << "current_t_inc = " << current_t_inc << endl;
	  break;
	case XK_Page_Down:
	case XK_minus:
	  rescale_alphas(current_t_inc*2);
	  cout << "current_t_inc = " << current_t_inc << endl;
	  break;
	case XK_w:
	case XK_W:
	  break;
	}
	break;
      case KeyRelease:
	switch(XKeycodeToKeysym(dpy, e.xkey.keycode, 0)){
	case XK_Control_L:
	case XK_Control_R:
	  control_pressed = false;
	  //cerr << "Releassed control\n";
	  break;
	case XK_Shift_L:
	case XK_Shift_R:
	  //cerr << "Releassed shift\n";
	  shift_pressed = false;
	}
	break;
      case ButtonRelease:
	switch(e.xbutton.button){
	case Button1:
	  {
	    create_alpha_transfer();
	    selected_point = -1;
	    redraw = true;
	  }
	  break;
	}
	break;
      case ButtonPress:
	{
	  int xpos = e.xbutton.x;
	  int ypos = yres - e.xbutton.y;
	
	  // check boundaries
	  int s=5;
	  int end=yres/2-5;
	  if (ypos < s || ypos > end)
	    break;
	  if (xpos < 5 || xpos > xres - 5)
	    break;
	  switch(e.xbutton.button){
	  case Button1:
	    if (shift_pressed) {
	      //cerr << "Left button pressed with shift\n";
	      selected_point = -1;
	      // create a point at the location of the click
	      AlphaPos new_guy((xpos-(float)5)/(xres-10),
			       (ypos-(float)5)/(yres/2-10));
	      int index = alpha_list.size();
	      alpha_list.grow(1,5);
	      while (new_guy.x < alpha_list[index-1].x) {
		alpha_list[index] = alpha_list[index-1];
		index--;
	      }
	      // now insert new_guy
	      alpha_list[index] = new_guy;
	      // make it selected for movement
	      selected_point = index;
	    } else if (control_pressed) {
	      //cerr << "Left button pressed with control\n";
	      selected_point = -1;
	      // find the point closest and delete it
	      // can't remove the end points
	      int index = select_point(xpos,ypos);
	      if (index > 0 && index < alpha_list.size()-1)
		alpha_list.remove(index);
	    } else {
	      // find the point closest and make it selected
	      int index = select_point(xpos,ypos);
	      if (index >= 0)
		selected_point = index;
	    }
	    break;
	  case Button2:
	    if (shift_pressed) {
	      //cerr << "Middle button pressed with shift\n";
	    } else if (control_pressed) {
	      //cerr << "Middle button pressed with control\n";
	    } else {
	    }
	    break;
	  case Button3:
	    if (shift_pressed) {
	      //cerr << "Right button pressed with shift\n";
	    } else if (control_pressed) {
	      //cerr << "Right button pressed with control\n";
	    } else {
	    }
	    break;
	  }
	}
	break;
      case MotionNotify:
	{
	  if (shift_pressed || control_pressed) break;
	  switch(e.xmotion.state&(Button1Mask|Button2Mask|Button3Mask)){
	  case Button1Mask:
	    {
	      if (selected_point < 0)
		// no point is selected, so don't do anything
		break;
	      int xpos = e.xbutton.x;
	      float xnorm;
	      if (xpos >= min_x && xpos < max_x)
		xnorm = (xpos - (float)5)/(xres-10);
	      else
		xnorm = alpha_list[selected_point].x;
	      int ypos = yres - e.xbutton.y;
	      float ynorm;
	      if (ypos < 5)
		ynorm = 0;
	      else if (ypos > (yres/2-5))
		ynorm = 1;
	      else
		ynorm = (ypos - (float)5)/(yres/2-10);
	    
	      alpha_list[selected_point] = AlphaPos(xnorm,ynorm);

	      redraw = true;
	    }
	    break;
	  case Button2Mask:
	  case Button3Mask:
	    break;
	  } // end switch(button mask with motion)
	}
	break;
      default:
	cerr << "Unknown event, type=" << e.type << '\n';
      } // end switch (e.type)
    } // end of while (there is a queued event)
  } // end of for(;;)
}

void VolumeVisDpy::compute_hist(GLuint fid) {
  //cerr << "VolumeVisDpy:compute_hist:start\n";
  glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
  glViewport(0, 0, xres, yres);
  glClearColor(0, 0, .2, 1);
  glClear(GL_COLOR_BUFFER_BIT);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0, 1, 0, 1);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  
  printString(fid, .1, .5, "Recomputing histogram...\n", Color(1,1,1));
  glFlush();

  int nhist=xres-10;
  //cerr << "VolumeVisDpy:compute_hist:xres = " << xres << "\n";
  // allocate and initialize the memory for the histogram
  if (hist)
    delete[] hist;
  hist=new int[nhist];
  for(int i=0;i<nhist;i++){
    hist[i]=0;
  }
  // loop over all the data and compute histograms
  for (int v = 0; v < volumes.size() ; v++) {
    VolumeVis* volume = volumes[v];
    // must index the data using three dimesions rather than as one, because
    // a BrickArray3 uses buffers on the ends to make the bricks all the same
    // size.
    for (int x = 0; x < volume->data.dim1(); x++) {
      for (int y = 0; y < volume->data.dim2(); y++) {
	for (int z = 0; z < volume->data.dim3(); z++) {
	  int idx=(int)((volume->data(x,y,z)-data_min) * scale * (nhist-1));
	  if (idx >= 0 && idx < nhist)
	    hist[idx]++;
	  //cerr << "data = " << volume->data(x,y,z) << ", data_min="<<data_min<<", data_max = "<<data_max<<", scale = "<<scale<<", idx=" << idx << '\n';
	}
      }
    }
  } // end loop for all volumes
  
  //cerr << "VolumeVisDpy:compute_hist:past compute histograms\n";
  // determine the maximum height for each histogram
  int* hp=hist;
  int max=0;
  for(int i=0;i<nhist;i++){
    if(*hp>max)
      max=*hp;
    hp++;
  }
  histmax=max;
  cout << "histmax = " << histmax << endl;
  //cerr << "VolumeVisDpy:compute_hist:end\n";
}

void VolumeVisDpy::draw_hist(GLuint fid, XFontStruct* font_struct) {
  int descent=font_struct->descent;
  int textheight=font_struct->descent+font_struct->ascent+2;

  glViewport(0, 0, xres, yres);
  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT);
  int nhist=xres-10;

  int s=yres/2; // start
  int e=yres; // end
  int h=e-s;
  glViewport(5, s, xres-10, e-s);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0, xres-10, -float(textheight)*histmax/(h-textheight), histmax);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
    
  ScalarTransform1D<int,Color*> stripes(color_transform.get_results_ptr());
  stripes.scale(0,nhist-1);
  glBegin(GL_LINES);
  for(int i=0;i<nhist;i++){
    Color *c=stripes.lookup(i);
    //    cout << "color[i="<<i<<"] = " << *c << endl;
    glColor3f(c->red(), c->green(), c->blue());
    glVertex2i(i, 0);
    glVertex2i(i, hist[i]);
  }
  glEnd();
      
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0, xres, 0, h);
  char buf[100];
  // print the min on the left
  sprintf(buf, "%g", data_min);
  printString(fid, 2, descent+1, buf, Color(1,1,1));
  // print the name in the middle
  int x = (int)((xres - calc_width(font_struct,"Data Histogram"))/2);
  printString(fid, x, descent+1, "Data Histogram", Color(1,1,1));
  // print the max on the right
  sprintf(buf, "%g", data_max);
  int w=calc_width(font_struct, buf);
  printString(fid, xres-2-w, descent+1, buf, Color(1,1,1));
  
  int errcode;
  while((errcode=glGetError()) != GL_NO_ERROR){
    cerr << "We got an error from GL: " << (char*)gluErrorString(errcode) << endl;
  }
  //cerr << "VolumeVisDpy:draw_hist:end\n";
}

// displays the alpha transfer curve as well as a representation of the colors
// and their corresponding opacities.  No alpha blending is used, because we
// are using a black background.  This allows us the ablility to just multiply
// the corresponding colors, by the alpha value.
void VolumeVisDpy::draw_alpha_curve(GLuint /*fid*/, XFontStruct* font_struct) {
  int descent=font_struct->descent;
  int textheight=font_struct->descent+font_struct->ascent+2;

  // set up the frame for the lower half of the user interface.
  int s=5;
  int e=yres/2-5;
  int h=e-s;
  int width = xres -10;
  glViewport(5, s, width, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0, width, 0, h);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  // draw the background
  // stripes allows us the oportunity to use an integer index based on the
  // width of the window to grab a color.
  ScalarTransform1D<int,Color*> stripes(color_transform.get_results_ptr());
  stripes.scale(0,width-1);
  alpha_stripes.scale(0,width-1);

  glBegin(GL_LINES);
  for(int i=0;i<width;i++){
    Color *c=stripes.lookup(i); // get the color
    float alpha = alpha_stripes.lookup(i); // get the alpha
    glColor3f(c->red()*alpha, c->green()*alpha, c->blue()*alpha);
    glVertex2i(i, 0);
    glVertex2i(i, h);
  }
  glEnd();
  
  // now draw the alpha curve
  glColor3f(1.0, 1.0, 1.0);
  glBegin(GL_LINE_STRIP);
  for(unsigned int j = 0; j < alpha_list.size(); j++) {
    //    cout << "drawing a point at ("<<alpha_list[j].x<<", "<<alpha_list[j].val<<")\n";
    glVertex2i((int)(alpha_list[j].x*width), (int)(alpha_list[j].val*h));
  }
  glEnd();

  glColor3f(1.0, 0.5, 1.0);
  int radius = (width/100)*3;
  for(unsigned int k = 0; k < alpha_list.size(); k++) {
    draw_circle(radius, (int)(alpha_list[k].x*width),
		(int)(alpha_list[k].val*h));
  }
}

void VolumeVisDpy::rescale_alphas(float new_t_inc) {
  // modify the alpha matrix by the t_inc
  // we are assuming the base delta_t is 1 and that the new delta_t is t_inc
  // the formula:
  //    a_1 : original opacity
  //    a_2 : resulting opacity
  //    d_1 : original sampling distance
  //    d_2 : new sampling distance
  // a_2 = 1 - (1 - a_1)^(d_2/d_1)
  float d2_div_d1 = new_t_inc/current_t_inc;
  for(unsigned int i = 0; i < alpha_transform.size(); i++) {
    alpha_transform[i] = 1 - powf(1 - alpha_transform[i], d2_div_d1);
    //    cout <<"alpha_transform[i="<<i<<"] = "<<alpha_transform[i]<<", ";
  }
  cout << endl;
  current_t_inc = new_t_inc;
}

// Preconditions:
//   assuming that the data in alpha_transform and alpha_stripes is already
//     allocated.
//   alpha_list.size() >= 2
//   alpha_list[0].x == 0
//   alpha_list[alpha_list.size()-1].x == 1

void VolumeVisDpy::create_alpha_transfer() {
  // the ratio of values as explained in rescale_alphas
  float d2_div_d1 = current_t_inc/original_t_inc;

  // loop over the alpha values and fill in the array
  int start = 0;
  int end;
  int nindex = alpha_transform.size() - 1;
  for (int a_index = 0; a_index < (alpha_list.size()-1); a_index++) {
    // the start has already been computed, so you need to figure out
    // the end.
    end = (int)(alpha_list[a_index+1].x * nindex);
    float val = alpha_list[a_index].val;
    float val_inc = (alpha_list[a_index+1].val - val) / (end - start);
    for (int i = start; i <= end; i++) {
      //    cout << "val = "<<val<<", ";
      alpha_stripes[i] = val;
      // apply the alpha scaling
      alpha_transform[i] = 1 - powf(1 - val, d2_div_d1);
      //    cout <<"alpha_transform[i="<<i<<"] = "<<alpha_transform[i]<<"\n";
      val += val_inc;
    }
    start = end;
  }
}

// finds the closest point to x,y in the alpha_list and returns the index
int VolumeVisDpy::select_point(int xpos, int ypos) {
  // need to figure out the normalized points
  float x = (xpos - (float)5)/(xres-10);
  float y = (ypos - (float)5)/(yres/2-10);
  //cout << "norm (x,y) = ("<<x<<", "<<y<<")\n";
  // now loop over the values and find the point closest
  float max_distance = FLT_MAX;
  int index = -1;
  for(unsigned int i = 0; i < alpha_list.size(); i++) {
    // we don't really care about the actuall distance, just the relative
    // distance, so we don't have to square root this value.
    float distance = (x - alpha_list[i].x) * (x - alpha_list[i].x) +
      (y - alpha_list[i].val) * (y - alpha_list[i].val);
    if (distance < max_distance) {
      // new close point
      max_distance = distance;
      index = i;
    }
  }
  // now to set the min_x and max_x;
  // if the selected point is either the first or last point they need to be
  // made so that they can't move left or right
  float min,max;
  if (index > 0 && index < alpha_list.size()-1) {
    // center point
    min = alpha_list[index-1].x;
    max = alpha_list[index+1].x;
  } else {
    // end point
    min = max = alpha_list[index].x;
  }
  min_x =(int)(min*(xres-10)+5);
  max_x =(int)(max*(xres-10)+5);
  //cout << "Closest to point "<<index<<".\n";
  return index;
}

// assuming that the data in color_transform already allocated
// This is really easy to compute.  All we do is take the color_index and
// interpolate the values.  We are assuming that each color is evenly spaced.
void VolumeVisDpy::create_color_transfer() {
  ScalarTransform1D<unsigned int,Color> c_index(&colors_index);
  c_index.scale(0,color_transform.size()-1);
  for(unsigned int i = 0; i < color_transform.size(); i++) {
    Color *c = color_transform[i];
    *c = c_index.interpolate(i);
  }
}

void VolumeVisDpy::animate(bool& changed) {
  if (current_t_inc != t_inc) {
    changed = true;
    t_inc = current_t_inc;
    cout << "t_inc now equals "<< t_inc<<endl;
  }
}

static void printString(GLuint fontbase, double x, double y,
			const char *s, const Color& c)
{
  glColor3f(c.red(), c.green(), c.blue());
  
  glRasterPos2d(x,y);
  /*glBitmap(0, 0, x, y, 1, 1, 0);*/
  glPushAttrib (GL_LIST_BIT);
  glListBase(fontbase);
  glCallLists((int)strlen(s), GL_UNSIGNED_BYTE, (GLubyte *)s);
  glPopAttrib ();
}

static int calc_width(XFontStruct* font_struct, const char* str)
{
  XCharStruct overall;
  int ascent, descent;
  int dir;
  XTextExtents(font_struct, str, (int)strlen(str), &dir, &ascent, &descent, &overall);
  return overall.width;
}

/*
  Midpoint Circle Algorithm found in Introduction to Computer Graphics

 */
void draw_circle(int radius, int x_center, int y_center) {
  int x,y,d,deltaE,deltaSE;
  x = 0;
  y = radius;
  d = 1 - radius;
  deltaE = 3;
  deltaSE = 5 - radius * 2;
  glBegin(GL_POINTS);
  circle_points(x,y,x_center,y_center);
  while (y > x) {
    if (d < 0) {
      d += deltaE;
      deltaE += 2;
      deltaSE += 2;
      x++;
    } else {
      d += deltaSE;
      deltaE += 2;
      deltaSE += 4;
      x++;
      y--;
    }
    circle_points(x,y,x_center,y_center);
  }
  glEnd();
}

/*
  found in Introduction to Computer Graphics
*/
void circle_points(int x, int y, int x_center, int y_center) {
  glVertex2i(x_center+x,y_center+y);
  glVertex2i(x_center+y,y_center+x);
  glVertex2i(x_center+y,y_center-x);
  glVertex2i(x_center+x,y_center-y);
  glVertex2i(x_center-x,y_center-y);
  glVertex2i(x_center-y,y_center-x);
  glVertex2i(x_center-y,y_center+x);
  glVertex2i(x_center-x,y_center+y);
}

