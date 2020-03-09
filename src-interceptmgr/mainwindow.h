#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QStringList>

#include <QString>
#include <QList>

#include <QProcess>

#include <windows.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private:
    Ui::MainWindow *ui;

    void parseArgs();

    QString region, locale;

    const QList<QString> locales;

    QStringList args;
    QString program;

    QProcess clientProcess;

    int indexOfLocale;

    int exitCode;

private slots:
    void on_start_clicked();
    void on_localeCombo_currentIndexChanged(int);

    void onProcessStop(int, QProcess::ExitStatus);

};

#endif // MAINWINDOW_H
