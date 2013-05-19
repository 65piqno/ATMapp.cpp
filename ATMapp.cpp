/* Student Names: _______, _______
 * Team Number: #__
 * Lab Section: B01
 * Developing an ATM system that supports 6 basic use cases
 */

#include <cstdlib>
#include <ctime>
#include <ctype.h>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <time.h>
#include <unistd.h>

//#define WINDOWS
#define UNIX

//#define SHOW_LOAD_UNLOAD_MESSAGES

using namespace std;

typedef struct Client {
  int id; // unique
	string first_name;
	string last_name;
	char gender; // M or F
	string card_number;
	string pin;
	// other fields, address, phone, email
} Client_s;

typedef enum Account_Type {
	checking, saving
} Account_Type_s;

typedef struct Account_s {
	int client_id;
	unsigned int institution_number; // 3 digits
	unsigned int branch_number; // 5 digits
	unsigned int account_number; // 7 digits
	double balance;
	Account_Type account_type;
} Account;

typedef struct Transaction_s {
	string date_time_string;
	unsigned int account_number; // 7 digits
	double amount;
} Transaction;

/********************************************************************/
// global vars
//HANDLE global_console_handle;
int global_logged_in_client_id;

Client global_clients[500];
int global_num_of_clients;

Account global_accounts[500];
int global_num_of_accounts;

void change_account_balance (unsigned int account_number, double change) {
	for (int i=0;i<global_num_of_accounts;i++)
		if (global_accounts[i].account_number == account_number) {
			global_accounts[i].balance += change;
			return;
		}
}

Transaction global_transactions[5000];
int global_num_of_transactions;

string global_data_file_path = "./";

// global CONST's
const int DELAY_FOR_PROGRESS_BARS=10000; // in microseconds . the lower this value, the faster the progress bars

