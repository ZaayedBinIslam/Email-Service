#include "mainmenu.h"

#include "ui_mainmenu.h"
#include <QMessageBox>
#include <QDebug>
#include <QMessageBox>
#include <QDialog>
#include <QVBoxLayout>
#include <QLabel>
#include <QTextEdit>
#include <algorithm>

MainMenu::MainMenu(QWidget *parent) : QWidget(parent), ui(new Ui::MainMenu), userManager(nullptr) {
  ui->setupUi(this);
  connect(ui->tableWidget_inbox, &QTableWidget::itemDoubleClicked, this, &MainMenu::onInboxItemDoubleClicked);
  connect(ui->tableWidget_sentMail, &QTableWidget::itemDoubleClicked, this, &MainMenu::onSentMailItemDoubleClicked);
}

MainMenu::~MainMenu() { delete ui; }

void MainMenu::setUserEmail(const QString& email,UserManager* manager)
{
  userEmail = email;
  QLabel *label = ui->tabCompose->findChild<QLabel*>("compose_userEmail");
  // QLabel *username = ui->tabProfile->findChild<QLabel*>("labelUsername");
  qDebug() << "debug *";
  label->setText(email);
  // username->setText(email);
  qDebug() << "debug 1";
  refreshInbox(manager);
  refreshSentMail(manager);
  qDebug() << "debug 2";
}

void MainMenu::setUserManager(UserManager* manager)
{
  userManager = manager;
}

void MainMenu::on_pushButton_compose_sendMail_clicked()
{
  if (!userManager) {
    QMessageBox::warning(this, "Error", "User manager not initialized");
    return;
  }

  QString to = ui->tabCompose->findChild<QLineEdit*>("lineEdit_compose_to")->text();
  QString subject = ui->tabCompose->findChild<QLineEdit*>("lineEdit_compose_subject")->text();
  QString body = ui->tabCompose->findChild<QTextEdit*>("textEdit_compose_emailBody")->toPlainText();


  if (to.isEmpty() || subject.isEmpty() || body.isEmpty()) {
    QMessageBox::warning(this, "Error", "Please fill all fields");
    return;
  }

  if (userManager->sendEmail(userEmail.toStdString(), to.toStdString(), subject.toStdString(), body.toStdString())) {
     qDebug()<<"Debug mail"<<to<<" "<<subject<<" "<<body;
    QMessageBox::information(this, "Success", "Email sent successfully");
     refreshInbox(userManager);
     refreshSentMail(userManager);
    clearComposeFields();
  } else {
    QMessageBox::warning(this, "Error", "Failed to send email");
  }
}

void MainMenu::clearComposeFields()
{
  // toLineEdit->clear();
  ui->tabCompose->findChild<QLineEdit*>("lineEdit_compose_to")->clear();
  // subjectLineEdit->clear();
  ui->tabCompose->findChild<QLineEdit*>("lineEdit_compose_subject")->clear();
  // bodyTextEdit->clear();
  ui->tabCompose->findChild<QTextEdit*>("textEdit_compose_emailBody")->clear();
}

// void MainMenu::on_logoutButton_clicked()
// {
//   emit logoutRequested();
// }

void MainMenu::refreshInbox(UserManager* manager) {
  qDebug() << "debug 3";
  setUserManager(manager);
  if (!userManager) {
    qDebug()<<"not found";
    return;}

  QTableWidget* inboxTable = ui->tableWidget_inbox;  // Assuming this is the name in your UI
  // inboxTable->setItem(1,1,new QTableWidgetItem("test"));
  inboxTable->setRowCount(0);  // Clear existing rows

  std::vector<Email> inbox = userManager->getInbox(userEmail.toStdString());
  qDebug() << "debug 4";
  for (const auto& email : inbox) {
    // qDebug()<<QString::fromStdString(email.getSender());

    int row = inboxTable->rowCount();
    inboxTable->insertRow(row);
    inboxTable->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(email.getSender())));
    inboxTable->setItem(row, 1, new QTableWidgetItem(email.getIsRead() ? "Yes" : "No"));
    inboxTable->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(email.getFormattedTime())));
    inboxTable->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(email.getSubject())));
  }
}

