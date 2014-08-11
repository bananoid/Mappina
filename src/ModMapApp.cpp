#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/Utilities.h"
#include "cinder/Font.h"
#include "cinder/Camera.h"

#include "cinderSyphon.h"
#include "OscListener.h"

#define WIDTH 1920
#define HEIGHT 1080

using namespace ci;
using namespace ci::app;
using namespace std;

class ModMapApp : public AppNative {
  public:
	void setup();
	void mouseDown( MouseEvent event );
	void keyDown( KeyEvent event );
	void update();
	void draw();
  void prepareSettings(cinder::app::AppBasic::Settings *settings);
  
  osc::Listener mOSCListener;
  
  syphonServer mSyphonOutA;
  Font mFont;
  float mOSCParam_speed;
  float mRotation;
  
  CameraPersp mCam;
  Vec3f mEye;
  Vec3f mCenter;
  Vec3f mUp;
};


void ModMapApp::prepareSettings(cinder::app::AppBasic::Settings *settings){
  settings->setWindowSize(WIDTH*.5,HEIGHT*.5);
	settings->setFrameRate(60.f);
//  settings->enableHighDensityDisplay();
}

void ModMapApp::setup()
{
  //Camera
  mCam.setPerspective( 60.0f, getWindowAspectRatio(), 5.0f, 3000.0f );
  mEye = Vec3f( 0.0f, 0.0f, 500.0f );
  mCenter = Vec3f( 0.0f, 0.0f, 0.0f );
  mUp = Vec3f::yAxis();
  mCam.lookAt( mEye, mCenter, mUp );
  
  mFont = Font( "Arial", 120.0f );
  
  mSyphonOutA.setName("ModMap :: Camera A");
  mOSCListener.setup( 3123 );
  
  mOSCParam_speed = 20;
  mRotation = 0;
}

void ModMapApp::keyDown( KeyEvent event ){
  
}

void ModMapApp::mouseDown( MouseEvent event )
{
}

void ModMapApp::update()
{
  float time = app::getElapsedSeconds();
  
  while( mOSCListener.hasWaitingMessages() ) {
		osc::Message message;
		mOSCListener.getNextMessage( &message );
    
    for (int i = 0; i < message.getNumArgs(); i++)
    {
      if (message.getAddress().compare("/ModMap/speed") == 0)
      {
        mOSCParam_speed = message.getArgAsFloat(i);
      }
    }
  }
  
  mRotation = time * mOSCParam_speed ;
}

void ModMapApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );
  gl::setMatrices( mCam );
  
  gl::color(0, 0.3, 0.9);
  float r = getWindowWidth() * 0.1;
//  gl::drawSolidEllipse(getWindowCenter(), 100, 100);
  gl::pushMatrices();
  gl::rotate(Vec3f(mRotation,mRotation * 0.73,0));
  gl::drawSphere(Vec3f(0,0,0), r, 6);
  gl::popMatrices();
  
  mSyphonOutA.publishScreen();
  
  gl::drawString(   toString( getFrameRate() ) , Vec2f( getWindowWidth() - 140.0f, getWindowHeight()- 100.0f ), Color::white(), mFont);

}

CINDER_APP_NATIVE( ModMapApp, RendererGl )