#define NORMAL_COLOR  "\x1B[0m"
#define RED     "\x1B[31m"
#define GREEN   "\x1B[32m"
#define YELLOW  "\x1B[33m"
#define BLUE  "\x1B[34m"
#define MAGENTA  "\x1B[35m"
#define CYAN  "\x1B[36m"
#define WHITE  "\x1B[37m"
/********************************************************************/
int only_digits(string card_number_last_four) {
	for (unsigned int i = 0; i < card_number_last_four.length(); i++) {
		if (!isdigit(card_number_last_four[i]))
			return 0;
	}
	return 1;
}
/********************************************************************/
Client find_client_by_client_id(int id) {
	for (int i=0;i<global_num_of_clients;i++)
		if (global_clients[i].id == id)
			return global_clients[i];

	Client invalidClient;
	invalidClient.id=-1;
	return invalidClient;
}
/********************************************************************/
Account find_account_by_account_number(unsigned int account_number) {
	for (int i=0;i<global_num_of_accounts;i++)
		if (global_accounts[i].account_number == account_number)
			return global_accounts[i];

	Account invalidAccount;
	invalidAccount.account_number=-1;
	return invalidAccount;
}
/********************************************************************/
void print_client(Client c) {
	cout<< "-----------------------\nPrinting client information...\n";
	//SetConsoleTextAttribute(global_console_handle, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	cout << "ID: " << c.id << endl;
	cout << "Name: " << c.first_name << " " << c.last_name << endl;
	cout << "Card #: " << c.card_number << endl;
	cout << "PIN: " << c.pin << endl << endl;

	// back to default
	//SetConsoleTextAttribute(global_console_handle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}
/********************************************************************/
void print_all_clients() {
	cout<< "-----------------------\n";
	cout << "List of all " << global_num_of_clients
			<< " clients in the system...\n";
	for (int i = 0; i < global_num_of_clients; i++) {
		print_client(global_clients[i]);
	}
}
/********************************************************************/
void print_account(Account account) {

	cout<< "-----------------------\nPrinting account details...\n";

	//SetConsoleTextAttribute(global_console_handle, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	cout << "Client: " << find_client_by_client_id(account.client_id).first_name << " "
					   << find_client_by_client_id(account.client_id).last_name << endl;
	cout << "Balance: $" << account.balance << endl;
	cout << "Type: " << ((account.account_type == checking) ? "checking" : "saving") << endl;

	// back to default
	//SetConsoleTextAttribute(global_console_handle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}
/********************************************************************/
void print_all_accounts() {
	cout<< "-----------------------\n";
	cout << "List of all " << global_num_of_accounts
			<< " accounts in the system...\n";
	for (int i = 0; i < global_num_of_accounts; i++)
		print_account(global_accounts[i]);
}
/********************************************************************/
void print_transaction(Transaction transaction) {

	cout<< "-----------------------\nPrinting transaction details...\n";

	//SetConsoleTextAttribute(global_console_handle, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	cout << "Date/time: " << transaction.date_time_string << endl;
	cout << "Account #: " << transaction.account_number << endl;
	cout << "Amount   : $" << transaction.amount << endl;

	// back to default
	//SetConsoleTextAttribute(global_console_handle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}
/********************************************************************/
void print_all_transactions() {
	cout<< "-----------------------\n";
	cout << "List of all " << global_num_of_transactions
			<< " transactions in the system...\n";
	for (int i = 0; i < global_num_of_transactions; i++) {
		print_transaction(global_transactions[i]);
	}
}
/********************************************************************/
int check_if_client_id_exists(int client_id) {
	return 1;
}
/********************************************************************/
string int_to_string(int x)
{
/**************************************/
/* This function is similar to itoa() */
/* "integer to alpha", a non-standard */
/* C language function. It takes an   */
/* integer as input and as output,    */
/* returns a C++ string.              */
/* itoa()  returned a C-string (null- */
/* terminated)                        */
/* This function is not needed because*/
/* the following template function    */
/* does it all                        */
/**************************************/
       string r;
       stringstream s;

       s << x;
       r = s.str();

       return r;
}
/********************************************************************/
string double_to_string(double d)
{
       string r;
       stringstream s;

       s << d;
       r = s.str();

       return r;
}
/********************************************************************/
void display_error_message(string message) {
	//SetConsoleTextAttribute(global_console_handle, FOREGROUND_RED | FOREGROUND_INTENSITY);
	cout << RED << message << "\n" << NORMAL_COLOR;
	//SetConsoleTextAttribute(global_console_handle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}
/********************************************************************/
void print_string_in_ascii(string str) {
	unsigned int i;
	for (i=0;i<str.length();i++)
		cout << int(str[i]) << " ";
	cout << endl;
}
/********************************************************************/
void read_data_from_files()
{
	// read clients
	string line;
	string clients_file_path = global_data_file_path;
	clients_file_path.append("clients.txt");
	ifstream clients_file(clients_file_path.c_str());
	if (clients_file.is_open())
	{
		while (clients_file.good())
		{
			/* Format:
			 int id; // unique
			 char first_name[20];
			 char last_name[20];
			 char card_number[10];
			 char pin[4];
			 (EMPTY LINE)
			 next record...
			 */
			Client aClient;
			getline(clients_file, line);
			aClient.id = atoi(line.c_str());

			getline(clients_file, aClient.first_name);
			getline(clients_file, aClient.last_name);
			getline(clients_file, line);
			aClient.gender = line[0];
			getline(clients_file, aClient.card_number);
			getline(clients_file, aClient.pin);

			//read empty line
			getline(clients_file, line);

			//print_client(aClient);//this is for debugging only! comment out eventually.
			global_clients[global_num_of_clients] = aClient;

			global_num_of_clients++;
		}
		clients_file.close();
		cout << "Finished reading data for " << global_num_of_clients << " clients from file ... " << endl;
	}
	else
	{
		display_error_message("Unable to open file: clients.txt. Exiting init...");
		exit (1);
	}

	// read accounts
	string accounts_file_path = global_data_file_path;
	accounts_file_path.append("accounts.txt");
	ifstream accounts_file(accounts_file_path.c_str());
	if (accounts_file.is_open())
	{
		while(accounts_file.good())
		{
			/* Format:
			int client_id;
			int institution_number;
			int branch_number;
			int account_number;
			double balance;
			Account_Type account_type;
			(EMPTY LINE)
			next record...
			*/

			Account_s anAccount;
			getline(accounts_file, line);
			anAccount.client_id = atoi(line.c_str());
			getline(accounts_file, line);
			anAccount.institution_number = atoi(line.c_str());
			getline(accounts_file, line);
			anAccount.branch_number = atoi(line.c_str());
			getline(accounts_file, line);
			anAccount.account_number = atoi(line.c_str());
			getline(accounts_file, line);
			anAccount.balance = atoi(line.c_str());
			getline(accounts_file, line);

			if (line.find("checking"))
				anAccount.account_type=checking;
			else if (line.find("saving"))
				anAccount.account_type=saving;
			else
				display_error_message("Undefined account type in accounts file: " + line);

			//read empty line
			getline(accounts_file, line);

			//print_account(anAccount);//this is for debugging only! comment out eventually.
			global_accounts[global_num_of_accounts] = anAccount;
			global_num_of_accounts++;

		}
		accounts_file.close();
		cout << "Finished reading data for " << global_num_of_accounts << " accounts from file ... " << endl;
	}
	else
		{
			display_error_message("Unable to open file: accounts.txt. Exiting init...");
			exit (1);
		}


	// read transactions
	string transactions_file_path = global_data_file_path;
	transactions_file_path.append("transactions.txt");
	ifstream transactions_file(transactions_file_path.c_str());
	if (transactions_file.is_open())
	{
		while(transactions_file.good())
		{
			/* Format:
			 string date_time_string;
			 int account_number; double amount;
			 next record...
			 */

			Transaction_s aTransaction;//this is for debugging only! comment out eventually.
			getline(transactions_file, aTransaction.date_time_string);
			getline(transactions_file, line, ' ');
			aTransaction.account_number = atoi(line.c_str());
			getline(transactions_file, line);
			aTransaction.amount = atoi(line.c_str());
			
			//print_transaction(aTransaction); //this is for debugging only! comment out eventually.
			global_transactions[global_num_of_transactions] = aTransaction;
			global_num_of_transactions++;

			
		}
		transactions_file.close();
		cout << "Finished reading data for " << global_num_of_transactions << " transactions from file ... " << endl;
	}
	else
	{
		display_error_message("Unable to open file: transactios.txt. Exiting init...");
		exit (1);
	}
}
/********************************************************************/
void write_accounts_to_files_on_exit() {
	string accounts_file_path = global_data_file_path;
	accounts_file_path.append("accounts.txt");
	ofstream accounts_file(accounts_file_path.c_str());

	if (accounts_file.is_open()) {
		for (int i = 0; i < global_num_of_accounts; i++) {
			/* Format:
				int client_id;
				unsigned int institution_number; // 3 digits
				unsigned int branch_number; // 5 digits
				unsigned int account_number; // 7 digits
				double balance;
				Account_Type account_type;
			 (EMPTY LINE)
			 next record...
			 */

			// TO DO
		}

		accounts_file.close();
		cout << "Finished writing the updated data of all " << global_num_of_accounts
				<< " accounts to file ... " << endl;
	} else {
		display_error_message("write_accounts_to_files_on_exit(): Unable to write to file: accounts.txt");
		return;
	}
}
/********************************************************************/
void init() {
	#ifdef SHOW_LOAD_UNLOAD_MESSAGES
	cout<< GREEN << "-----------------------\n";
	cout << "Starting system initialization... " << endl;
	#endif

	//global_console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
	global_logged_in_client_id = 0;

	global_num_of_clients = 0;
	global_num_of_accounts = 0;
	global_num_of_transactions=0;
	read_data_from_files();

	#ifdef SHOW_LOAD_UNLOAD_MESSAGES
	print_all_clients();
	print_all_accounts();
	print_all_transactions();
	cout << "System initialization done." << endl;
	cout<< "-----------------------\n" << NORMAL_COLOR;
	#endif

}
/********************************************************************/
void clear_screen(void) {
#ifdef UNIX
	system("clear");
#elif defined WINDOWS
	system("cls");
#endif
}
/********************************************************************/
int valid_menu_choice(int choice) {
	int num_of_menu_choices=6;
	int valid_menu_choices[] = {0, 1, 2, 3, 4, 5};

	for(int i=0;i<num_of_menu_choices;i++)
		if (valid_menu_choices[i] == choice) return 1;

	return 0;
}
/********************************************************************/
int get_client_main_menu_choice() {
	int choice = -1;

	clear_screen();

	//SetConsoleTextAttribute(global_console_handle, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	cout << GREEN << "Please select one the following options:\n\n";
	cout << "1-Withdraw" << endl;
	cout << "2-Deposit" << endl;
	cout << "3-View balance" << endl;
	cout << "4-Transfer funds" << endl;
	cout << "5-View my transactions" << endl;
	cout << "0-Quit" << endl;
	//SetConsoleTextAttribute(global_console_handle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);

	cout << "Enter your menu choice: " << NORMAL_COLOR;
	cin >> choice;

	while (!valid_menu_choice(choice)) {
		display_error_message("Error. Invalid menu choice. Please try again.");
		cout << "Enter your menu choice: ";
		cin >> choice;
	}

	// back to default
	//SetConsoleTextAttribute(global_console_handle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

	return choice;
}
/********************************************************************/
int get_client_id_by_card_number(char card_number[]) {
	return 1;
}
/********************************************************************/
Client login_input() {

	Client input_client;

	string card_number_last_four;
	string card_pin;

	cout << GREEN << "Please login:\n\n";
	cout << "Card number: 4500 5400 1234 xxxx ";
	cin >> card_number_last_four;

	while ((card_number_last_four.length() != 4) || (!only_digits(card_number_last_four))) {
		display_error_message("Error. Can only enter 4 digits... Please try again.");
		cout << "Card number: 4500 5400 1234 xxxx ";
		cin >> card_number_last_four;
	}
	string input_card_number="450054001234";
	input_card_number.append(card_number_last_four);
	input_client.card_number = input_card_number;
	
	//EDITS-------------------------------------------------------/
	cout << "Card pin: ";
	cin >> card_pin;

	
	while ((card_pin.length() !=4) || (!only_digits(card_pin)))
	{
		display_error_message("Error. Can only enter 4 digits... Please try again.");
		cout << "Card pin: ";
		cin >> card_pin;
	}
	input_client.pin = card_pin;
	//EDITS-------------------------------------------------------/
	cout << NORMAL_COLOR;
	return input_client;
}
/********************************************************************/
int login_check_username_password(Client input_client) {
	/*
	 * promises:
	 * 		if the card_number and PIN given in input_client match a client in the system
	 * 			it should return the ID of the client
	 * 		else
	 * 			it should return 0
	 */

	 for(int i = 0; i < global_num_of_clients; i++)
	 {
		if(global_clients[i].card_number == input_client.card_number && global_clients[i].pin == input_client.pin)
			return global_clients[i].id;
	 }

	return 0;
}
/********************************************************************/
void login() {
	do {
		Client input_client=login_input();
		global_logged_in_client_id = login_check_username_password(input_client);
		if (global_logged_in_client_id == 0)
			display_error_message("Invalid username and/or password... Please try again.");
	}
	while (global_logged_in_client_id == 0);
}
/********************************************************************/
void display_welcome_message_and_wait_for_user() {
	string title_and_full_name = "EMPTY NAME";
	Client client=find_client_by_client_id(global_logged_in_client_id);

	//EDITS-------------------------------------------------------/
	string title;
	string fullname;
	if(client.gender == 'M')
	{
		title = "Mr.";
	} else {
		title = "Mrs.";
	}
	fullname = client.first_name + " " + client.last_name;
	title_and_full_name = title + " " + fullname;
	
	
	
	//EDITS-------------------------------------------------------/

	// you need to properly fill up title_and_full_name from client variable

	cout << YELLOW<< "Welcome: " << title_and_full_name << "\n";
	cout << "--------------------------------------------\n";
	cout << "Login successful. Press ENTER to continue..." << endl;

	getchar();getchar();
}
/********************************************************************/
/********************************************************************/
void process_quit() {
	write_accounts_to_files_on_exit();

	cout << "Program is now exiting.\n";
}
/********************************************************************/
void log_transaction(unsigned int account_number, double amount) {
	/* format:
	 * 		date time
	 * 		account_number change_amount
	 * example:
	 * 		Jun 09 2012 22:21:47
	 * 		1234567 -20
	 */

	string line;
	string transactions_file_path = global_data_file_path;
	transactions_file_path.append("transactions.txt");
	ofstream transactions_file(transactions_file_path.c_str(), ios::app);
	if (transactions_file.is_open()) {
		time_t t = time(0);   // get time now
		struct tm * time_now = localtime( & t );
		char time_now_string[80];
		strftime (time_now_string, 80, "%b %d %Y %X",time_now);
		transactions_file << time_now_string << "\n";

		transactions_file << account_number << " " << amount << endl;

		transactions_file.close();
		cout << "A transaction for $" << amount << " was successfully logged into file ... " << endl;
	} else {
		display_error_message("log_transaction(): Unable to write to file: transactions.txt");
		return;
	}
}
/*******************************************************************
 * returns the account for client by ID, found in sequence_number
 * in the accounts file for this client
 */
int find_client_account_number_by_sequence_number(int client_id, int sequence_number) {
	int tmp_sequence_number=0;
	for (int i=0;i<global_num_of_accounts;i++) {
		if (global_accounts[i].client_id == global_logged_in_client_id)
			tmp_sequence_number++;
		if (tmp_sequence_number == sequence_number)
			return global_accounts[i].account_number;
	}
	return -1;
}
/********************************************************************/
void display_accounts_list_and_balances() {
	cout << "List of your accounts with their balances: \n";
	cout << "--------------------------------------------------\n";
	cout << " #   Inst #   Branch #   Account #         Balance  \n";
	cout << "--------------------------------------------------\n";
	int num_of_accounts=0;
	for (int i=0;i<global_num_of_accounts;i++) {
		if (global_accounts[i].client_id == global_logged_in_client_id) {
			num_of_accounts++;
			cout << setw (2) << num_of_accounts << "   "
				 << setw (6) << global_accounts[i].institution_number << "   "
				 << setw (8) << global_accounts[i].branch_number << "   "
				 << setw (9) << global_accounts[i].account_number << "      "
				 <<  "$"
				 << setw (9) << setprecision (2) << fixed
				 << global_accounts[i].balance
				 << "\n";
		}
	}
	cout << "----------------------------------------------\n";
	cout << "You have " << num_of_accounts << " accounts in this system.\n";
	cout << "----------------------------------------------\n";
}
/********************************************************************/
int is_account_sequence_number_valid(int account_sequence_number) {
	// account_sequence_number will be valid as long it is >=1 and <= num_of_accounts the client has
	int num_of_accounts=0;

	for (int i=0;i<global_num_of_accounts;i++)
		if (global_accounts[i].client_id == global_logged_in_client_id)
			num_of_accounts++;

	if (account_sequence_number>=1 && account_sequence_number <=num_of_accounts)
		return 1;
	else
		return 0;
}
/********************************************************************/
void process_withdraw() {
	cout << "----------------------------------------------\n";
	cout << "------------------ Withdraw ------------------\n";
	cout << "----------------------------------------------\n";
	display_accounts_list_and_balances();

	// TO DO - start

	// implement error checking
	// ...

	// implement process_withdraw
	// ...

	// TO DO - end
	
	cout << "-----------------------\n";
	cout << "Press ENTER to continue\n";
	getchar();getchar();
}
/********************************************************************/
void process_deposit() {
	cout << "----------------------------------------------\n";
	cout << "------------------ Deposit -------------------\n";
	cout << "----------------------------------------------\n";
	display_accounts_list_and_balances();

	int account_sequence_number=1;
	cout << "Enter account # (1, ... above): ";
	cin >> account_sequence_number;

	while (!is_account_sequence_number_valid(account_sequence_number)) {
		display_error_message("Error. Invalid account. Please try again.");
		cout << "Enter account # (1, ... above):";
		cin >> account_sequence_number;
	}

	cout << "Enter amount to deposit: $";
	double amount_to_deposit;
	cin >> amount_to_deposit;

	cout << "Put the funds in an envelope and slide it into the ATM.\nPress ENTER to feed it in.\n";
	getchar();getchar();

	// TO DO

	cout << "-----------------------\n";
	cout << "Press ENTER to continue\n";
	getchar();getchar();

}
/********************************************************************/
void process_view_transactions() {
	cout << "-------------------------------------------------\n";
	cout << "--------------- View Transactions ---------------\n";
	cout << "----------------------------------------------------------------\n";
	cout << "Date/time             Account #       Debit     Credit\n";
	cout << "----------------------------------------------------------------\n";
	for (int i = 0; i < global_num_of_transactions; i++) {
		if (find_account_by_account_number(global_transactions[i].account_number).client_id
				==global_logged_in_client_id) {
			// TO DO

		}
	}

	cout << "-----------------------\n";
	cout << "Press ENTER to continue\n";
	getchar();getchar();
}
/********************************************************************/
void process_view_balance() {
	cout << "----------------------------------------------\n";
	cout << "---------- View Account Balances -------------\n";
	cout << "----------------------------------------------\n";
	display_accounts_list_and_balances();

	cout << "Press ENTER to continue\n";
	getchar();getchar();

}
/********************************************************************/
void process_transfer() {
	cout << "----------------------------------------------\n";
	cout << "-------------- Transfer Funds ----------------\n";
	cout << "----------------------------------------------\n";
	display_accounts_list_and_balances();

	int source_account_sequence_number;
	int destination_account_sequence_number;
	do {
		cout << "Enter source account # (1, ... above): ";
		cin >> source_account_sequence_number;
		while (!is_account_sequence_number_valid(source_account_sequence_number)) {
			display_error_message("Error. Invalid account. Please try again.");
			cout << "Enter source account # (1, ... above):";
			cin >> source_account_sequence_number;
		}

		cout << "Enter destination account # (1, ... above): ";
		cin >> destination_account_sequence_number;
		while (!is_account_sequence_number_valid(destination_account_sequence_number)) {
			display_error_message("Error. Invalid account. Please try again.");
			cout << "Enter destination account # (1, ... above):";
			cin >> destination_account_sequence_number;
		}

		if (source_account_sequence_number == destination_account_sequence_number)
			display_error_message("Error. Source and destination accounts cannot be the same. Please try again.");
	} while (source_account_sequence_number == destination_account_sequence_number);

	// TO DO

	cout << "-----------------------\n";
	cout << "Press ENTER to continue\n";
	getchar();getchar();

}
/********************************************************************/
int main() {
	init();
	login();
	display_welcome_message_and_wait_for_user();
	int menu_choice;
	do {
		menu_choice = get_client_main_menu_choice();
		switch (menu_choice) {
			case 1: process_withdraw(); break;
			case 2: process_deposit(); break;
			case 3: process_view_balance(); break;
			case 4: process_transfer(); break;
			case 5: process_view_transactions(); break;
			case 0: break;
			default: { // should never come here!
				display_error_message("Error: Menu choice is invalid. Exiting.");
				exit(1);
			}
		}
	} while (menu_choice != 0);
	process_quit();

	return 0;
}


