#include "presetsketch.h"

PresetSketch::PresetSketch(QWidget *parent) :
	QWidget(parent)
{
	def_width=16;
    def_height=50;
    eye_size=1;
    m_preset=0;
    m_heights.append(QPointF(-1,0));
    m_heights.append(QPointF(-0.8,0.2));
    m_heights.append(QPointF(-0.8,0.2));
    m_heights.append(QPointF(-0.5,0.6));
    m_heights.append(QPointF(-0.5,0.6));
    m_heights.append(QPointF(-0.3,0.4));
    m_heights.append(QPointF(-0.3,0.4));
    m_heights.append(QPointF(-0.0,0.8));
    m_heights.append(QPointF(-0.0,0.8));
    m_heights.append(QPointF(0.1,0.64));
    m_heights.append(QPointF(0.1,0.64));
    m_heights.append(QPointF(0.2,0.5));
    m_heights.append(QPointF(0.2,0.5));
    m_heights.append(QPointF(0.5,0.7));
    m_heights.append(QPointF(0.5,0.7));
    m_heights.append(QPointF(0.8,0.3));
    m_heights.append(QPointF(0.8,0.3));
    m_heights.append(QPointF(1.0,0.1));
}

void PresetSketch::setPreset(Preset *preset)
{
    m_preset=preset;
    repaint();
}

float min(float x,float y)
{
	if(x>y)
		return y;
	return x;
}
float max(float x,float y)
{
	if(x>y)
		return x;
	return y;
}


void PresetSketch::paintEvent(QPaintEvent *)
{

    if (!m_preset)
    {
        return;
    }
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	float scale_factor=min(this->width()/def_width,this->height()/def_height);
	float shiftx=0,shifty=0;
	if (this->width()/def_width > this->height()/def_height)
	{
		shiftx=(this->width()-scale_factor*def_width)/2;
	}
	else
	{
		shifty=(this->height()-scale_factor*def_height)/2;
	}

    float eyesep=m_preset->getEyeSeperation();
	float lefteyex=def_width/2-eyesep/2;
    float obs_dist=m_preset->getObserverDistance();
	//two eyes
	painter.drawEllipse(shiftx+scale_factor*(lefteyex-eye_size/2),shifty+scale_factor*1,scale_factor*eye_size,scale_factor*eye_size);
	painter.drawEllipse(shiftx+scale_factor*(lefteyex-eye_size/2+eyesep),shifty+scale_factor*1,scale_factor*eye_size,scale_factor*eye_size);
	//screen
    float screen_width= m_preset->getResultWidth()/(float)m_preset->getDotsPerInch();
    painter.drawLine(shiftx+scale_factor*(def_width/2-screen_width/2),shifty+scale_factor*(1+eye_size/2+obs_dist),shiftx+scale_factor*(def_width/2+screen_width/2),shifty+scale_factor*(1+eye_size/2+obs_dist));
    float max_depth=m_preset->getMaximumDepth();
    float min_depth=m_preset->getMinimumDepth();
    bool cross=!m_preset->getIsParallel();
    QVector<QPoint> h_proccessed;
    QPointF i;
    QPoint spoint;
	if(!cross)
	{
        float pxscale=(screen_width-eyesep)*(obs_dist+min_depth)/(2*obs_dist)+(eyesep/2);
        foreach (i, m_heights)
        {
            h_proccessed.append(QPoint (shiftx+scale_factor*(i.x()*pxscale+def_width/2),shifty+scale_factor*(-i.y()*(max_depth-min_depth)+1+eye_size/2+obs_dist+max_depth)));
        }

	}
	else
	{
        float in_far=obs_dist+max_depth;
        float in_near=obs_dist+min_depth;
        float screen_sep_near=(eyesep*min_depth)/in_near;
        float screen_sep_far=(eyesep*max_depth)/in_far;
        float out_far=(obs_dist*eyesep)/(eyesep+screen_sep_near);
        float out_near=(obs_dist*eyesep)/(eyesep+screen_sep_far);
        float pxscale=(screen_width+eyesep)*(out_near)/(2*obs_dist)-(eyesep/2);
        foreach (i, m_heights)
        {
            h_proccessed.append(QPoint (shiftx+scale_factor*(i.x()*pxscale+def_width/2),shifty+scale_factor*(-i.y()*(out_far-out_near)+1+eye_size/2+out_far)));
        }
    }
    painter.drawLines(h_proccessed);

    spoint=h_proccessed.at(3);
    if(!cross)
    {
        painter.drawLine(shiftx+scale_factor*(def_width/2-eyesep/2),shifty+scale_factor*(1+eye_size/2),spoint.x(),spoint.y());
        painter.drawLine(shiftx+scale_factor*(def_width/2+eyesep/2),shifty+scale_factor*(1+eye_size/2),spoint.x(),spoint.y());
    }
    else
    {
        float eyeleftx=shiftx+scale_factor*(def_width/2-eyesep/2);
        float eyerightx=shiftx+scale_factor*(def_width/2+eyesep/2);
        float eyey=shifty+scale_factor*(1+eye_size/2);
        float screeny=shifty+scale_factor*(1+eye_size/2+obs_dist);
        float leyes=(-eyeleftx+spoint.x())*(screeny-spoint.y())/(spoint.y()-eyey)+spoint.x();
        float reyes=(-eyerightx+spoint.x())*(screeny-spoint.y())/(spoint.y()-eyey)+spoint.x();
        painter.drawLine(eyeleftx,eyey,leyes,screeny);
        painter.drawLine(eyerightx,eyey,reyes,screeny);
    }
}

void PresetSketch::presetUpdated()
{
    repaint();
}
