#include "mainwindow.h"
#include <QDebug>
#include <QMessageBox>
#include <exception>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
  loginForm = new login(this);
  signupForm = new signup(this);
  mainMenu = new MainMenu(this);

  setupConnections();

  showLoginForm();

  resize(400, 300);

}

MainWindow::~MainWindow()
{
  delete loginForm;
  delete signupForm;
  delete mainMenu;
}

void MainWindow::setupConnections()
{
  connect(loginForm, &login::loginAttempt, this, &MainWindow::handleLoginAttempt);
  connect(loginForm, &login::signupRequested, this, &MainWindow::showSignupForm);
  connect(signupForm, &signup::signupAttempt, this, &MainWindow::handleSignupAttempt);
  connect(signupForm, &signup::backToLoginRequested, this, &MainWindow::showLoginForm);
  connect(mainMenu, &MainMenu::logoutRequested, this, &MainWindow::handleLogout);
}

void MainWindow::showLoginForm()
{
  qDebug() << "Showing login form";
  setCentralWidget(loginForm);
  loginForm->show();
  if (signupForm) signupForm->hide();
  if (mainMenu) mainMenu->hide();
}

void MainWindow::showSignupForm()
{
  qDebug() << "Showing signup form";
  setCentralWidget(signupForm);
  signupForm->show();
  loginForm->hide();
  if (mainMenu) mainMenu->hide();
}

void MainWindow::showMainMenu(const QString& userEmail)
{
  qDebug() << "Showing main menu for user:" << userEmail;
  setCentralWidget(mainMenu);
  mainMenu->setUserEmail(userEmail,&userManager);
  mainMenu->setUserManager(&userManager);
  mainMenu->show();
  loginForm->hide();
  signupForm->hide();
}

void MainWindow::handleLoginAttempt(const QString& email, const QString& password)
{
  if (userManager.login(email.toStdString(), password.toStdString())) {
    showMainMenu(email);

  } else {
    loginForm->showErrorMessage("Invalid email or password");
  }
}

void MainWindow::handleSignupAttempt(const QString& firstName, const QString& lastName, const QString& email, const QString& password)
{
  if (userManager.signup(firstName.toStdString(), lastName.toStdString(), email.toStdString(), password.toStdString())) {
    showLoginForm();
    loginForm->showSuccessMessage("Account created successfully. Please log in.");

    // qDebug() << "Signup successful, about to show login form";
    // showLoginForm();
    // qDebug() << "Login form shown";
    // loginForm->showSuccessMessage("Account created successfully. Please log in.");
    // qDebug() << "Success message shown";

  } else {
    signupForm->showErrorMessage("Email already exists");
  }





}

void MainWindow::handleLogout()
{
  showLoginForm();
}
