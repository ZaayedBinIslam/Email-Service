#include "login.h"

#include "ui_login.h"
#include <QMessageBox>

login::login(QWidget *parent) : QWidget(parent), ui(new Ui::login) {
  ui->setupUi(this);
}

login::~login() { delete ui; }

// void login::on_loginButton_clicked()
// {
//   QString email = ui->emailLineEdit->text();
//   QString password = ui->passwordLineEdit->text();
//   emit loginAttempt(email, password);
// }

// void login::on_signupButton_clicked()
// {
//   emit signupRequested();
// }

void login::showErrorMessage(const QString &message)
{
  QMessageBox::warning(this, "Error", message);
}

void login::showSuccessMessage(const QString &message)
{
  QMessageBox::information(this, "Success", message);
}

void login::on_pushButton_login_login_clicked()
{
    QString email = ui->lineEdit_login_mail->text();
    QString password = ui->lineEdit_login_pass->text();
    emit loginAttempt(email, password);
}


void login::on_pushButton_login_signup_clicked()
{
    emit signupRequested();
}

