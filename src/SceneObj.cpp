#include "SceneObj.h"
#include "cinder/Rand.h"
#include "cinder/gl/gl.h"
#include "cinder/app/AppBasic.h"

using namespace ci;



SceneObj::SceneObj()
{
  replaceObj = this;
  isInitialized = false;
  scale = 1;
  rotation = 0;
  rotationDir = Vec3f(1,0.2,0);
  
  int mSeed = clock() & 65535;
	int mOctaves = 2;
  mPerlin = Perlin( mOctaves, mSeed );
  
  perlinScale = 1;
  perlinScaleAmp = 0;
  perlinScaleSpeed = 10;
  perlinScaleFreq = 0.1;
}

void SceneObj::setup(){
  isInitialized = true;
  updateRandomScale();

}

void SceneObj::update(){
  float time = app::getElapsedSeconds();
  Vec3f center = boundingSphere.getCenter() * 0.00001;
  
  rotationDir = mPerlin.dfBm(center.x , center.y, time  * 0.0001);
  rotationDir.normalize();
  
  center = boundingSphere.getCenter() * 0.001;
  center *= perlinScaleFreq;
  time *= 1;
  perlinScale = sin(center.x + time) * cos(center.y + time) * .5;
  scale *= 1 + perlinScale * perlinScaleAmp;
  
}

void  SceneObj::updateRandomScale(){
  randomScale = randPosNegFloat(1, 2);
}
void SceneObj::draw(){
  if(!isInitialized) return;
  
  float rot = rotation;

  Vec3f center = boundingSphere.getCenter();
  
  gl::pushModelView();
  
  if(objType == 1){
    
//    if (replaceObj == this) {
//      gl::translate(center);
//      gl::rotate( Vec3f( rot , rot *.2 , 0 ) );
//      gl::translate(center * -1);
//    }
    
    float resScale = randomScale * scale;
    gl::translate(center);
    gl::translate(replaceObj->boundingSphere.getCenter() );
    
    gl::scale(Vec3f(resScale,resScale,resScale));
    gl::rotate( rotationDir * rot );
    
//    console() << rot << " \n";
  }
  
  gl::draw(replaceObj->vbo);
  
  gl::popModelView();
  
}

