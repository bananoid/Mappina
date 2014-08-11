#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/Utilities.h"
#include "cinder/Font.h"
#include "cinder/Camera.h"

#include "cinder/gl/Texture.h"
#include "cinder/gl/Fbo.h"
#include "cinder/gl/Vbo.h"
#include "cinder/gl/GlslProg.h"

#include "cinder/ObjLoader.h"

#include "cinder/Sphere.h"

#include "cinderSyphon.h"
#include "OscListener.h"

#include "MeshHelper.h"

#define WIDTH 1920
#define HEIGHT 1080
#define RES_DEF_OBJ		CINDER_RESOURCE( ./, cube.obj, 128, DATA )

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
  
  TriMesh			  mMesh;
	gl::VboMesh		mVBO;
	gl::GlslProg	mShader;
  
};


void ModMapApp::prepareSettings(cinder::app::AppBasic::Settings *settings){
  settings->setWindowSize(WIDTH*.5,HEIGHT*.5);
	settings->setFrameRate(60.f);
//  settings->enableHighDensityDisplay();
}

void ModMapApp::setup()
{
  //Camera
  mCam.setPerspective( 31.417, getWindowAspectRatio(), 5.0f, 3000.0f );
  mEye = Vec3f( 0.0f, 0.0f, 3396.584);
  mCenter = Vec3f( 0.0f, 0.0f, 0.0f );
  mUp = Vec3f::yAxis();
  mCam.lookAt( mEye, mCenter, mUp );
  mCam.setNearClip(10000);
//  mCam.setFarClip(0);
  mFont = Font( "Arial", 120.0f );
  
  mSyphonOutA.setName("ModMap :: Camera A");
  mOSCListener.setup( 3123 );
  
  mOSCParam_speed = 20;
  mRotation = 0;
  
  //Setup mVBO
//  mMesh = MeshHelper::createSphere();
//  mVBO = gl::VboMesh( mMesh );
  
  ObjLoader loader( loadResource( "./assets/cinderModMap001.obj" ) );
//  ObjLoader loader( loadFile( "exa010.obj" ) );
  loader.load( &mMesh, true );
  mVBO = gl::VboMesh( mMesh );
}

void ModMapApp::keyDown( KeyEvent event ){
  if( event.getChar() == 'o' ) {
		fs::path path = getOpenFilePath();
		if( ! path.empty() ) {
			ObjLoader loader( loadFile( path ) );
			loader.load( &mMesh, true );
			mVBO = gl::VboMesh( mMesh );
			console() << "path" << path;
      console() << "Total verts: " << mMesh.getVertices().size() << std::endl;
		}
	}
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
  gl::enableDepthWrite();
	gl::enableDepthRead();
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );
  gl::setMatrices( mCam );
  
  gl::color(0, 0.3, 0.9);
//  float r = getWindowWidth() * 0.1;
//  gl::drawSolidEllipse(getWindowCenter(), 100, 100);
  gl::pushMatrices();
//  gl::rotate(Vec3f(0,mRotation,0));
//  gl::scale(Vec3f(100,100,100));
//  gl::drawSphere(Vec3f(0,0,0), r, 6);
  gl::draw(mVBO);
  gl::popMatrices();
  
  mSyphonOutA.publishScreen();
  
  gl::drawString(   toString( getFrameRate() ) , Vec2f( getWindowWidth()*.5 - 140.0f, getWindowHeight()*.5 - 100.0f ), Color::white(), mFont);

}

CINDER_APP_NATIVE( ModMapApp, RendererGl )
