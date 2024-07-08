#ifndef MAINMENU_H
#define MAINMENU_H

#include <QWidget>
#include <QString>
#include "qtablewidget.h"
#include "usermanager.h"

namespace Ui {
class MainMenu;
}

class MainMenu : public QWidget {
  Q_OBJECT

 public:
  explicit MainMenu(QWidget *parent = nullptr);
  ~MainMenu();

  void setUserEmail(const QString& email,UserManager* manager);
  void setUserManager(UserManager* manager);

 signals:
  void logoutRequested();

 // private slots:
 //  void on_logoutButton_clicked();

 // private slots:
 //  void on_compose_userEmail_linkHovered(const QString &link);

 private slots:
  void on_pushButton_compose_sendMail_clicked();

  void refreshInbox(UserManager* manager);
  void onInboxItemDoubleClicked(QTableWidgetItem* item);

  void refreshSentMail(UserManager* manager);
  void onSentMailItemDoubleClicked(QTableWidgetItem* item);

  void on_logout_clicked();

 private:
  Ui::MainMenu *ui;
  QString userEmail;

  UserManager* userManager;

  void clearComposeFields();
  void showEmailDialog(const Email& email);
  void showSentEmailDialog(const Email& email);
};

#endif  // MAINMENU_H
