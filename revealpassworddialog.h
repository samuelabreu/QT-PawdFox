#ifndef REVEALPASSWORDDIALOG_H
#define REVEALPASSWORDDIALOG_H

#include <QDialog>

namespace Ui {
class RevealPasswordDialog;
}

class RevealPasswordDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RevealPasswordDialog(QWidget *parent = 0);
    ~RevealPasswordDialog();
    void setPasswordLabel(const std::string &value);
private Q_SLOTS:
    void on_copyPasswordButton_clicked();

private:
    Ui::RevealPasswordDialog *ui;
};

#endif // REVEALPASSWORDDIALOG_H
