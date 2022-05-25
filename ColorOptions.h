#ifndef COLOROPTIONS_H
#define COLOROPTIONS_H

#include <QWidget>
#include <QColorDialog>
#include <clockwindow.h>
#include <QShowEvent>
#include <colorbutton.h>

namespace Ui {
class ColorOptions;
}

class ColorOptions : public QWidget
{
    Q_OBJECT

public:
    explicit ColorOptions(QWidget *parent = nullptr);
    ~ColorOptions();
    void setColors(ClockColorData data);
signals:
    void saveColors(ClockColorData data);
    void loadOldColors(ClockColorData data);

    void newHourColor(QColor);
    void newMinuteColor(QColor);
    void newSecondColor(QColor);
    void newBorderColor(QColor);
    void newBackgroundColor(QColor);

private slots:
    void onButtonHourClrClick();
    void onButtonMinuteClrClick();
    void onButtonSecondsClrClick();
    void onButtonBackgroundClrClick();
    void onButtonBorderClrClick();

    void onSave();
    void onCancel();

    void onSelectHourColor(QColor);
    void onSelectMinuteColor(QColor);
    void onSelectSecondColor(QColor);
    void onSelectBorderColor(QColor);
    void onSelectbackgroundColor(QColor);
protected:
    void showEvent(QShowEvent *);
private:
    Ui::ColorOptions *ui;
    QColorDialog *cDial;
    ClockColorData colors, tempColors;

    void DisplayColors(ClockColorData clrs);

};

#endif // COLOROPTIONS_H
