#include "ColorOptions.h"
#include "ui_ColorOptions.h"

ColorOptions::ColorOptions(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ColorOptions)
{
    ui->setupUi(this);

    cDial = new QColorDialog();

    colors.hourClr = Qt::black;
    colors.minuteClr = Qt::black;
    colors.secondsClr = Qt::black;
    colors.borderClr = Qt::black;
    colors.backgroundClr = Qt::gray;
    DisplayColors(colors);

    connect(ui->pb_hourClr,      SIGNAL(clicked()),SLOT(onButtonHourClrClick()));
    connect(ui->pb_minuteClr,    SIGNAL(clicked()),SLOT(onButtonMinuteClrClick()));
    connect(ui->pb_secondsClr,   SIGNAL(clicked()),SLOT(onButtonSecondsClrClick()));
    connect(ui->pb_backgroundClr,SIGNAL(clicked()),SLOT(onButtonBackgroundClrClick()));
    connect(ui->pb_borderClr,    SIGNAL(clicked()),SLOT(onButtonBorderClrClick()));

    connect(ui->pb_save,   SIGNAL(clicked()),SLOT(onSave()));
    connect(ui->pb_cancel, SIGNAL(clicked()),SLOT(onCancel()));
}

ColorOptions::~ColorOptions()
{
    delete ui;
    if(cDial) delete cDial;
}

void ColorOptions::setColors(ClockColorData data)
{
    colors = data;
    tempColors = data;
    DisplayColors(colors);
}
// Обработка нажатия на кнопки выбора цвета
void ColorOptions::onButtonHourClrClick()
{
    connect(cDial,SIGNAL(colorSelected(QColor)),this,SLOT(onSelectHourColor(QColor)));
    cDial->setWindowTitle(("Выберите цвет часовой стрелки"));
    cDial->show();
}

void ColorOptions::onButtonMinuteClrClick()
{
    connect(cDial,SIGNAL(colorSelected(QColor)),this,SLOT(onSelectMinuteColor(QColor)));
    cDial->setWindowTitle(("Выберите цвет минутной стрелки"));
    cDial->show();
}

void ColorOptions::onButtonSecondsClrClick()
{
    connect(cDial,SIGNAL(colorSelected(QColor)),this,SLOT(onSelectSecondColor(QColor)));
    cDial->setWindowTitle(("Выберите цвет секундной стрелки"));
    cDial->show();
}

void ColorOptions::onButtonBackgroundClrClick()
{
    connect(cDial,SIGNAL(colorSelected(QColor)),this,SLOT(onSelectbackgroundColor(QColor)));
    cDial->setWindowTitle(("Выберите цвет циферблата"));
    cDial->show();
}

void ColorOptions::onButtonBorderClrClick()
{
    connect(cDial,SIGNAL(colorSelected(QColor)),this,SLOT(onSelectBorderColor(QColor)));
    cDial->setWindowTitle(("Выберите цвет меток циферблата"));
    cDial->show();
}

// Сохранить цвета
void ColorOptions::onSave()
{
    colors = tempColors;
    emit saveColors(colors);
    hide();
}
// Откатить настройки цвета
void ColorOptions::onCancel()
{
    emit loadOldColors(colors);
    hide();
}

// ********** Обработка выбранного цвета
void ColorOptions::onSelectHourColor(QColor clr)
{
    disconnect(cDial,SIGNAL(colorSelected(QColor)),this,SLOT(onSelectHourColor(QColor)));
    tempColors.hourClr = clr;
    ui->pb_hourClr->setColor(clr);
    emit newHourColor(clr);
}

void ColorOptions::onSelectMinuteColor(QColor clr)
{
    disconnect(cDial,SIGNAL(colorSelected(QColor)),this,SLOT(onSelectMinuteColor(QColor)));
    tempColors.minuteClr = clr;
    ui->pb_minuteClr->setColor(clr);
    emit newMinuteColor(clr);
}

void ColorOptions::onSelectSecondColor(QColor clr)
{
    disconnect(cDial,SIGNAL(colorSelected(QColor)),this,SLOT(onSelectSecondColor(QColor)));
    tempColors.secondsClr = clr;
    ui->pb_secondsClr->setColor(clr);
    emit newSecondColor(clr);
}

void ColorOptions::onSelectBorderColor(QColor clr)
{
    disconnect(cDial,SIGNAL(colorSelected(QColor)),this,SLOT(onSelectBorderColor(QColor)));
    tempColors.borderClr = clr;
    ui->pb_borderClr->setColor(clr);
    emit newBorderColor(clr);
}

void ColorOptions::onSelectbackgroundColor(QColor clr)
{
    disconnect(cDial,SIGNAL(colorSelected(QColor)),this,SLOT(onSelectbackgroundColor(QColor)));
    tempColors.backgroundClr = clr;
    ui->pb_backgroundClr->setColor(clr);
    emit newBackgroundColor(clr);
}



void ColorOptions::showEvent(QShowEvent *)
{
    tempColors = colors;
    DisplayColors(colors);
}

void ColorOptions::DisplayColors(ClockColorData clrs)
{
    ui->pb_hourClr->setColor(      clrs.hourClr);
    ui->pb_minuteClr->setColor(    clrs.minuteClr);
    ui->pb_secondsClr->setColor(   clrs.secondsClr);
    ui->pb_backgroundClr->setColor(clrs.backgroundClr);
    ui->pb_borderClr->setColor(    clrs.borderClr);
}
