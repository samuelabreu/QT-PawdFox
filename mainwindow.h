#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QSettings>
#include <QTableView>
#include <QComboBox>
#include <QSettings>

#ifdef DUMBLIB
#include "temp.h"
#else
#include "libpawdfox.h"
#endif

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private Q_SLOTS:
    void on_profilesComboBox_currentIndexChanged(int index);
    void on_searchEdit_textChanged(const QString &arg1);
    void on_passwordOkButton_clicked();
    void on_actionAbout_triggered();
    void on_tableView_customContextMenuRequested(const QPoint &pos);
    void slotVisitSite();
    void slotCopyAddress();
    void slotCopyUsername();
    void slotCopyPassword();
    void slotRevealPassword();

protected:
    void closeEvent(QCloseEvent *event);

private:
    libpawdfox::PawdFox *pfox = new libpawdfox::PawdFox();
    Ui::MainWindow *ui;
    QStandardItemModel *model;
    QSettings *settings;
    QMenu *tableMenu;
    QVector<libpawdfox::firefox_credential> current;

    // UI Methods
    void initUi();
    void populateTableView();
    void resetUi();
    void resetProfileUi();
    void showInfo(int i);
//    void pickProfileIni(); // Usuário escolher onde tá o profile.ini
    void askPassword();
    void buzz();
    void copyToClipboard(std::string value);
    // Logic Methods
    void init();
    int openProfileIni(QString path);
    void selectProfile(libpawdfox::firefox_profile profile);
    int openProfile(libpawdfox::firefox_profile profile);
};

#endif // MAINWINDOW_H
