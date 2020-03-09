#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    locales({"en_GB","en_US","fr_FR","es_ES","ja_JP","ko_KR","zh_CN","ru_RU"}),
    args(qApp->arguments()),
    clientProcess(this),
    exitCode(0)
{
    ui->setupUi(this);

    connect(&clientProcess, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(onProcessStop(int,QProcess::ExitStatus)));

    program = args[0];
    args.removeAt(0);

    system("start cmd /c \"move LeagueClient.exe clientmgr.exe & move og.exe LeagueClient.exe\""); // Restore real client

    parseArgs();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::parseArgs()
{
    int i = 0;
    for (QString &s: args) {
        if (s.startsWith("--locale")) {
            ui->localeCombo->setCurrentIndex(locales.indexOf(s.right(5)));
            indexOfLocale = i;
        }
        if (s.startsWith("--region")) {
            ui->regLabel->setText(s.remove(0, 9));
        }

        i++;
    }
}

void MainWindow::on_start_clicked()
{
    if (clientProcess.state() == QProcess::NotRunning) {
        // Start
        clientProcess.start(program, args);
        ui->start->setText("Stop");
        ui->stateLabel->setText("Client launched !");
    }
    else {
        // Stop
        clientProcess.kill();
        ui->start->setEnabled(false);
        ui->stateLabel->setText("Pending stop...");
    }
}

void MainWindow::on_localeCombo_currentIndexChanged(int i)
{
    args[indexOfLocale] = "--locale=" + locales[i];
}

void MainWindow::onProcessStop(int code, QProcess::ExitStatus)
{
    exitCode = code;
    ui->start->setText("Start");
    ui->stateLabel->setText("Client stopped.");
    ui->start->setEnabled(true);
}
