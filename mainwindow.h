#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <clockwindow.h>
#include <QSettings>
#include <QFile>
#include <QCloseEvent>
#include <QTextStream>
#include <QStringList>
#include <QTableWidgetItem>
#include <QLabel>
#include <ColorOptions.h>

#define LOG_FILE_NAME       "/log.txt"
#define PARAMS_FEED             QString("FEED")
#define PARAMS_SEC_FEED         QString("Sec_feed")
#define PARAMS_MIN_FEED         QString("Min_feed")
#define PARAMS_HOUR_FEED        QString("Hour_feed")
#define PARAMS_COLORS           QString("COLORS")
#define PARAMS_HOUR_CLR         QString("Hour_clr")
#define PARAMS_MIN_CLR          QString("Min_clr")
#define PARAMS_SEC_CLR          QString("Sec_clr")
#define PARAMS_BORDER_CLR       QString("Border_clr")
#define PARAMS_BACKGROUND_CLR   QString("Background_clr")
#define PARAMS_FILE_NAME    "/params.ini"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void onSaveColors(ClockColorData data);
    void onLoadOldColors(ClockColorData data);

    void showOptions();
    void showFeedOptions();
    void showTable();

    void on_pb_setFeed_clicked();
    void on_pb_closeWindow_clicked();
    void on_pb_clear_clicked();
    void on_actionExit_clicked();

protected:
    void closeEvent(QCloseEvent *);
private:
    Ui::MainWindow *ui;
    ClockWindow *clock;
    QSettings *settings;
    QFile logFile;
    QTextStream logStream;
    QAction *feed, *colors, *table;
    int hourFeed, minFeed, secFeed;
    ColorOptions *colorOptions;

    void saveSettings(ClockColorData data);
    void loadSettings();
    void fillTable();
    void appendToTable(QString str);

};

#endif // MAINWINDOW_H