void MainMenu::onInboxItemDoubleClicked(QTableWidgetItem* item) {
  if (!userManager) return;

  QTableWidget* inboxTable = ui->tableWidget_inbox;
  int row = item->row();
  QString sender = inboxTable->item(row, 0)->text();
  QString time = inboxTable->item(row, 2)->text();
  QString subject = inboxTable->item(row, 3)->text();

  std::vector<Email> inbox = userManager->getInbox(userEmail.toStdString());
  auto it = std::find_if(inbox.begin(), inbox.end(), [&](const Email& e) {
    return e.getSender() == sender.toStdString() &&
           e.getFormattedTime() == time.toStdString() &&
           e.getSubject() == subject.toStdString();
  });

  if (it != inbox.end()) {
    showEmailDialog(*it);
    if (!it->getIsRead()) {
      it->markAsRead();
      inboxTable->item(row, 1)->setText("Yes");
      userManager->updateEmail(*it);
    }
  }
}

void MainMenu::showEmailDialog(const Email& email) {
  QDialog dialog(this);
  dialog.setWindowTitle("Email Details");
  dialog.setMinimumSize(400, 300);

  QVBoxLayout* layout = new QVBoxLayout(&dialog);

  layout->addWidget(new QLabel("From: " + QString::fromStdString(email.getSender())));
  layout->addWidget(new QLabel("To: " + QString::fromStdString(email.getReceiver())));
  layout->addWidget(new QLabel("Subject: " + QString::fromStdString(email.getSubject())));
  layout->addWidget(new QLabel("Time: " + QString::fromStdString(email.getFormattedTime())));

  QTextEdit* bodyText = new QTextEdit();
  bodyText->setPlainText(QString::fromStdString(email.getMailText()));
  bodyText->setReadOnly(true);
  layout->addWidget(bodyText);

  dialog.exec();
}


void MainMenu::refreshSentMail(UserManager* manager) {
  setUserManager(manager);
  if (!userManager) {
    qDebug() << "UserManager is null in refreshSentMail";
    return;
  }

  QTableWidget* sentMailTable = ui->tableWidget_sentMail;
  if (!sentMailTable) {
    qDebug() << "Sent mail table widget not found";
    return;
  }

  sentMailTable->setRowCount(0);  // Clear existing rows

  std::vector<Email> allMails = userManager->getAllEmails();
  std::vector<Email> sentMails;

          // Filter emails sent by the logged-in user
  for (const auto& email : allMails) {
    if (email.getSender() == userEmail.toStdString()) {
      sentMails.push_back(email);
    }
  }

  qDebug() << "Number of sent emails:" << sentMails.size();

  for (const auto& email : sentMails) {
    int row = sentMailTable->rowCount();
    sentMailTable->insertRow(row);
    sentMailTable->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(email.getReceiver())));
    sentMailTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(email.getFormattedTime())));
    sentMailTable->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(email.getSubject())));
  }

  qDebug() << "Sent mail table rows:" << sentMailTable->rowCount();

}

void MainMenu::onSentMailItemDoubleClicked(QTableWidgetItem* item) {
  if (!userManager) return;

  QTableWidget* sentMailTable = ui->tableWidget_sentMail;
  int row = item->row();
  QString receiver = sentMailTable->item(row, 0)->text();
  QString time = sentMailTable->item(row, 1)->text();
  QString subject = sentMailTable->item(row, 2)->text();

  std::vector<Email> allMails = userManager->getAllEmails();
  auto it = std::find_if(allMails.begin(), allMails.end(), [&](const Email& e) {
    return e.getSender() == userEmail.toStdString() &&
           e.getReceiver() == receiver.toStdString() &&
           e.getFormattedTime() == time.toStdString() &&
           e.getSubject() == subject.toStdString();
  });

  if (it != allMails.end()) {
    showSentEmailDialog(*it);
  }
}

void MainMenu::showSentEmailDialog(const Email& email) {
  QDialog dialog(this);
  dialog.setWindowTitle("Sent Email Details");
  dialog.setMinimumSize(400, 300);

  QVBoxLayout* layout = new QVBoxLayout(&dialog);

  layout->addWidget(new QLabel("To: " + QString::fromStdString(email.getReceiver())));
  layout->addWidget(new QLabel("Subject: " + QString::fromStdString(email.getSubject())));
  layout->addWidget(new QLabel("Time: " + QString::fromStdString(email.getFormattedTime())));

  QTextEdit* bodyText = new QTextEdit();
  bodyText->setPlainText(QString::fromStdString(email.getMailText()));
  bodyText->setReadOnly(true);
  layout->addWidget(bodyText);

  // QPushButton* closeButton = new QPushButton("Close");
  // layout->addWidget(closeButton);
  // connect(closeButton, &QPushButton::clicked, &dialog, &QDialog::accept);

  dialog.setLayout(layout);
  dialog.exec();
}




void MainMenu::on_logout_clicked()
{
    emit logoutRequested();
}

