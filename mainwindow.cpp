#include <QTableView>
#include <QStandardItemModel>
#include <QTime>
#include <QClipboard>
#include <QDesktopServices>
#include <QUrl>

#include <iostream>

#include "aboutdialog.h"
#include "revealpassworddialog.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"

#ifdef DUMBLIB
#include "temp.h"
#else
#include "libpawdfox.h"
#define emptyString empty
#endif

using namespace libpawdfox;
using namespace std;

#define NOPROFILESINI 1
#define NOPROFILES 2
#define PROFILECLOSEFAIL 3
#define PROFILEEMPTY 4

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->initUi();
    this->init();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete model;
    delete pfox;
}

void MainWindow::initUi()
{
    settings = new QSettings("com.gmail.sdepaula", "PawdFox");
    QByteArray g = settings->value("geometry").toByteArray();
    if (g.length() > 0)
        restoreGeometry(g);
    this->resetUi();
    ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);
    model = new QStandardItemModel(0, 2);
    model->setHorizontalHeaderItem(0, new QStandardItem(tr("Hostname")));
    model->setHorizontalHeaderItem(1, new QStandardItem(tr("Username")));
    ui->tableView->setModel(model);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void MainWindow::resetUi()
{
    ui->profilesComboBox->setEnabled(false);
    this->resetProfileUi();
}

void MainWindow::resetProfileUi()
{
    //TODO: showinfo mensagem inicial.
    ui->infoFrame->hide();
    ui->tableView->show();
    ui->passwordFrame->hide();
    ui->tableView->setEnabled(false);
    ui->searchEdit->setEnabled(false);
}

void MainWindow::on_profilesComboBox_currentIndexChanged(int index)
{
    model->removeRows(0, model->rowCount());
    pfox->credentials.clear();
    firefox_profile profile = pfox->profiles[index];
    selectProfile(profile);
}

void MainWindow::populateTableView()
{
    ui->searchEdit->setEnabled(true);
    ui->tableView->setEnabled(true);
    for (int i=0; i < current.count(); ++i)
    {
        QStandardItem *hostnameRow = new QStandardItem(QString(current[i].hostname.c_str()));
        QStandardItem *usernameRow = new QStandardItem(QString(current[i].username.c_str()));
        model->setItem(i,0,hostnameRow);
        model->setItem(i,1,usernameRow);
    }
    ui->searchEdit->setFocus();
}

void MainWindow::on_searchEdit_textChanged(const QString &query)
{
    model->removeRows(0, model->rowCount());
    current = QVector<firefox_credential>::fromStdVector(pfox->Filter(query.toStdString()));
    this->populateTableView();
}

void MainWindow::showInfo(int i)
{
    ui->tableView->hide();
    ui->passwordFrame->hide();
    ui->infoFrame->show();
    switch (i) {
    case NOPROFILESINI:
        ui->messageLabel->setText("Unfortunately i couldn't find a valid profiles.ini in the default place.\nPlease make sure Firefox is installed and you have at least one profile with saved accounts.");
        break;
    case NOPROFILES:
        ui->messageLabel->setText("Unfortunately i couldn't find any profile in you Firefox installation.\nPlease make sure Firefox is installed and you have at least one profile with saved accounts.");
        break;
    case PROFILECLOSEFAIL:
        ui->messageLabel->setText("A unknown error ocurred while i was trying to close the current firefox profile, please close and try again!");
        break;
    case PROFILEEMPTY:
        ui->messageLabel->setText("Selected profile doesn't have any saved password!");
        break;
    }
}

void MainWindow::askPassword()
{
    ui->tableView->hide();
    ui->infoFrame->hide();
    ui->passwordFrame->show();
    QPalette *palette = new QPalette();
    palette->setColor(QPalette::Text,Qt::black);
    ui->passwordLineEdit->setPalette(*palette);
    ui->passwordLineEdit->setFocus();
}

void MainWindow::on_passwordOkButton_clicked()
{
    int index = ui->profilesComboBox->currentIndex();
    firefox_profile profile = pfox->profiles[index];
    profile.password = ui->passwordLineEdit->text().toStdString();
    int status = openProfile(profile);
    if (status == LIBPAWD_STATUS_WRONG_PASSWORD)
    {
        QPalette *palette = new QPalette();
        palette->setColor(QPalette::Text,Qt::red);
        ui->passwordLineEdit->setPalette(*palette);
        this->buzz();
    } else
    {
        ui->passwordLineEdit->setText(QString(""));
    }
}

void MainWindow::buzz()
{
  int xp = x();
  int yp = y();
  QTime t;

  t.start();
  for ( int i = 32; i > 0; )
  {
    QApplication::processEvents();
    if ( t.elapsed() >= 1 )
    {
      int delta = i >> 2;
      int dir = i & 3;
      int dx = ((dir==1)||(dir==2)) ? delta : -delta;
      int dy = (dir<2) ? delta : -delta;
      move( xp+dx, yp+dy );
      t.restart();
      i--;
    }
  }
  move( xp, yp );
}

void MainWindow::on_actionAbout_triggered()
{
    AboutDialog *about = new AboutDialog(this);
    about->setAttribute( Qt::WA_DeleteOnClose );
    about->show();

}

