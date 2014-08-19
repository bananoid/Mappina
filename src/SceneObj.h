#pragma once
#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/Utilities.h"
#include "cinder/Camera.h"

#include "cinder/gl/Texture.h"
#include "cinder/gl/Fbo.h"
#include "cinder/gl/Vbo.h"
#include "cinder/gl/GlslProg.h"

#include "cinder/ObjLoader.h"

#include "cinder/Sphere.h"

#include "cinder/Perlin.h"
#include "cinder/Rand.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class SceneObj {
  
public:
  
  enum { TYPE_BOX=0, TYPE_INTERNAL };
  
	SceneObj();
  bool isInitialized;
  
  TriMesh mesh;
  gl::VboMesh vbo;
  Sphere boundingSphere;
  ObjLoader::Group group;
  
  SceneObj *replaceObj;
  
  float randomScale;
  void updateRandomScale();
  
  float scale;
  
  float perlinScale;
  float perlinScaleAmp;
  float perlinScaleSpeed;
  float perlinScaleFreq;
  
  float rotation;
  Vec3f rotationDir;
  
  int index;
  int objType;
  
  void setup();
  void update();
  void draw();
  
  Perlin	mPerlin;
  
};

