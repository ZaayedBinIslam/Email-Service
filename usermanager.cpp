#include "UserManager.h"
#include <algorithm>
#include <fstream>
#include <iostream>

UserManager::UserManager() : nextUserId(0), nextEmailId(0) {
  loadUsersFromFile();
  loadEmailsFromFile();
}

// int getNextEmailId()  { return nextEmailId ; }

void UserManager::loadUsersFromFile() {
  std::ifstream file("users.txt");
  if (file.is_open()) {
    std::string line;
    while (getline(file, line)) {
      size_t pos = 0;
      std::string token;
      std::vector<std::string> tokens;
      while ((pos = line.find(",")) != std::string::npos) {
        token = line.substr(0, pos);
        tokens.push_back(token);
        line.erase(0, pos + 1);
      }
      tokens.push_back(line);

      if (tokens.size() == 5) {
        int userId = std::stoi(tokens[2]);
        users.emplace_back(tokens[0], tokens[1], userId, tokens[3], tokens[4]);
        nextUserId = std::max(nextUserId, userId + 1);
      }
    }
    file.close();
  }
}

bool UserManager::isEmailUnique(const std::string &email) const {
  return std::none_of(users.begin(), users.end(), [&email](const User &user) {
    return user.getMailAddress() == email;
  });
}

bool UserManager::signup(const std::string &fn, const std::string &ln,
                         const std::string &email, const std::string &pass) {
  if (isEmailUnique(email)) {
    User newUser(fn, ln, nextUserId, email, pass);
    newUser.saveToFile();
    users.push_back(newUser);
    nextUserId++;
    return true;
  }
  return false;
}

bool UserManager::login(const std::string &email, const std::string &pass) {
  auto it = std::find_if(users.begin(), users.end(), [&](const User &user) {
    return user.getMailAddress() == email && user.getPassword() == pass;
  });
  return it != users.end();
}

void UserManager::loadEmailsFromFile() {
  std::ifstream file("emails.txt");
  if (file.is_open()) {
    std::string line;
    while (std::getline(file, line)) {
      try {
        Email email = Email::loadFromString(line);
        userEmails[email.getReceiver()].push_back(email);
        if (email.getSender() != email.getReceiver()) {
          Email sentEmail = email;
          sentEmail.setInboxType("sent");
          userEmails[email.getSender()].push_back(sentEmail);
        }
        nextEmailId = std::max(nextEmailId, email.getEmailId() + 1);
      } catch (const std::exception &e) {
        std::cerr << "Error loading email: " << e.what() << std::endl;
        // Continue to next line instead of terminating
      }
    }
    file.close();
  }
}

bool UserManager::sendEmail(const std::string &from, const std::string &to,
                            const std::string &subject,
                            const std::string &body) {
  if (isEmailUnique(from) || isEmailUnique(to)) {
    return false; // Either sender or receiver doesn't exist
  }

  Email newEmail(nextEmailId++, from, to, subject, body);
  newEmail.saveToFile();

  userEmails[to].push_back(newEmail);

  Email sentEmail = newEmail;
  sentEmail.setInboxType("sent");
  userEmails[from].push_back(sentEmail);

  return true;
}

std::vector<Email> UserManager::getInbox(const std::string &userEmail) const {
  std::vector<Email> inbox;
  if (userEmails.find(userEmail) != userEmails.end()) {
    for (const auto &email : userEmails.at(userEmail)) {
      if (email.getInboxType() == "inbox") {
        inbox.push_back(email);
      }
    }
  }
  return inbox;
}

std::vector<Email> UserManager::getSentBox(const std::string &userEmail) const {
  std::vector<Email> sentBox;
  if (userEmails.find(userEmail) != userEmails.end()) {
    for (const auto &email : userEmails.at(userEmail)) {
      if (email.getInboxType() == "sent") {
        sentBox.push_back(email);
      }
    }
  }
  return sentBox;
}

std::vector<Email> UserManager::getAllEmails() const {
  std::vector<Email> allEmails;
  for (const auto &userEmailPair : userEmails) {
    allEmails.insert(allEmails.end(), userEmailPair.second.begin(),
                     userEmailPair.second.end());
  }
  return allEmails;
}

void UserManager::updateEmail(const Email &updatedEmail) {
  auto &emails = userEmails[updatedEmail.getReceiver()];
  auto it = std::find_if(emails.begin(), emails.end(), [&](const Email &e) {
    return e.getEmailId() == updatedEmail.getEmailId();
  });
  if (it != emails.end()) {
    *it = updatedEmail;
    updatedEmail.updateInFile(); // Update in storage
  }

  // If this is a sent email, update it in the sender's sent box as well
  if (updatedEmail.getInboxType() == "sent") {
    auto &sentEmails = userEmails[updatedEmail.getSender()];
    auto sentIt =
        std::find_if(sentEmails.begin(), sentEmails.end(), [&](const Email &e) {
          return e.getEmailId() == updatedEmail.getEmailId();
        });
    if (sentIt != sentEmails.end()) {
      *sentIt = updatedEmail;
      updatedEmail.updateInFile(); // Update in storage
    }
  }
}
