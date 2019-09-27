#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget),
    lolPathSpecified(false),
    localechangerExtracted(false),
    appSettings(QSettings::NativeFormat, QSettings::UserScope, "lol_tool_lang", "lol_tool", this),
    locales({"en_GB", "en_US", "fr_FR", "es_ES", "ko_KR", "ja_JP", "zh_CN"})
{
    ui->setupUi(this);

    loadSettings();

}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_select_folder_button_clicked()
{
    QString path = QFileDialog::getExistingDirectory(this, "Select your LoL installation folder", "C:\\");
    if (check_Path(path)) {
        appSettings.setValue(PATH_KEY, path);
        //appSettings.sync();
    }
    return;
}
bool Widget::check_Path(QString path)
{
    bool r=false;
    if (!path.isEmpty()) {
        switch (fileExists(path + "/LeagueClient.exe")) {
        case Nonexistent:
            QMessageBox::critical(this, "Error", "The folder you selected is not an LoL Live servers folder.");
            r=false;
            break;
        case Exists_WDenied:
            QMessageBox::warning(this, "Warning", "The folder you selected seems to be a valid LoL folder,\nbut we can't access it. Try closing your League client or running this program with administrator privileges, or using Fast Fix if you enabled the tool but did not use it.");
            ui->folder_line->setText(path);
            r=false;
            break;
        default:
            ui->folder_line->setText(path);
            ui->extract_button->setEnabled(true);
            ui->enable_button->setEnabled(false);
            r=true;
            break;
        }
    }
    return r;
}

bool Widget::check_extraction()
{
    QString dest = ui->folder_line->text() + "/loc_selection.exe";
    if (fileExists(dest)) {
        if (fileChecksum(dest, QCryptographicHash::Sha256).toHex() == TOOL_HASH)
            return true;
        if (!QFile::remove(dest)){
            QMessageBox::critical(this, "Error", "The previously extracted tool has been modified but can't be erased.\nPlease try extracting it again.");
            qApp->quit();
        }
    }
    return false;
}

bool Widget::extractTool()
{
    if (fileChecksum("://resources/loc_selection.exe", QCryptographicHash::Sha256).toHex() != TOOL_HASH) {
        QMessageBox::critical(this, "Can't proceed", "It seems parts of this executable have been modified and can no longer be used. Please re-download the program.");
        return false;
    }
    QString dest = ui->folder_line->text() + "/loc_selection.exe";
    if (QFile::exists(dest))
        if (!QFile::remove(dest)) {
            QMessageBox::critical(this, "Error", "Extraction failed. Please try again with admin rights if the issue persists.");
            return false;
        }
    return QFile::copy("://resources/loc_selection.exe", dest);
}

void Widget::on_how_to_button_clicked()
{
    QMessageBox::information(this, "How to use the locale changer", "The first time you use the locale-changer, you have to extract it first by clicking the Extract button\n"
                             "Note: LoL folder path is needed for extraction.\nOnce extraction is complete, you have to enable locale-changer every time you want to use it.\n"
                             "Launch the Riot Client as usual, and let it update the client. Once the client is updated, and BEFORE logging in your account, enable locale-changer.\n"
                             "Note: locale-changer can't work if you enable automatic log in, and you have to enable it every time.");
}

void Widget::loadSettings()
{
    if (!appSettings.value(PATH_KEY).toString().isEmpty()) {
        if (check_Path(appSettings.value(PATH_KEY).toString())) {
            if (appSettings.value(EXTRACTION_STATUS).toBool() == true) {
                if (!check_extraction()) {
                    QMessageBox::warning(this, "Error", "An error was encountered with the previously extracted tool. Please extract it again.");
                    ui->extract_button->setEnabled(true);
                }
                else {
                    ui->enable_button->setEnabled(true);
                    ui->extract_button->setEnabled(false);
                }
            }
        }
    }
}

void Widget::on_extract_button_clicked()
{
    if (extractTool()) {
        ui->extract_button->setEnabled(false);
        ui->enable_button->setEnabled(true);
        appSettings.setValue(EXTRACTION_STATUS, true);
    }
}

void Widget::on_enable_button_clicked()
{
    QString file = ui->folder_line->text() + "/LeagueClient.exe";
    QString dest = ui->folder_line->text() + "/og.exe";
    QString txtf = ui->folder_line->text() + "/chosen_locale.txt";

    if (QFile::exists(txtf)) // WRITE LOCALE TO TXT FILE
        if (!QFile::remove(txtf))
        {
            QMessageBox::critical(this, "Error", "Couldn't access LoL path. Maybe missing permission ?");
            return;
        }

    QFile txtfile(txtf);
          if(txtfile.open(QIODevice::WriteOnly | QIODevice::Text))
          {
              QTextStream stream(&txtfile);

              stream << locales.value(ui->comboBox->currentIndex());

              txtfile.close();
          }

    if (!QFile::rename(file, dest)) { //RENAMES
        QMessageBox::critical(this, "Error", "Couldn't access LoL path. Maybe missing permission ?");
        return;
    }
    file = ui->folder_line->text() + "/loc_selection.exe";
    dest = ui->folder_line->text() + "/LeagueClient.exe";
    if (!QFile::rename(file, dest)) {
        QMessageBox::critical(this, "Error", "Couldn't access extracted path. Disabling the tool...");
        dest = ui->folder_line->text() + "/LeagueClient.exe";
        file = ui->folder_line->text() + "/og.exe";
        if (!QFile::rename(file, dest)) {
            QMessageBox::critical(this, "Error", "Couldn't restore your LoL client. Your LoL client might be missing. If it won't start anymore, rename og.exe back to LeagueClient.exe in your LoL folder, or contact the publisher for assistance.");
        }
        return;
    }
    QMessageBox::information(this, "Success", "The LoL language changer has been successfully enabled. You can now close this window.\n"
                             "If the client doesn't launch anymore, use Fast Fix");
    ui->enable_button->setEnabled(false);
    return;
}

void Widget::on_fastfix_button_clicked()
{
    if (QMessageBox::question(this, "Attention", "This option is only to be used if your LoL installation was broken because of the tool. Is it the case ?") == QMessageBox::Yes) {
        QString file = ui->folder_line->text() + "/LeagueClient.exe";
        QString dest = ui->folder_line->text() + "/og.exe";
        QString tool = ui->folder_line->text() + "/loc_selection.exe";
        // Could add file exist checks, but seems unnecessary, the user's been warned
        QFile::rename(file, tool);
        QFile::rename(dest, file);
    }
}


FileStatus fileExists(QString path) {
    QFileInfo check_file(path);
    // check if file exists and if yes: Is it really a file and no directory?
    if (check_file.exists() && check_file.isFile()) {
        if (check_file.isWritable())
            return Exists_Writable;
        return Exists_WDenied;
    } else {
        return Nonexistent;
    }
}

QByteArray fileChecksum(const QString &fileName,
                        QCryptographicHash::Algorithm hashAlgorithm)
{
    QFile f(fileName);
    if (f.open(QFile::ReadOnly)) {
        QCryptographicHash hash(hashAlgorithm);
        if (hash.addData(&f)) {
            return hash.result();
        }
    }
    return QByteArray();
}
