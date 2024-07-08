//non ui class
#ifndef USER_H
#define USER_H

#include "person.h"
#include <string>

class User: public Person {
 public:
  User();
  User(std::string fn,std::string ln,int userId, std::string mailAddress, std::string pass);
  User(std::string username, std::string pass);
  //~User();


          //void updatePass(); //update in profile.impl later
          //void updateName(); //update in profile.polymorphed func.impl later

  std::string getMailAddress() const;
  std::string getPassword() const;
  int getUserId() const;


  void setPassword(const std::string& newPassword);
  void saveToFile() const;

 private:
  int userId;
  std::string mailAddress;
  std::string password;

};

#endif  // USER_H
