#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "login.h"
#include "signup.h"
#include "usermanager.h"
#include "mainmenu.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

 private slots:
  void showLoginForm();
  void showSignupForm();
  void showMainMenu(const QString& userEmail);
  void handleLoginAttempt(const QString& email, const QString& password);
  void handleSignupAttempt(const QString& firstName, const QString& lastName, const QString& email, const QString& password);
  void handleLogout();

 // private:
 //  Ui::MainWindow *ui;

 private:
  login* loginForm;
  signup* signupForm;
  MainMenu* mainMenu;
  UserManager userManager;

  void setupConnections();
};
#endif  // MAINWINDOW_H
