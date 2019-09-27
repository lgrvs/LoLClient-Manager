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
#define TOOL_HASH "b39f1d3061f2b091c1700c6cd2bc823ca949f4cbe10671f1b60e8023a92fa708"

#endif // WIDGET_H
