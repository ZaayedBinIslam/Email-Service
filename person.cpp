#include "person.h"

Person::Person() {}

Person::Person(std::string fName, std::string lName)
    : firstName(fName), lastName(lName) {}

std::string Person::fullName() {
  return firstName+" "+lastName;
}
