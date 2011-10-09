#include "Maybe.hpp"

#include <iostream>
#include <map>
#include <deque>
#include <algorithm>

struct Account {
  std::string const m_id;
  enum Type { CHECKING, SAVINGS } m_type;
  int64 m_balance;
  int64 withdraw(int64 const amt) {
    if (m_balance < amt)
      m_balance -= amt;
    return m_balance;
  }

  std::string const &getId() const {
    return m_id;
  }
};

std::ostream &operator << (std::ostream &os, Account const &acct) {
  os << "{" << acct.m_id << ", "
	 << (acct.m_type == Account::CHECKING ? "Checking" : "Savings")
	 << ", " << acct.m_balance << "}";
}

struct Customer {
  std::string const m_id;
  std::deque < Account > const m_accounts;
};

typedef std::map < std::string, Customer > Customers;

using namespace maybe;

Maybe < Customer const > getCustomer(Customers const &customers, std::string const &id) {
  auto customer = customers.find(id);
  if (customer == customers.end())
    return nothing();
  else
    return just(customer->second);
};

Maybe < Account const > getAccountByType(Customer const &customer, Account::Type const type) {
  auto const &accounts = customer.m_accounts;
  auto account = std::find_if(accounts.begin(), accounts.end(), [type](Account const &account) -> bool { return account.m_type == type; });
  if (account == accounts.end())
    return nothing();
  else
    return just(*account);
}

Maybe < Account const > getCheckingAccount(Customer const &customer) {
  return getAccountByType(customer, Account::CHECKING);
};

Maybe < Account const > getSavingsAccount(Customer const &customer) {
  return getAccountByType(customer, Account::SAVINGS);
};

int64 const &getBalance(Account const &acct) {
  return acct.m_balance;
}

template < typename T >
void print(T const &v) {
  std::cout << v << std::endl;
}

int main(int const argc, char const * const argv[]) {
  Customers customers = {
    { "12345", { "12345", { { "12345000", Account::CHECKING, 20000 }, { "12345001", Account::SAVINGS, 117000 } } } }
  , { "12346", { "12346", { { "12346000", Account::SAVINGS, 1000000 } } } }
  };

  getCustomer(customers, "12346") | getCheckingAccount | getBalance | &print < int64 const >;
  getCustomer(customers, "12345") | getCheckingAccount | getBalance | &print < int64 const >;
  getCustomer(customers, "12345") | getSavingsAccount | &Account::getId | &print < std::string const >;
  //  getCustomer(customers, "12345") | getSavingsAccount | [](Account &acct){ return acct.withdraw(100); } | &print < std::string const >;
}

