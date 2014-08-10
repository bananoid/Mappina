#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class ModMapApp : public AppNative {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
};

void ModMapApp::setup()
{
}

void ModMapApp::mouseDown( MouseEvent event )
{
}

void ModMapApp::update()
{
}

void ModMapApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP_NATIVE( ModMapApp, RendererGl )
