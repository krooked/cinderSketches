#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/ImageIo.h"
#include "cinder/gl/Texture.h"
#include "cinder/Camera.h"

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
    CameraPersp mRenderCam;
    ci::gl::Texture myImage;

};

void Paint01App::setup()
{    
    _previousPointX = getMousePos().x;
    _previousPointY = getMousePos().y;
    // clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );
    myImage = gl::Texture( loadImage( loadAsset( "ps_brush_2.png" ) ) );
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glDepthFunc(GL_ALWAYS);
    gl::enableAlphaBlending();
    //gl::enableWireframe();
    //gl::enableAdditiveBlending();
}

void Paint01App::prepareSettings( Settings *settings )
{
	settings->setTitle("Drawing smooth lines using a geometry shader");
//    settings->setWindowPos(0, 0);
	settings->setWindowSize(getDisplay().getWidth(), 800);
//    settings->setFullScreen();
    settings->setFrameRate(60);
}

void Paint01App::mouseDown( MouseEvent event )
{
}

void Paint01App::update()
{

}

void Paint01App::draw()
{
    if (!myImage) {
        return;
    }
    const float brushWidth = myImage.getWidth();
    const float brushHeight = myImage.getHeight();
    
    Vec2f PointA = Vec2f(_previousPointX, _previousPointY);
    Vec2f PointB = Vec2f(getMousePos().x, getMousePos().y);
    Vec2f VectorAB = Vec2f(-PointA.x + PointB.x, -PointA.y + PointB.y);
    VectorAB.normalize();

    float distanceBetweenBrushDraw = 1;
    VectorAB*=distanceBetweenBrushDraw;
    
    while (PointA.distance(PointB) > distanceBetweenBrushDraw) {
        gl::draw(myImage, Vec2f(PointA.x - brushWidth / 2, PointA.y - brushHeight / 2));
        console()<<Vec2f(PointA.x - brushWidth / 2, PointA.y - brushHeight / 2)<<endl;
        PointA += VectorAB;
    }
    
    _previousPointX = PointA.x;
    _previousPointY = PointA.y;


}

CINDER_APP_BASIC( Paint01App, RendererGl() )
