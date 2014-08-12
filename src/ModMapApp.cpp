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

#define SYWIDTH 1920
#define SYHEIGHT 1080

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
  gl::Fbo m_fboSy;
  gl::TextureRef m_texSyRef;
  
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
  
  void setupScene();
  
};

void ModMapApp::prepareSettings(cinder::app::AppBasic::Settings *settings){
  settings->setWindowSize(WIDTH*.5,HEIGHT*.5);
	settings->setFrameRate(60.f);
//  settings->enableHighDensityDisplay();
}

void ModMapApp::setup()
{
  //Setup Params
  mRotation = 0;
  
  //Setup OSC
  mOSCListener.setup( 3123 );
  mOSCParam_speed = 20;
  
  //Setup Camera
  mCam.setPerspective( 31.417, getWindowAspectRatio(), 1000.0f, 6000.0f );
  mEye = Vec3f( 0.0f, 0.0f, 3396.584);
  mCenter = Vec3f( 0.0f, 0.0f, 0.0f );
  mUp = Vec3f::yAxis();
  mCam.lookAt( mEye, mCenter, mUp);
  
  //Setup Font
  mFont = Font( "Arial", 120.0f );
  
  //Setup VBO
  setupScene();
  
  //Setup FBO
  gl::Fbo::Format format2;
  format2.enableDepthBuffer(true);
  format2.enableColorBuffer(true, 1);
  format2.enableMipmapping( true );
  format2.setMinFilter( GL_LINEAR_MIPMAP_LINEAR );
  format2.setMagFilter(GL_LINEAR_MIPMAP_LINEAR);
  format2.setWrap(GL_CLAMP, GL_CLAMP);
  format2.setColorInternalFormat(GL_RGBA16F_ARB);
  m_fboSy = gl::Fbo(SYWIDTH, SYHEIGHT, format2);
  m_fboSy.getTexture().setFlipped(true);
  m_texSyRef = gl::Texture::create(SYWIDTH, SYHEIGHT);
  
  //Setup Syphon
  mSyphonOutA.setName("ModMap :: Camera A");
  
  //Setup Shader
  mShader = gl::GlslProg( loadResource( "./assets/shader.vert" ), loadResource( "./assets/shader.frag" ) );

}

void ModMapApp::setupScene(){
  ObjLoader loader( loadResource( "./assets/cinderModMap001.obj" ));
  
  loader.load(&mMesh, true,true,false );
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
  
  mRotation = sin( time * mOSCParam_speed * 1) * 2;
}

void ModMapApp::draw()
{
  float time = app::getElapsedSeconds();
  
  gl::enableDepthWrite();
	gl::enableDepthRead();
  
  gl::clear( ColorA( 0.3, 0.3, 0.3 ) );
  
  
  
  m_fboSy.bindFramebuffer();
  gl::clear( Color( 0, 0, 0 ) );
  
  
  
  
  gl::pushMatrices();
  
    gl::setMatrices( mCam );
  
//      gl::setMatricesWindow(m_fboSy.getSize() * Vec2i(1,-1));
    gl::setViewport(m_fboSy.getBounds());
  

  
      gl::rotate(Vec3f(0,mRotation,0));
  
//  mShader.bind();
//  mShader.uniform("lineOff", (float)(time * 0.1));
  gl::draw(mVBO);
//  mShader.unbind();
  
  m_fboSy.unbindFramebuffer();
  
  gl::popMatrices();
  
  *m_texSyRef = m_fboSy.getDepthTexture();
//  *m_texSyRef = m_fboSy.getTexture();

  mSyphonOutA.publishTexture(m_texSyRef);
  
//  gl::pushMatrices();
  gl::setMatricesWindow(getWindowSize());
  gl::setViewport(Area( 0, 0, getWindowWidth(), getWindowHeight()));
  
  gl::pushMatrices();
    gl::scale(Vec3f(0.5,0.5,0.5));
    gl::draw(m_fboSy.getTexture(), Vec2f(0, 0));
  gl::popMatrices();
  
//  gl::drawString( toString( getAverageFps() ) , Vec2f( 20, 400 ), Color::white(), mFont);
//  gl::popMatrices();
}

CINDER_APP_NATIVE( ModMapApp, RendererGl )
