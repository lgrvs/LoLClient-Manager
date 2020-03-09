#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QFileDialog>
#include <QString>
#include <QFileInfo>
#include <QMessageBox>
#include <QSettings>
#include <QStringList>
#include <QCryptographicHash>
#include <QByteArray>
#include <QFile>
#include <QList>
#include <QTextStream>

#include <QDebug>

enum FileStatus {
    Exists_Writable,
    Exists_WDenied,
    Nonexistent
};

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT
    
public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    
private:
    Ui::Widget *ui;
    bool check_Path(QString path);
    bool check_extraction();
    bool extractTool();
    void loadSettings();


    bool lolPathSpecified, localechangerExtracted;
    QSettings appSettings;
    QList<QString> locales;




private slots:
    void on_select_folder_button_clicked();
    void on_how_to_button_clicked();
    void on_extract_button_clicked();
    void on_enable_button_clicked();
    void on_fastfix_button_clicked();
};

FileStatus fileExists(QString path);
QByteArray fileChecksum(const QString &fileName,
                        QCryptographicHash::Algorithm hashAlgorithm);

// Settings keys
#define PATH_KEY "path"
#define EXTRACTION_STATUS "extracted"
#define TOOL_HASH "de60b16c0b6f4785ab2ed345e3feb65ada79e1caae14dc487f680a59d06329c2"

#endif // WIDGET_H
