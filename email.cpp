#include "Email.h"
#include <fstream>
#include <iostream>
#include <sstream>
// #include <iomanip>
#include <vector>

Email::Email(int id, const std::string& from, const std::string& to, const std::string& subj, const std::string& body)
    : emailId(id), isRead(false), inboxType("inbox"), time(std::time(nullptr)),
      sender(from), receiver(to), subject(subj), mailText(body) {}

void Email::markAsRead() { isRead = true; }
void Email::setInboxType(const std::string& type) { inboxType = type; }

int Email::getEmailId() const { return emailId; }
bool Email::getIsRead() const { return isRead; }
std::string Email::getInboxType() const { return inboxType; }
std::time_t Email::getTime() const { return time; }
std::string Email::getSender() const { return sender; }
std::string Email::getReceiver() const { return receiver; }
std::string Email::getSubject() const { return subject; }
std::string Email::getMailText() const { return mailText; }

std::string Email::getFormattedTime() const {
  char buffer[80];
  struct tm* timeinfo = localtime(&time);
  strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);
  return std::string(buffer);
}

void Email::saveToFile() const {
  std::ofstream file("emails.txt", std::ios::app);
  if (file.is_open()) {
    file << emailId << "," << (isRead ? "1" : "0") << "," << inboxType << "," << time << ","
         << sender << "," << receiver << "," << subject << "," << mailText << "\n";
    file.close();
  } else {
    std::cerr << "Unable to open file for saving email." << std::endl;
  }
}

Email Email::loadFromString(const std::string& emailStr) {
  std::istringstream iss(emailStr);
  std::string token;
  std::vector<std::string> tokens;
  while (std::getline(iss, token, ',')) {
    tokens.push_back(token);
  }

  if (tokens.size() >= 8) {  // Changed from == 8 to >= 8
    Email email(std::stoi(tokens[0]), tokens[4], tokens[5], tokens[6],
                tokens.size() > 7 ? tokens[7] : "");  // Handle potential missing body
    email.isRead = (tokens[1] == "1");
    email.inboxType = tokens[2];
    email.time = std::stoll(tokens[3]);
    return email;
  }

          // If we can't parse the email, return a default email instead of throwing
  return Email(0, "unknown", "unknown", "Invalid Email", "This email could not be loaded correctly.");
}


void Email::updateInFile() const {
  std::ifstream inFile("emails.txt");
  std::ofstream outFile("emails_temp.txt");

  if (!inFile || !outFile) {
    std::cerr << "Error opening files for updating email." << std::endl;
    return;
  }

  std::string line;
  while (std::getline(inFile, line)) {
    std::istringstream iss(line);
    std::string idStr;
    std::getline(iss, idStr, ',');

    if (std::stoi(idStr) == emailId) {
      // This is the email we want to update, write the new data
      outFile << emailId << "," << (isRead ? "1" : "0") << "," << inboxType << ","
              << time << "," << sender << "," << receiver << "," << subject << ","
              << mailText << "\n";
    } else {
      // This is not the email we're updating, write the original line
      outFile << line << "\n";
    }
  }

  inFile.close();
  outFile.close();

          // Replace the original file with the updated one
  std::remove("emails.txt");
  std::rename("emails_temp.txt", "emails.txt");
}



