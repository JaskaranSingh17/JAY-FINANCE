#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <ctime>
#include <iomanip>
#include <algorithm>
using namespace std;

// Helper function for current date
string currentDate() {
time_t now = time(0);
tm *ltm = localtime(&now);
return to_string(1900 + ltm->tm_year) + "-" +
to_string(1 + ltm->tm_mon) + "-" +
to_string(ltm->tm_mday);
}

class Date {
public:
string date;
Date() { date = currentDate(); }
};

class Notification {
private:
int notificationID;
string message;
Date date;
public:
Notification(int id, string msg) : notificationID(id), message(msg) {}

void sendNotification() {
cout << "Notification [" << date.date << "]: " << message << endl;
}
};

class Transaction {
private:
int transactionID;
double amount;
Date date;
string category;
string type;
public:
Transaction(int id, double amt, string cat, string typ)
: transactionID(id), amount(amt), category(cat), type(typ) {}

void display() {
string emoji = (type == "Income") ? "" : "";
cout << emoji << " Transaction #" << transactionID
<< " [" << date.date << "]\n"
<< "Amount: ₹" << amount
<< "\nCategory: " << category
<< "\nType: " << type << "\n\n";
}

string getCategory() { return category; }
double getAmount() { return amount; }
string getType() { return type; }
};

class Budget {
private:
int budgetID;
double limit;
string category; // Private member
double currentSpending;
public:
Budget(int id, double lim, string cat)
: budgetID(id), limit(lim), category(cat), currentSpending(0) {}

// Add this getter method
string getCategory() { return category; }

void trackExpenses(double amount) {
currentSpending += amount;
if(currentSpending > limit) {
Notification alert(100 + budgetID,
"Budget exceeded for " + category + "!");
alert.sendNotification();
}
else if(currentSpending >= 0.75 * limit) {
Notification alert(200 + budgetID,
"Budget " + category + " reaching limit (" +
to_string((int)((currentSpending/limit)*100)) + "%)");
alert.sendNotification();
}
}

void adjustBudget(double newLimit) {
limit = newLimit;
Notification(300 + budgetID,
"Budget adjusted for " + category + " to ₹" + to_string(newLimit))
.sendNotification();
}

void display() {
cout << "Budget for " << category
<< "\nLimit: ₹" << limit
<< "\nSpent: ₹" << currentSpending
<< "\nRemaining: ₹" << (limit - currentSpending) << "\n\n";
}
};

class Investment {
private:
int investmentID;
string type;
double amount;
double expectedReturns;
string riskLevel;
public:
Investment(int id, string typ, double amt, double ret, string risk)
: investmentID(id), type(typ), amount(amt),
expectedReturns(ret), riskLevel(risk) {}

void display() const {
cout << "Investment #" << investmentID
<< "\nType: " << type
<< "\nAmount: ₹" << amount
<< "\nExpected Returns: " << expectedReturns << "%"
<< "\nRisk: " << riskLevel << "\n\n";
}

double getExpectedReturns() const { return expectedReturns; }
string getType() const { return type; }
};

class TaxOptimization {
private:
int taxID;
double taxableIncome;
double deductions;
public:
TaxOptimization(double income) : taxableIncome(income), deductions(0) {}

void suggestTaxSavingInvestments(vector<Investment>& investments) {
double total = 0;
for(auto& inv : investments) {
if(inv.getType() == "PPF" || inv.getType() == "ELSS") {
total += inv.getExpectedReturns();
}
}
deductions = total;
Notification(400, "Tax Saving Tip: You can claim ₹" +
to_string(deductions) + " in deductions").sendNotification();
}

double calculateTax() {
return max(0.0, (taxableIncome - deductions) * 0.3);
}
};

