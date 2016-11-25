#include "dccslider.h"

#include <QWheelEvent>

using namespace dcc;

DCCSlider::DCCSlider(DCCSlider::SliderType type, QWidget *parent) :
    QSlider(parent)
{
    setType(type);
}

void DCCSlider::setType(DCCSlider::SliderType type)
{
    switch(type) {
    case Vernier: setProperty("handleType", "Vernier"); break;
    case Progress: setProperty("handleType", "None"); break;
    default: setProperty("handleType", "Normal"); break;
    }
}

void DCCSlider::wheelEvent(QWheelEvent *e)
{
    e->ignore();
}