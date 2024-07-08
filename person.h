//non ui class
#ifndef PERSON_H
#define PERSON_H

#include <string>

class Person {
 public:
  Person();
  Person(std::string fName, std::string lName);
  //~Person();

  std::string fullName();
  //void updateName(); // update in profile.polymorphed func.impl later

  std::string firstName;
  std::string lastName;

};

#endif // PERSON_H

