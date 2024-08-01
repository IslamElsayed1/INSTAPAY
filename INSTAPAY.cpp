#include <iostream>
#include <vector>
#include <string>
using namespace std;

class User
{
public:
   string id;
   string password;
   double balance;

   User(string userID = "", string userPassword = "", double initialBalance = 0)
   {
      id = userID;
      password = userPassword;
      balance = initialBalance;
   }
};

class PaymentMethod
{
public:
   virtual double getServiceCharge() const = 0;
   virtual string getMethodName() const = 0;
};

class Visa : public PaymentMethod
{
public:
   double getServiceCharge() const override
   {
      return 10.0;
   }

   string getMethodName() const override
   {
      return "Visa";
   }
};

class Fawry : public PaymentMethod
{
public:
   double getServiceCharge() const override
   {
      return 5.0;
   }

   string getMethodName() const override
   {
      return "Fawry";
   }
};

class Account
{
   vector<User> users;

public:
   void addUser(const string &id, const string &password, double initialBalance)
   {
      users.push_back(User(id, password, initialBalance));
      cout << "Account created successfully!\n";
   }

   User login(const string &id, const string &password)
   {
      for (const auto &user : users)
      {
         if (user.id == id && user.password == password)
         {
            cout << "Login successful!\n";
            return user;
         }
      }
      cout << "Invalid ID or Password.\n";
      return User();
   }

   bool transferMoney(User &sender, const string &receiverID, double amount, const PaymentMethod *paymentMethod)
   {
      for (auto &user : users)
      {
         if (user.id == receiverID)
         {
            double totalAmount = amount + paymentMethod->getServiceCharge();
            if (sender.balance >= totalAmount)
            {
               sender.balance -= totalAmount;
               user.balance += amount;
               return true;
               // cout << "Transfer successful! Service charge: $" << paymentMethod->getServiceCharge() << "\n";
            }
            else
            {
               // cout << "Insufficient balance.\n";
               return false;
            }
         }
      }
      cout << "Receiver ID not found.\n";
      return false;
   }

   int getNumberOfUsers() const
   {
      return users.size();
   }

   void displayBalance(const User &user) const
   {
      cout << "Your balance is: $" << user.balance << "\n";
   }
};

int main()
{
   Account account;
   int choice;

   while (true)
   {
      cout << "Welcome! Please choose an option:\n";
      cout << "1. Register\n2. Login\n3. Exit\n";
      cin >> choice;

      string id, password;
      double initialBalance;
      User loggedInUser;

      switch (choice)
      {
      case 1:
         cout << "Enter new ID: ";
         cin >> id;
         cout << "Enter new Password: ";
         cin >> password;
         cout << "Enter initial balance: ";
         cin >> initialBalance;
         account.addUser(id, password, initialBalance);
         break;
      case 2:
         cout << "Enter your ID: ";
         cin >> id;
         cout << "Enter your Password: ";
         cin >> password;
         loggedInUser = account.login(id, password);
         if (loggedInUser.id != "")
         {
            while (true)
            {
               cout << "Choose an option:\n";
               cout << "1. Send money\n2. Display balance\n3. Logout\n";
               cin >> choice;

               if (choice == 3)
               {
                  loggedInUser = User();
                  break;
               }
               else if (choice == 2)
               {
                  account.displayBalance(loggedInUser);
               }
               else if (choice == 1)
               {
                  if (account.getNumberOfUsers() < 2)
                  {
                     cout << "At least two registered users are required to send money.\n";
                     continue;
                  }

                  const PaymentMethod *paymentMethod = nullptr;
                  cout << "Choose payment method:\n";
                  cout << "1. Visa\n2. Fawry\n";
                  cin >> choice;

                  switch (choice)
                  {
                  case 1:
                     paymentMethod = new Visa();
                     break;
                  case 2:
                     paymentMethod = new Fawry();
                     break;
                  default:
                     cout << "Invalid payment method.\n";
                     continue;
                  }

                  cout << "Enter receiver ID: ";
                  cin >> id;
                  double amount;
                  cout << "Enter amount to send: ";
                  cin >> amount;
                  if (account.transferMoney(loggedInUser, id, amount, paymentMethod))
                  {
                     cout << "Transfer successful! Service charge: $" << paymentMethod->getServiceCharge() << "\n";
                  }
                  else
                  {
                     cout << "Failed to send money. Please check your balance and try again.\n";
                  }
                  delete paymentMethod;
               }
               else
               {
                  cout << "Invalid option.\n";
               }
            }
         }
         break;
      case 3:
         cout << "Exiting...\n";
         return 0;
      default:
         cout << "Invalid choice.\n";
      }
   }

   return 0;
}
