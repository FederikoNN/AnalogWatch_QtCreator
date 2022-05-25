#ifndef CLOCKWINDOW_H
#define CLOCKWINDOW_H

#include <QWidget>
#include <QPainter>
#include <QTime>
#include <QTimer>
#include <QResizeEvent>

#define MAXIMUM_TIME_IN_HOURS               12
#define HOUR_ARROW_COEF                     0.6
#define MINUTE_ARROW_COEF                   0.9
#define SEC_ARROW_COEF                      1
#define CLOCK_START_BORDER                  5
#define CLOCK_HOUR_ARROW_WIDTH              7
#define CLOCK_MIN_ARROW_WIDTH               4
#define CLOCK_SEC_ARROW_WIDTH               2
#define CLOCK_BORDER_ARROW_WIDTH            5

struct ClockColorData{
    QColor hourClr;
    QColor minuteClr;
    QColor secondsClr;
    QColor borderClr;
    QColor backgroundClr;
};

QT_BEGIN_NAMESPACE
namespace Ui { class ClockWindow; }
QT_END_NAMESPACE

class ClockWindow : public QWidget
{
    Q_OBJECT
public:
    explicit ClockWindow(QWidget *parent = nullptr);
    void setTime(QTime time);
    void pause();
    void unpause();
    void setColors(ClockColorData data);
    ClockColorData getCurrentColors();
    void setFeed(int h, int m, int s);
    QTime getCurrentTime();
public slots:
    void setHourArrowColor(QColor clr);
    void setMinuteArrowColor(QColor clr);
    void setSecondsArrowColor(QColor clr);
    void setBorderColor(QColor clr);
    void setBackgroundColor(QColor clr);
signals:
    void currentTime(QTime);

private slots:
    void onTmr();

private:
    Ui::ClockWindow *ui;
    QRect drawArea;
    int border;
    QTimer *tmr;
    QTime currentClockTime;
    ClockColorData colors;
    QPen hourPen, minutePen, secondsPen, backgroundPen, borderPen;

protected:
    void paintEvent(QPaintEvent *) override;
    void resizeEvent(QResizeEvent *) override;
};
#endif // CLOCKWINDOW_H

