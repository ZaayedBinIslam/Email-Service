#ifndef SIGNUP_H
#define SIGNUP_H

#include <QWidget>
#include <QString>

namespace Ui {
class signup;
}

class signup : public QWidget {
  Q_OBJECT

 public:
  explicit signup(QWidget *parent = nullptr);
  ~signup();

  void showErrorMessage(const QString& message);

 signals:
  void signupAttempt(const QString& firstName, const QString& lastName, const QString& email, const QString& password);
  void backToLoginRequested();

 // private slots:
 //  void on_signupButton_clicked();
 //  void on_backButton_clicked();

 private slots:
  void on_pushButton_signup_clicked();

  void on_pushButton_signup_gotologin_clicked();

 private:
  Ui::signup *ui;
};

#endif  // SIGNUP_H
