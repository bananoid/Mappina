#include "SceneObj.h"
#include "cinder/Rand.h"
#include "cinder/gl/gl.h"
#include "cinder/app/AppBasic.h"

using namespace ci;



SceneObj::SceneObj()
{
  replaceObj = this;
  isInitialized = false;
}

void SceneObj::setup(){
  isInitialized = true;
  updateRandomScale();
}

void SceneObj::update(){
  
}

void  SceneObj::updateRandomScale(){
  randomScale = randPosNegFloat(1, 2);
}
void SceneObj::draw(){
  if(!isInitialized) return;
  
  float time = app::getElapsedSeconds();
  float rot = time * 50;

  Vec3f center = boundingSphere.getCenter();
  
  gl::pushModelView();
  
  
  
  if(objType == 1){
    
//    if (replaceObj == this) {
//      gl::translate(center);
//      gl::rotate( Vec3f( rot , rot *.2 , 0 ) );
//      gl::translate(center * -1);
//    }
    
    float scale = randomScale;
    gl::translate(center);
    gl::translate(replaceObj->boundingSphere.getCenter() );
    
    gl::scale(Vec3f(scale,scale,scale));
    gl::rotate( Vec3f( rot , rot *.2 , 0 ) );
  }
  
  gl::draw(replaceObj->vbo);
  
  gl::popModelView();
  
}

