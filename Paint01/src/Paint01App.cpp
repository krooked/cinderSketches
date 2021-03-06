#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/ImageIo.h"
#include "cinder/gl/Texture.h"
#include "cinder/Surface.h"
#include <list>

using namespace ci;
using namespace ci::app;
using namespace std;

class Paint01App : public AppBasic {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
    void prepareSettings( Settings *settings );   
private:
    float _previousPointX;
    float _previousPointY;
    ci::gl::Texture _myImage;
    std::list<Vec2f> _points;
    list<Vec2f>::iterator _it;
    Surface brushSurface;
};

void Paint01App::setup()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glDepthFunc(GL_ALWAYS);
    gl::enableAlphaBlending();
    gl::enableVerticalSync();
    gl::clear(Color(0, 0, 0)); 
    
    brushSurface = loadImage( loadAsset("ps_brush_2.png"));
    _myImage = gl::Texture(brushSurface);
    
    _previousPointX = getMousePos().x;
    _previousPointY = getMousePos().y;
    
}

void Paint01App::prepareSettings(Settings *settings)
{
	settings->setTitle("Paint01");
	settings->setWindowSize(getDisplay().getWidth(), getDisplay().getHeight());
    settings->setFrameRate(60);
//    settings->setFullScreen();
}

void Paint01App::mouseDown( MouseEvent event )
{
}

void Paint01App::update()
{
    if (!_myImage) {
        return;
    }
    
    const float brushWidth = _myImage.getWidth();
    const float brushHeight = _myImage.getHeight();
    
    Vec2f PointA = Vec2f(_previousPointX, _previousPointY);
    Vec2f PointB = Vec2f(getMousePos().x, getMousePos().y);
    Vec2f VectorAB = Vec2f(-PointA.x + PointB.x, -PointA.y + PointB.y);
    VectorAB.normalize();
    
    float distanceBetweenBrushDraw = 1;
    VectorAB*=distanceBetweenBrushDraw;
    
    while (PointA.distance(PointB) > distanceBetweenBrushDraw) {        
        _points.push_back(Vec2f(PointA.x - brushWidth / 2, PointA.y - brushHeight / 2));
        PointA += VectorAB;
    }
    
    _previousPointX = PointA.x;
    _previousPointY = PointA.y;
}

void Paint01App::draw()
{
    while (!_points.empty()) {
        _it = _points.begin();
        gl::draw(_myImage, *_it);
        cout<<*_it<<endl;
        _points.pop_front();
    }
}

CINDER_APP_BASIC( Paint01App, RendererGl() )
