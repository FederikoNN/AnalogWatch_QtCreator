#ifndef COLORBUTTON_H
#define COLORBUTTON_H

#include <QPushButton>
#include <QPaintEvent>
#include <QWidget>
#include <QBrush>
#include <QPainter>
#include <QPen>
#include <QColor>
#include <QRect>


class ColorButton : public QPushButton
{
    Q_OBJECT
public:
    explicit ColorButton(QWidget *parent = 0);
    void setColor(QColor );
signals:

public slots:

private:
    QColor fill_color;
    void paintEvent (QPaintEvent *);
};

#endif // COLORBUTTON_H