class Account {
private:
int accountID;
double balance;
double income;
double expenses;
vector<Transaction> transactions;
vector<Budget> budgets;
public:
Account(int id) : accountID(id), balance(0), income(0), expenses(0) {}

void addIncome(double amount, string category) {
income += amount;
balance += amount;
transactions.push_back(Transaction(transactions.size()+1,
amount, category, "Income"));
Notification(500, "Income added: ₹" + to_string(amount)).sendNotification();
}

void addExpense(double amount, string category) {
expenses += amount;
balance -= amount;
transactions.push_back(Transaction(transactions.size()+1,
amount, category, "Expense"));

for(auto& budget : budgets) {
if(budget.getCategory() == category) {
budget.trackExpenses(amount);
}
}
}

void addBudget(double limit, string category) {
budgets.push_back(Budget(budgets.size()+1, limit, category));
}

void displayDashboard() {
cout << "\n\nJAY FINANCE DASHBOARD\n";
cout << "------------------------------\n";
cout << "Total Income: ₹" << income << endl;
cout << "Total Expenses: ₹" << expenses << endl;
cout << "Current Balance: ₹" << balance << "\n\n";

cout << "Recent Transactions:\n";
for(int i = max(0, (int)transactions.size()-5); i < transactions.size(); i++) {
transactions[i].display();
}

cout << "Active Budgets:\n";
for(auto& budget : budgets) {
budget.display();
}
}

vector<Budget>& getBudgets() { return budgets; }
double getBalance() { return balance; }
};

class User {
private:
int userID;
string name;
string email;
string password;
Account account;
vector<Investment> investments;
public:
User(int id, string n, string e, string p)
: userID(id), name(n), email(e), password(p), account(id) {}

bool login(string em, string pw) {
return (em == email && pw == password);
}

void updateProfile(string newName, string newEmail) {
name = newName;
email = newEmail;
Notification(600, "Profile updated successfully!").sendNotification();
}

void financialCheckup() {
TaxOptimization tax(account.getBalance());
tax.suggestTaxSavingInvestments(investments);
cout << "Estimated Tax Liability: ₹" << tax.calculateTax() << "\n\n";
}

void addInvestment(string type, double amount, double returns, string risk) {
investments.push_back(Investment(investments.size()+1, type, amount, returns, risk));
Notification(700, "New investment added: " + type).sendNotification();
}

void displayInvestments() {
cout << "\nInvestment Portfolio:\n";
for(auto& inv : investments) {
inv.display();
}
}

Account& getAccount() { return account; }
};

template <typename T>
class UserProfile {
private:
T userID;
string name;
string email;
string password;
Account* account;
vector<Investment> investments;

public:
UserProfile(T id, string n, string e, string p, Account* acc)
: userID(id), name(n), email(e), password(p), account(acc) {}

// Getters
T getID() const { return userID; }
string getName() const { return name; }
string getEmail() const { return email; }
Account* getAccount() const { return account; }

bool authenticate(string enteredEmail, string enteredPass) const {
return (email == enteredEmail) && (password == enteredPass);
}

void displayProfile() const {
cout << "\nUser Profile\n";
cout << "ID: " << userID << endl;
cout << "Name: " << name << endl;
cout << "Email: " << email << endl;
cout << "Account Balance: ₹" << account->getBalance() << endl;
}

// Add methods from the User class
void addInvestment(string type, double amount, double returns, string risk) {
investments.push_back(Investment(investments.size() + 1, type, amount, returns, risk));
Notification(700, "New investment added: " + type).sendNotification();
}

void displayInvestments() const {
cout << "\nInvestment Portfolio:\n";
for (const auto& inv : investments) {
inv.display();
}
}

void financialCheckup() {
TaxOptimization tax(account->getBalance());
tax.suggestTaxSavingInvestments(investments);
cout << "Estimated Tax Liability: ₹" << tax.calculateTax() << "\n\n";
}

void updateProfile(string newName, string newEmail) {
name = newName;
email = newEmail;
Notification(600, "Profile updated successfully!").sendNotification();
}
};