void MainWindow::closeEvent(QCloseEvent *event)
{
    settings->setValue("geometry", saveGeometry());
}

void MainWindow::on_tableView_customContextMenuRequested(const QPoint &pos)
{
    if (tableMenu == NULL)
    {
        QAction *visitSiteAction = new QAction(tr("Visitar Site"), this);
        connect(visitSiteAction, SIGNAL(triggered()), this, SLOT(slotVisitSite()));

        QAction *copyAddressAction = new QAction(tr("Copiar Endereço"), this);
        connect(copyAddressAction, SIGNAL(triggered()), this, SLOT(slotCopyAddress()));
        QAction *copyUserAction = new QAction(tr("Copiar Usuário"), this);
        connect(copyUserAction, SIGNAL(triggered()), this, SLOT(slotCopyUsername()));
        QAction *copyPasswordAction = new QAction(tr("Copiar Senha"), this);
        connect(copyPasswordAction, SIGNAL(triggered()), this, SLOT(slotCopyPassword()));

        QAction *revealPasswordAction = new QAction(tr("Revelar Senha"), this);
        connect(revealPasswordAction, SIGNAL(triggered()), this, SLOT(slotRevealPassword()));

        tableMenu = new QMenu(this);
        tableMenu->addAction(visitSiteAction);
        tableMenu->addSeparator();
        tableMenu->addAction(copyAddressAction);
        tableMenu->addAction(copyUserAction);
        tableMenu->addAction(copyPasswordAction);
        tableMenu->addSeparator();
        tableMenu->addAction(revealPasswordAction);
    }
    tableMenu->popup(ui->tableView->viewport()->mapToGlobal(pos));
}

void MainWindow::slotVisitSite()
{
    int row = ui->tableView->selectionModel()->currentIndex().row();
    QString link = QString::fromStdString(current[row].hostname);
    QDesktopServices::openUrl(QUrl(link));
}

void MainWindow::slotCopyAddress()
{
    int row = ui->tableView->selectionModel()->currentIndex().row();
    this->copyToClipboard(current[row].hostname);
}

void MainWindow::slotCopyUsername()
{
    int row = ui->tableView->selectionModel()->currentIndex().row();
    this->copyToClipboard(current[row].username);
}

void MainWindow::slotCopyPassword()
{
    int row = ui->tableView->selectionModel()->currentIndex().row();
    this->copyToClipboard(pfox->GetPassword(current[row].encrypted_password));
}

void MainWindow::slotRevealPassword()
{
    int row = ui->tableView->selectionModel()->currentIndex().row();
    RevealPasswordDialog *reveal = new RevealPasswordDialog(this);
    reveal->setAttribute( Qt::WA_DeleteOnClose );
    reveal->setPasswordLabel(pfox->GetPassword(current[row].encrypted_password));
    reveal->show();
}

void MainWindow::copyToClipboard(string value)
{
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(QString::fromStdString(value));
}

// LOGIC

void MainWindow::init() // Método quando abre o programa.
{
    int status = this->openProfileIni(QString(emptyString.c_str()));
    if (status == LIBPAWD_STATUS_UNKNOWN)
    {
        this->showInfo(NOPROFILESINI);
        //TODO: Pedir pra escolher onde tá o profiles.ini
    }
    else
    {
        QVector<firefox_profile> profiles = QVector<firefox_profile>::fromStdVector(pfox->profiles);
        for (int i=0;i < profiles.count(); i++)
        {
            ui->profilesComboBox->addItem(QString(profiles[i].name.c_str()));
        }
        if (profiles.count() > 0) {
            ui->profilesComboBox->setEnabled(true);
        } else {
            //TODO: Pedir pra escolher outro profiles.ini
            this->showInfo(NOPROFILES);
        }
    }
}

// Tenta abrir o profile.ini
int MainWindow::openProfileIni(QString path)
{
    return pfox->OpenIni(path.toStdString());
}

void MainWindow::selectProfile(firefox_profile profile)
{
    int status = openProfile(profile);
    if (status == LIBPAWD_STATUS_WRONG_PASSWORD)
    {
        this->askPassword();
    }
}

int MainWindow::openProfile(firefox_profile profile)
{
    int closeStatus = pfox->CloseProfile();
    if (closeStatus == LIBPAWD_STATUS_SUCCESS)
    {
        int status = pfox->ReadLogins(profile);
        switch (status)
        {
        case LIBPAWD_STATUS_SUCCESS:
            this->resetProfileUi();
            current = QVector<firefox_credential>::fromStdVector(pfox->credentials);
            this->populateTableView();
            break;
        case LIBPAWD_STATUS_FAIL_SHUTDOWN:
            this->showInfo(PROFILECLOSEFAIL);
            break;
        case LIBPAWD_STATUS_DECRYPT_FAILED:
            //TODO: Mensagem de erro no decrypt
            break;
        case LIBPAWD_STATUS_FILE_DOESNT_EXIST:
            //TODO: Mensagem de erro de arquivo logins.json não existe
            this->showInfo(PROFILEEMPTY);
            break;
        case LIBPAWD_STATUS_WRONG_PASSWORD:
            break;
        default:
            cout << "Erro genérico" << endl;
            break;
            //TODO: Mensagem de erro genérica
        }
        return status;
    } else {
        return closeStatus;
    }
}
