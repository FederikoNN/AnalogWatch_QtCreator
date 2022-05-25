#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QApplication>
#include <QDir>
#include <QVariant>
#include <QIcon>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), clock(0)
{
    ui->setupUi(this);
    ui->clockFeedPanel->hide();
    clock = new ClockWindow();
    clock->setTime(QTime::currentTime());
    ui->clockLayout->addWidget(clock);

    connect(ui->actionExit,SIGNAL(triggered()),SLOT(on_actionExit_clicked()));
    connect(ui->actionClock_feed,SIGNAL(triggered()),this,SLOT(showFeedOptions()));
    connect(ui->actionTable,SIGNAL(triggered()),this,SLOT(showTable()));
    connect(ui->actionColor_options,SIGNAL(triggered()),this,SLOT(showOptions()));


    feed = ui->toolBar->addAction(QIcon(":/ui/Icons/auto-repair.png"),"");
    colors = ui->toolBar->addAction(QIcon(":/ui/Icons/colors.png"),"");
    table =ui->toolBar->addAction(QIcon(":/ui/Icons/calendar.png"),"");


    connect(colors,SIGNAL(triggered()),this,SLOT(showOptions()));
    connect(feed,SIGNAL(triggered()),this,SLOT(showFeedOptions()));
    connect(table,SIGNAL(triggered()),this,SLOT(showTable()));

    ui->dockWidget->hide();

    ui->tableWidget->setColumnCount(1);

    QString path = QApplication::applicationDirPath()+LOG_FILE_NAME;
    logFile.setFileName(path);
    logFile.open(QIODevice::ReadOnly);
    logStream.setDevice(&logFile);
    fillTable();

    colorOptions = new ColorOptions();
    colorOptions->setWindowFlag(Qt::WindowStaysOnTopHint);
    colorOptions->hide();

    connect(colorOptions,SIGNAL(saveColors(ClockColorData)),SLOT(onSaveColors(ClockColorData)));
    connect(colorOptions,SIGNAL(loadOldColors(ClockColorData)),SLOT(onLoadOldColors(ClockColorData)));

    connect(colorOptions,SIGNAL(newHourColor(QColor)      ),clock,SLOT(setHourArrowColor(QColor)));
    connect(colorOptions,SIGNAL(newMinuteColor(QColor)    ),clock,SLOT(setMinuteArrowColor(QColor)));
    connect(colorOptions,SIGNAL(newSecondColor(QColor)    ),clock,SLOT(setSecondsArrowColor(QColor)));
    connect(colorOptions,SIGNAL(newBorderColor(QColor)    ),clock,SLOT(setBorderColor(QColor)));
    connect(colorOptions,SIGNAL(newBackgroundColor(QColor)),clock,SLOT(setBackgroundColor(QColor)));
    hourFeed = 0;
    minFeed = 0;
    secFeed = 0;

    path = QApplication::applicationDirPath()+PARAMS_FILE_NAME;
    if(!QFile::exists(path))
    {
        QFile f(path);
        f.open(QIODevice::WriteOnly);
        f.close();
        ClockColorData clrs = clock->getCurrentColors();
        settings = new QSettings(path,QSettings::IniFormat);

        saveSettings(clrs);
    }
    else {
        settings = new QSettings(path,QSettings::IniFormat);
        loadSettings();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
    if(clock) delete clock;
    if(settings) delete settings;
    if(table) delete table;
    if(feed) delete feed;
    if(colorOptions) delete colorOptions;
    if(colors) delete colors;
}

void MainWindow::onSaveColors(ClockColorData data)
{
    if(clock)
    {
        clock->setColors(data);
    }
    saveSettings(data);
}

void MainWindow::onLoadOldColors(ClockColorData data)
{
    if(clock)
    {
        clock->setColors(data);
    }
}

void MainWindow::showOptions()
{
    if(colorOptions)
    {
        colorOptions->show();
    }
}

void MainWindow::showFeedOptions()
{
    if(ui->clockFeedPanel->isHidden())
{
    ui->clockFeedPanel->show();
    ui->timeEdit->setTime(clock->getCurrentTime());
    clock->pause();
}
else
{
    ui->clockFeedPanel->hide();
    clock->unpause();
}

}

void MainWindow::showTable()
{
    if(ui->dockWidget->isHidden())
    ui->dockWidget->show();
    else ui->dockWidget->hide();
}

void MainWindow::closeEvent(QCloseEvent *)
{
    if(colorOptions)
    {
        colorOptions->hide();
    }
    logFile.close();
    if(clock) saveSettings(clock->getCurrentColors());
}

void MainWindow::saveSettings(ClockColorData data)
{
    if(!settings) return;
    QTime tm;
    if(clock) tm = clock->getCurrentTime();

    hourFeed = tm.hour();
    minFeed = tm.minute();
    secFeed = tm.second();

    settings->setValue(PARAMS_FEED+"/"+PARAMS_SEC_FEED,secFeed);
    settings->setValue(PARAMS_FEED+"/"+PARAMS_MIN_FEED,minFeed);
    settings->setValue(PARAMS_FEED+"/"+PARAMS_HOUR_FEED,hourFeed);

    settings->setValue(PARAMS_COLORS+"/"+PARAMS_HOUR_CLR,data.hourClr);
    settings->setValue(PARAMS_COLORS+"/"+PARAMS_MIN_CLR,data.minuteClr);
    settings->setValue(PARAMS_COLORS+"/"+PARAMS_SEC_CLR,data.secondsClr);
    settings->setValue(PARAMS_COLORS+"/"+PARAMS_BORDER_CLR,data.borderClr);
    settings->setValue(PARAMS_COLORS+"/"+PARAMS_BACKGROUND_CLR,data.backgroundClr);

}

void MainWindow::loadSettings()
{
    if(!settings) return;

    hourFeed = settings->value(PARAMS_FEED+"/"+PARAMS_HOUR_FEED).toInt();
    minFeed = settings->value(PARAMS_FEED+"/"+PARAMS_MIN_FEED).toInt();
    secFeed = settings->value(PARAMS_FEED+"/"+PARAMS_SEC_FEED).toInt();

    QTime tm(hourFeed, minFeed, secFeed);

    ClockColorData data;
    data.hourClr =       QColor(settings->value(PARAMS_COLORS+"/"+PARAMS_HOUR_CLR).toString());
    data.minuteClr =     QColor(settings->value(PARAMS_COLORS+"/"+PARAMS_MIN_CLR).toString());
    data.secondsClr =    QColor(settings->value(PARAMS_COLORS+"/"+PARAMS_SEC_CLR).toString());
    data.borderClr =     QColor(settings->value(PARAMS_COLORS+"/"+PARAMS_BORDER_CLR).toString());
    data.backgroundClr = QColor(settings->value(PARAMS_COLORS+"/"+PARAMS_BACKGROUND_CLR).toString());

    if(colorOptions)
    {
        colorOptions->setColors(data);
    }
    if(clock)
    {
        clock->setColors(data);
        clock->setTime(tm);
    }
}

void MainWindow::fillTable()
{
    QString text = logFile.readAll();

    QStringList times = text.split("#", QString::SkipEmptyParts);

if(times.count()>0)
{
    for(int i=0;i<times.count();i++)
    {
        ui->tableWidget->insertRow(i);
        QLabel *lbl = new QLabel;
        lbl->setText(times.at(i));
        ui->tableWidget->setCellWidget(ui->tableWidget->rowCount()-1,0,lbl);
    }
}
    logFile.close();
    logFile.open(QIODevice::Append);
}

void MainWindow::appendToTable(QString str)
{
    logStream<<str+"#";
    ui->tableWidget->insertRow(ui->tableWidget->rowCount());
    QLabel *lbl = new QLabel;
    lbl->setText(str);
    ui->tableWidget->setCellWidget(ui->tableWidget->rowCount()-1,0,lbl);
}

void MainWindow::on_pb_setFeed_clicked()
{
    if(clock)
{
    appendToTable(ui->timeEdit->time().toString("hh:mm:ss"));
    clock->setTime(ui->timeEdit->time());
    ui->clockFeedPanel->hide();
    clock->unpause();
}
}

void MainWindow::on_pb_closeWindow_clicked()
{
    ui->clockFeedPanel->hide();
    clock->unpause();
}

void MainWindow::on_pb_clear_clicked()
{
    while(ui->tableWidget->rowCount()>0)
{
    ui->tableWidget->removeRow(0);
}
    logFile.close();
    logFile.open(QIODevice::WriteOnly);

    logFile.close();

    logFile.open(QIODevice::Append);
    logStream.setDevice(&logFile);
}

void MainWindow::on_actionExit_clicked()
{
    QApplication::quit();
}
