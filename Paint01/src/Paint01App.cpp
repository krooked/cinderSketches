#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/ImageIo.h"
#include "cinder/gl/Texture.h"
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

};

void Paint01App::setup()
{    
    gl::clear( Color( 0, 0, 0 ) );
    
    _previousPointX = getMousePos().x;
    _previousPointY = getMousePos().y;

    _myImage = gl::Texture( loadImage( loadAsset( "ps_brush_2.png" ) ) );
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glDepthFunc(GL_ALWAYS);
    gl::enableAlphaBlending();
}

void Paint01App::prepareSettings( Settings *settings )
{
	settings->setTitle("Drawing smooth lines using a geometry shader");
	settings->setWindowSize(800, 800);
    settings->setFrameRate(220);
}

void Paint01App::mouseDown( MouseEvent event )
{
}

void Paint01App::update()
{
}

void Paint01App::draw()
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
     
    while (!_points.empty()) {
        list<Vec2f>::iterator it = _points.begin();
        gl::draw(_myImage, *it);
        _points.pop_front();
    
    }

    _previousPointX = PointA.x;
    _previousPointY = PointA.y;


}

CINDER_APP_BASIC( Paint01App, RendererGl() )
