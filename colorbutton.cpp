#include "colorbutton.h"


#define BORDER      3
ColorButton::ColorButton(QWidget *parent) :
    QPushButton(parent)
{
    fill_color = Qt::green;
}

void ColorButton::setColor(QColor clr)
{
    fill_color = clr;
    fill_color.setAlpha(255);
    update();
}

void ColorButton::paintEvent(QPaintEvent *)
{
    QPainter *painter = new QPainter(this);

    QBrush brush(fill_color, Qt::SolidPattern);
    QPen pen( brush,  1, Qt::SolidLine);
    pen.setColor(Qt::black);
    painter->setPen(pen);
    painter->setBrush(brush);
    QRect rect(BORDER,BORDER-1,this->width()-(BORDER+2),this->height()-(BORDER+3));
    painter->drawRoundedRect(rect,BORDER,BORDER);

    painter->end();
}
