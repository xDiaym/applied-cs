#include <cctype>
#include <cstdio>
#include <exception>
#include <stdexcept>
#include <iostream>
#include <string>
#include <format>

class PasswordValidationError : std::exception {
public:
  PasswordValidationError(std::string msg)
    : message_(msg) {}

  const char* what() const noexcept {
    return "Password validation error";
  }
  std::string message() const noexcept { return message_; }

private:
  std::string message_;
};

void validate(std::string s) {
  if (s.size() < 8)
    throw std::length_error("Too short!");
  bool numeric = false, lower = false, upper = false;
  for (char c : s) {
    if (isdigit(c)) numeric = true;
    if (islower(c)) lower = true;
    if (isupper(c)) upper = true;
  }
  if (!numeric) throw PasswordValidationError("No digits!");
  if (!lower) throw PasswordValidationError("No lowercase!");
  if (!upper) throw PasswordValidationError("No uppercase!");
}

int main() {
  std::string s; 
  while (true) {
    std::cin >> s;
    try {
      validate(s);
      std::cout << "password is ok\n";
      break;
    } catch (std::length_error& le) {
      std::cout << "Password invalid: too short\n";
    } catch (PasswordValidationError &pve) {
      std::cout << "Password invalid: " << pve.message() << '\n';
    } catch (...) {
      std::cout << "Password invalid: unknown exception" << '\n';
      throw;
    }
  }
}