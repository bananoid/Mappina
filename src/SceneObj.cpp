#include "SceneObj.h"
#include "cinder/Rand.h"
#include "cinder/gl/gl.h"
#include "cinder/app/AppBasic.h"

using namespace ci;



SceneObj::SceneObj()
{
}

void SceneObj::setup(){
  
}

void SceneObj::update(){
  
}

void SceneObj::draw(){
  
  float time = app::getElapsedSeconds();
  float rot = time * 100;

  Vec3f center = boundingSphere.getCenter();
  
  gl::pushModelView();
  
  if(objType == 1){
  
    gl::translate(center);
    gl::rotate( Vec3f( rot , rot *.5 , 0 ) );
    gl::translate(center * -1);
    
  }
  
  gl::draw(vbo);
  
  gl::popModelView();
  
}

