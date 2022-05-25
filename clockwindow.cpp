#include "clockwindow.h"
#include <qmath.h>

ClockWindow::ClockWindow(QWidget *parent) : QWidget(parent)
{

    border = CLOCK_START_BORDER;
    drawArea.setRect(border,border,this->width()-border*2, this->height()-border*2);

    tmr = new QTimer();

       colors.borderClr = Qt::black;
       colors.minuteClr = Qt::black;
       colors.secondsClr = Qt::black;
       colors.hourClr = Qt::black;
       colors.backgroundClr = Qt::gray;

       hourPen.setColor(colors.hourClr);
       hourPen.setWidth(CLOCK_HOUR_ARROW_WIDTH);

       minutePen.setColor(colors.minuteClr);
       minutePen.setWidth(CLOCK_MIN_ARROW_WIDTH);

       secondsPen.setColor(colors.secondsClr);
       secondsPen.setWidth(CLOCK_SEC_ARROW_WIDTH);

       backgroundPen.setColor(colors.backgroundClr);
       QBrush brush;
       brush.setStyle(Qt::SolidPattern);
       brush.setColor(colors.backgroundClr);
       backgroundPen.setBrush(brush);

       borderPen.setColor(colors.borderClr);
       borderPen.setWidth(CLOCK_BORDER_ARROW_WIDTH);


       tmr->setInterval(1000);
       connect(tmr, SIGNAL(timeout()), this, SLOT(onTmr()));
       tmr->start();
}

void ClockWindow::setTime(QTime time)
{
    currentClockTime = time;
    update();
}

void ClockWindow::pause()
{
    tmr->stop();
}

void ClockWindow::unpause()
{
    tmr->start();
}

QTime ClockWindow::getCurrentTime()
{
    return  currentClockTime = QTime::currentTime();
}

void ClockWindow::onTmr()
{
    currentClockTime = currentClockTime.addSecs(1);
    update();
}

void ClockWindow::resizeEvent(QResizeEvent *)
{
    drawArea.setRect(border,border,this->width()-border*2, this->height()-border*2);
    update();
}

void ClockWindow::paintEvent(QPaintEvent *)
{
//    QTime currentClockTime = QTime::currentTime();
    QPainter *painter = new QPainter(this);
    painter->setRenderHints(QPainter::Antialiasing);

    int w=drawArea.width(),h=drawArea.height(),radius=0;

    if(w>h) radius = h/2;
    else radius = w/2;

    QPoint center = drawArea.center();
    painter->setPen(backgroundPen);
    painter->setBrush(backgroundPen.brush());
    painter->drawEllipse(center,radius,radius);
    painter->setBrush(Qt::NoBrush);

    painter->setPen(borderPen);
    painter->translate(center);

    for(int i(0); i < 60; i++)
    {
        if(i%5 == 0)
        {
            painter->drawLine(0, radius, 0, radius - 15);
        }
        else
        {
            painter->drawLine(0, radius, 0, radius - 4);
        }
        painter->rotate(6.0);
    }
    painter->setPen(Qt::NoPen);

    /* ***** Расчет длины стрелок и положения ***** */
       // минутная стрелка
       QPointF l1p1(0.0, 0.0),l1p2;
       qreal x,y,alpha;
       alpha = (((360*(currentClockTime.minute() + currentClockTime.second()/60.0) /60.0))-90) * 3.14/180.0;
       x = radius*MINUTE_ARROW_COEF*cos(alpha);
       y = radius*MINUTE_ARROW_COEF*sin(alpha);
       l1p2 = QPointF(x,y);

       // часовая стрелка
       QPointF l2p1(0.0, 0.0),l2p2;
       alpha = ((360*(currentClockTime.hour() + currentClockTime.minute()/60.0) /MAXIMUM_TIME_IN_HOURS)-90)*3.14/180.0;
       x = radius*HOUR_ARROW_COEF*cos(alpha);
       y = radius*HOUR_ARROW_COEF*sin(alpha);
       l2p2 = QPointF(x,y);

       // секундная
       QPointF l3p1(0.0, 0.0),l3p2;
       alpha = ((360*currentClockTime.second()/60)-90)*3.14/180;
       x = radius*SEC_ARROW_COEF*cos(alpha);
       y = radius*SEC_ARROW_COEF*sin(alpha);
       l3p2 = QPointF(x,y);

       //отрисовка

       painter->setPen(minutePen);
       painter->drawLine(l1p1,l1p2);

       painter->setPen(hourPen);
       painter->drawLine(l2p1,l2p2);

       painter->setPen(secondsPen);
       painter->drawLine(l3p1,l3p2);

       delete painter;
}

void ClockWindow::setBorderColor(QColor clr)
{
    colors.borderClr = clr;
    borderPen.setColor(colors.borderClr);
    update();
}

void ClockWindow::setMinuteArrowColor(QColor clr)
{
    colors.minuteClr = clr;
    minutePen.setColor(colors.minuteClr);
    update();
}

void ClockWindow::setHourArrowColor(QColor clr)
{
    colors.hourClr = clr;
    hourPen.setColor(colors.hourClr);
    update();
}

void ClockWindow::setSecondsArrowColor(QColor clr)
{
    colors.secondsClr = clr;
    secondsPen.setColor(colors.secondsClr);
    update();
}

void ClockWindow::setBackgroundColor(QColor clr)
{
    colors.backgroundClr = clr;
    backgroundPen.setColor(colors.backgroundClr);
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(colors.backgroundClr);
    backgroundPen.setBrush(brush);
    update();
}

void ClockWindow::setColors(ClockColorData data)
{
    colors = data;

    hourPen.setColor(colors.hourClr);

    minutePen.setColor(colors.minuteClr);

    secondsPen.setColor(colors.secondsClr);

    backgroundPen.setColor(colors.backgroundClr);
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(colors.backgroundClr);
    backgroundPen.setBrush(brush);

    borderPen.setColor(colors.borderClr);

    update();
}

ClockColorData ClockWindow::getCurrentColors()
{
    return  colors;
}
