#include "signup.h"

#include "ui_signup.h"

#include <QMessageBox>

signup::signup(QWidget *parent) : QWidget(parent), ui(new Ui::signup) {
  ui->setupUi(this);
}

signup::~signup() { delete ui; }

// void Signup::on_signupButton_clicked()
// {
//   QString firstName = ui->firstNameLineEdit->text();
//   QString lastName = ui->lastNameLineEdit->text();
//   QString email = ui->emailLineEdit->text();
//   QString password = ui->passwordLineEdit->text();

//   emit signupAttempt(firstName, lastName, email, password);
// }

// void Signup::on_backButton_clicked()
// {
//   emit backToLoginRequested();
// }

void signup::showErrorMessage(const QString &message)
{
  QMessageBox::warning(this, "Error", message);
}

void signup::on_pushButton_signup_clicked()
{
    QString firstName = ui->lineEdit_signup_fname->text();
    QString lastName = ui->LineEdit_signup_lname->text();
    QString email = ui->lineEdit_signup_mail->text();
    QString password = ui->lineEdit_signup_pass->text();
    QString passwordReenter = ui->lineEdit_signup_pass_reenter->text();

    if(password==passwordReenter)
    {
      emit signupAttempt(firstName, lastName, email, password);
    }
    else
      QMessageBox::warning(this, "Error", "Passwords didn't match,Please Retry");
}


void signup::on_pushButton_signup_gotologin_clicked()
{
    emit backToLoginRequested();
}

