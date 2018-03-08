#include <QClipboard>

#include "revealpassworddialog.h"
#include "ui_revealpassworddialog.h"

RevealPasswordDialog::RevealPasswordDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RevealPasswordDialog)
{
    ui->setupUi(this);
}

RevealPasswordDialog::~RevealPasswordDialog()
{
    delete ui;
}

void RevealPasswordDialog::setPasswordLabel(const std::string &value)
{
    ui->passwordRevealLabel->setText(QString::fromStdString(value));
}

void RevealPasswordDialog::on_copyPasswordButton_clicked()
{
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(ui->passwordRevealLabel->text());
}