template <typename T>
class UserManager {
private:
map<T, UserProfile<T>> users;

public:
void addUser(const UserProfile<T>& user) {
users.insert({user.getID(), user}); // Use insert to avoid default constructor
}

UserProfile<T>* authenticateUser(T id, string email, string password) {
auto it = users.find(id);
if (it != users.end() && it->second.authenticate(email, password)) {
return &it->second;
}
return nullptr;
}

void listUsers() const {
cout << "\nRegistered Users:\n";
for (const auto& pair : users) {
cout << "ID: " << pair.first
<< " | Name: " << pair.second.getName()
<< " | Email: " << pair.second.getEmail() << endl;
}
}
};

// Main Application
int main() {
cout << "Welcome to JAY FINANCE\n";

// Create predefined user accounts
UserManager<int> userManager;

// Create and add accounts
Account* jaskaranAccount = new Account(1);
userManager.addUser(UserProfile<int>(1, "Jaskaran Singh Panjetta", "jaskaran@jayfinance.com", "Pass@123", jaskaranAccount));

Account* yashAccount = new Account(2);
userManager.addUser(UserProfile<int>(2, "Yash Patil", "yash@jayfinance.com", "Pass@23", yashAccount));

Account* aaditAccount = new Account(3);
userManager.addUser(UserProfile<int>(3, "Aadit Rangnaker", "aadit@jayfinance.com", "Pass@123", aaditAccount));

// Login flow
int userID;
string email, password;

userManager.listUsers();

cout << "\nLogin to Continue\n";
cout << "Enter User ID: ";
cin >> userID;
cout << "Email: ";
cin >> email;
cout << "Password: ";
cin >> password;

UserProfile<int>* currentUser = userManager.authenticateUser(userID, email, password);

if(!currentUser) {
cout << "\nAuthentication Failed!\n";
return 0;
}

cout << "\nWelcome, " << currentUser->getName() << "!\n";
currentUser->displayProfile();

// Get the user's account pointer
Account* userAccount = currentUser->getAccount();

// Main application loop
while(true) {
// Display financial overview
userAccount->displayDashboard();
currentUser->displayInvestments();
currentUser->financialCheckup();

// Interactive menu
cout << "\nMAIN MENU\n";
cout << "1. Add Income\n";
cout << "2. Add Expense\n";
cout << "3. Create Budget\n";
cout << "4. Add Investment\n";
cout << "5. Update Profile\n";
cout << "6. Exit\n";
cout << "Choice: ";

int choice;
cin >> choice;

if(choice == 6) break;

double amount;
string category, type, risk;
double returns;

switch(choice) {
case 1:
cout << "\nAdd Income\n";
cout << "Amount: ₹";
cin >> amount;
cout << "Category: ";
cin >> category;
userAccount->addIncome(amount, category);
break;

case 2:
cout << "\nAdd Expense\n";
cout << "Amount: ₹";
cin >> amount;
cout << "Category: ";
cin >> category;
userAccount->addExpense(amount, category);
break;

case 3: {
double limit;
cout << "\nCreate Budget\n";
cout << "Category: ";
cin >> category;
cout << "Monthly Limit: ₹";
cin >> limit;
userAccount->addBudget(limit, category);
break;
}

case 4:
cout << "\nAdd Investment\n";
cout << "Type (FD/Stocks/Mutual Funds): ";
cin >> type;
cout << "Amount: ₹";
cin >> amount;
cout << "Expected Returns (%): ";
cin >> returns;
cout << "Risk Level (Low/Medium/High): ";
cin >> risk;
currentUser->addInvestment(type, amount, returns, risk);
break;

case 5: {
string newEmail, newName;
cout << "\nUpdate Profile\n";
cout << "New Name: ";
cin.ignore();
getline(cin, newName);
cout << "New Email: ";
getline(cin, newEmail);
currentUser->updateProfile(newName, newEmail);
break;
}
}
}

// Cleanup
delete jaskaranAccount;
delete yashAccount;
delete aaditAccount;

cout << "\nThank you for using JAY FINANCE! Your financial data has been secured.\n";
return 0;
}