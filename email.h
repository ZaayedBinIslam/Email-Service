//non ui class
#ifndef EMAIL_H
#define EMAIL_H

#include <string>
#include <ctime>

class Email {
 public:
  Email();

  Email(int id, const std::string& from, const std::string& to, const std::string& subj, const std::string& body);

  void markAsRead();
  void setInboxType(const std::string& type);

  int getEmailId() const;
  bool getIsRead() const;
  std::string getInboxType() const;
  std::time_t getTime() const;
  std::string getSender() const;
  std::string getReceiver() const;
  std::string getSubject() const;
  std::string getMailText() const;

  std::string getFormattedTime() const;
  void saveToFile() const;

  void setIsRead(bool newIsRead);
  static Email loadFromString(const std::string& emailStr);

  void updateInFile() const;


 private:
  int emailId;
  bool isRead;
  std::string inboxType; //inbox or sentbox
  std::time_t time; //time when mail was sent.unix epoch .when using ,we convert to string time
  std::string sender;
  std::string receiver;
  std::string subject;
  std::string mailText;
};

#endif  // EMAIL_H
