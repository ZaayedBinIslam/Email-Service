#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>

namespace Ui {
class login;
}

class login : public QWidget {
  Q_OBJECT

 public:
  explicit login(QWidget *parent = nullptr);
  ~login();

  void showErrorMessage(const QString& message);
  void showSuccessMessage(const QString& message);

 signals:
  void loginAttempt(const QString& email, const QString& password);
  void signupRequested();

 // private slots:
 //  void on_loginButton_clicked();
 //  void on_signupButton_clicked();

 private slots:
  void on_pushButton_login_login_clicked();

  void on_pushButton_login_signup_clicked();

 private:
  Ui::login *ui;
};

#endif  // LOGIN_H
