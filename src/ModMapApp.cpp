#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/Utilities.h"
#include "cinder/Font.h"
#include "cinder/Camera.h"
#include "cinder/params/Params.h"

#include "cinder/gl/Texture.h"
#include "cinder/gl/Fbo.h"
#include "cinder/gl/Vbo.h"
#include "cinder/gl/GlslProg.h"

#include "cinder/ObjLoader.h"

#include "cinder/Sphere.h"

#include "cinderSyphon.h"
#include "OscListener.h"

#include "MeshHelper.h"

#include "SceneObj.h"

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
  float lineOff;
  
  CameraPersp mCam;
  Vec3f mEye;
  Vec3f mCenter;
  Vec3f mUp;
  Quatf mSceneRotation;
  
  TriMesh			  mMesh;
	gl::VboMesh		mVBO;
	gl::GlslProg	mShader;
  
  std::vector<SceneObj> mEsaboxes;
  std::vector<SceneObj> mPlatonics;
  
  void setupScene();
  void drawScene();
  
  params::InterfaceGl mParams;
  
  float mDeltaTime;
  float lastElapsedTime;
  
};

void ModMapApp::prepareSettings(cinder::app::AppBasic::Settings *settings){
  settings->setWindowSize(WIDTH*.5,HEIGHT*.5);
	settings->setFrameRate(60.f);
//  settings->enableHighDensityDisplay();
}

void ModMapApp::setup()
{
  //Setup Params
  mParams = mParams = params::InterfaceGl( "ModMap", Vec2i( 225, 200 ) );
  mParams.addParam( "Scene Rotation", &mSceneRotation );
  mRotation = 0;
  lineOff = 0;
  
  //Setup OSC
  mOSCListener.setup( 3123 );
  mOSCParam_speed = 6;
  
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
  //mShader = gl::GlslProg( loadResource( "./assets/shader.vert" ), loadResource( "./assets/shader.frag" ) );

	mShader = gl::GlslProg(loadResource("./assets/wireframe.vert"), loadResource("./assets/wireframe.frag"), loadResource("./assets/wireframe.geom"), GL_TRIANGLES, GL_TRIANGLE_STRIP, 6);

  //TIME SETUP
  mDeltaTime = 0;
  lastElapsedTime = app::getElapsedSeconds();
}

void ModMapApp::setupScene(){
  ObjLoader loader( loadResource( "./assets/cinderModMap002.obj" ));
  
  std::vector<ObjLoader::Group> groups = loader.getGroups();
  size_t numGroups = loader.getNumGroups();
  
  console() << "setupScene >>> numGroups :: " << numGroups << "\n";
  
  ObjLoader::Group g;
  
  SceneObj sObj;
  TriMesh mesh;
  gl::VboMesh vbo;
  Sphere boundingSphere;
  
  for(int i = 0; i < numGroups ; i++ ){
    g = groups[i];
    
    
    
    loader.load(i, &mesh, true,true,true );
    vbo = gl::VboMesh( mesh );
    boundingSphere = Sphere::calculateBoundingSphere( mesh.getVertices() );
    
    sObj = SceneObj();
    sObj.group = g;
    sObj.vbo = vbo;
    sObj.mesh = mesh;
    sObj.boundingSphere = boundingSphere;
    sObj.index = i;
    
    
    console() << "\tgroup " << i << " \tname ::" << g.mName << "\t\tmcenter :" << boundingSphere.getCenter() << "\n";
    
    if(g.mName.find("platonic") == 0){

      sObj.objType = 1;
      mPlatonics.push_back(sObj);
      
    }else{
      
      sObj.objType = 0;
      mEsaboxes.push_back(sObj);
      
    }
    
    sObj.setup();
  }
  
}

void ModMapApp::mouseDown( MouseEvent event )
{
}

void ModMapApp::update()
{
  float time = app::getElapsedSeconds();
  
  mDeltaTime = time - lastElapsedTime;
  lastElapsedTime = time;
  
//  console() << "time " << time << " delta" << mDeltaTime << "\n" ;
  
  if( mOSCListener.hasWaitingMessages() ) {
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
  
  SceneObj sObj;
  for(std::vector<SceneObj>::size_type i = 0; i != mPlatonics.size(); i++) {
    sObj = mPlatonics[i];
    sObj.update();
  }
  
  
  mRotation = sin( time * mOSCParam_speed * 1) * 2;
  lineOff += mDeltaTime * mOSCParam_speed *.4;
}

void ModMapApp::draw()
{
//  float time = app::getElapsedSeconds();
  
  gl::enableDepthWrite();
	gl::enableDepthRead();
  
  gl::clear( ColorA( 0.3, 0.3, 0.3 ) );
  
  m_fboSy.bindFramebuffer();
  gl::clear( Color( 0, 0, 0 ) );
  
  gl::setMatrices( mCam );
  
//  gl::setMatricesWindow(m_fboSy.getSize() * Vec2i(1,-1));
  gl::setViewport(m_fboSy.getBounds());
  
  drawScene();
  
  m_fboSy.unbindFramebuffer();
  
//  *m_texSyRef = m_fboSy.getDepthTexture();
  *m_texSyRef = m_fboSy.getTexture();

  mSyphonOutA.publishTexture(m_texSyRef);
  
  gl::setMatricesWindow(getWindowSize());
  gl::setViewport(Area( 0, 0, getWindowWidth(), getWindowHeight()));
  
  gl::pushMatrices();
    gl::scale(Vec3f(0.5,0.5,0.5));
    gl::draw(m_fboSy.getTexture(), Vec2f(0, 0));
  gl::popMatrices();
  
  gl::drawString( toString( getAverageFps() ) , Vec2f( 20, 400 ), Color::white(), mFont);

}

void ModMapApp::drawScene(){
  SceneObj sObj;
  
  mShader.bind();
  
  
  mShader.uniform("WIN_SCALE", Vec2f(WIDTH,HEIGHT) );
  //mShader.uniform("lineOff", lineOff );
  for(std::vector<SceneObj>::size_type i = 0; i != mEsaboxes.size(); i++) {
    sObj = mEsaboxes[i];
    sObj.draw();
  }
  
  for(std::vector<SceneObj>::size_type i = 0; i != mPlatonics.size(); i++) {
    sObj = mPlatonics[i];
    sObj.draw();
  }
  
  mShader.unbind();
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

CINDER_APP_NATIVE( ModMapApp, RendererGl )
