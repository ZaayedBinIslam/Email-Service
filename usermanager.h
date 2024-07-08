#ifndef USER_MANAGER_H
#define USER_MANAGER_H

#include "user.h"
#include "email.h"
#include <vector>
#include <map>

class UserManager {
 private:
  std::vector<User> users;
  int nextUserId;
  int nextEmailId;
  std::map<std::string, std::vector<Email>> userEmails;

  void loadUsersFromFile();
  void loadEmailsFromFile();
  bool isEmailUnique(const std::string& email) const;

 public:
  UserManager();

  // int getNextEmailId();

  bool signup(const std::string& fn, const std::string& ln, const std::string& email, const std::string& pass);
  bool login(const std::string& email, const std::string& pass);

  bool sendEmail(const std::string& from, const std::string& to, const std::string& subject, const std::string& body);
  std::vector<Email> getInbox(const std::string& userEmail) const;
  std::vector<Email> getSentBox(const std::string& userEmail) const;
  std::vector<Email> getAllEmails() const;

  void updateEmail(const Email& updatedEmail) ;
};

#endif // USER_MANAGER_H
