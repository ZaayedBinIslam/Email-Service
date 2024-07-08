#include "user.h"
#include <fstream>

User::User() {}


User::User(std::string fn, std::string ln, int userId,std::string mailAddress,
           std::string pass
           )
    : Person(fn, ln), userId(userId),mailAddress(mailAddress), password(pass)
{}
User::User(std::string mail, std::string pass ): mailAddress(mail),password(pass){}

std::string User::getMailAddress() const { return mailAddress; }
std::string User::getPassword() const { return password; }
int User::getUserId() const { return userId; }

void User::setPassword(const std::string &newPassword) {
  password = newPassword;
}

void User::saveToFile() const {
  std::ofstream file("users.txt", std::ios::app);
  if (file.is_open()) {
    file << firstName << "," << lastName << "," << userId << "," << mailAddress << "," << password << "\n";
    file.close();
  }
}

