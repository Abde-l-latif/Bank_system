#include <iostream>
#include <stdio.h>
#include <string>
#include <fstream>
#include <vector>
#include <iomanip>

using namespace std;

enum e_choice { show_clients = 1, add_client, delete_client, update_client, find_client, transaction , manage_users , exit_program };

enum e_transaction {deposit = 1 , withdraw , total_balance , main_menu};

struct s_client {
	string account_number = "";
	string pin_code = "";
	string full_name = "";
	string phone_number = "";
	double account_balance = 0;
	bool delete_client = false;
};

struct s_user {
	string username = "";
	string password = "";
	short permission = 0; 
	bool delete_user = false;
};




/* GOLOBAL CONSTANTS PERMISSIONS */
const short show_client_list_permission = 1;
const short add_new_client_permission = 2;
const short delete_client_permission = 4;
const short update_permission = 8;
const short find_client_permission = 16;
const short transaction_permission = 32;
const short manage_user_permission = 64;


/* ===== CUSTOM FUNCTIONS  ===== */

void custom_header(string msg)
{
	system("cls");
	cout << "============================================================\n\n";
	cout << " \t\t   "<< msg << "\n\n";
	cout << "============================================================\n\n";
}

void custom_pause(string msg)
{
	cout << msg ;
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	cin.get();
}




/* ======== CONVERSION OF TYPES ========= */

vector<string> convert_string_to_vector(string line , string delim = "#//#")
{
	vector<string> client;
	int position = 0;
	string word = ""; 

	while ((position = line.find(delim)) != string::npos)
	{
		word = line.substr(0, position);
		if (word != "")
			client.push_back(word);

		line.erase(0, position + delim.length());
			
	}

	if(line != "")
		client.push_back(line);

	return client;
}

string convert_struct_to_string(s_client client, string delim = "#//#")
{
	string text = "";

	text += client.account_number + delim;
	text += client.pin_code + delim;
	text += client.full_name + delim;
	text += client.phone_number + delim;
	text += to_string(client.account_balance);

	return text; 
}

s_client convert_vector_to_struct(vector<string>& client)
{
	s_client record;

	record.account_number = client[0];
	record.pin_code = client[1];
	record.full_name = client[2];
	record.phone_number = client[3];
	record.account_balance = stod(client[4]);

	return record;
}

s_user convert_vector_to_structUSER(vector<string>& user)
{
	s_user record;

	record.username = user[0];
	record.password = user[1];
	record.permission = stoi(user[2]);

	return record;
}



/* ====== FILES CLIENT ====== */

const string bank_file = "Abank_file.txt";

void add_to_file(string record)
{
	fstream My_file;

	My_file.open(bank_file, ios::out | ios::app);

	if (My_file.is_open())
	{
		My_file << record << endl;

		My_file.close();
	}

}

void write_file(vector<s_client> &all_clients)
{
	fstream My_file;

	My_file.open(bank_file, ios::out );

	if (My_file.is_open())
	{
		
		for (s_client& C : all_clients)
		{
			
			if (C.delete_client == false)
				My_file << convert_struct_to_string(C) << endl;

		}


		My_file.close();
	}
}

vector<s_client> read_file()
{
	vector<s_client> all_clients;
	fstream My_file;
	My_file.open(bank_file, ios::in);
	if (My_file.is_open())
	{
		string line;

		while (getline(My_file, line))
		{
			vector<string> person = convert_string_to_vector(line);
			s_client client = convert_vector_to_struct(person);
			all_clients.push_back(client);
		}

		My_file.close();
	}
	return all_clients;
}




void denied_func()
{
	cout << "-------------------------------------------------" << endl;
	cout << "Access Denied ," << endl;
	cout << "You don't have permission to do this ," << endl;
	cout << "please contact your Admin" << endl;
	cout << "-------------------------------------------------" << endl;
}

bool check_for_permission(short user_permission, short original_permission)
{
	return ((user_permission & original_permission) == original_permission);
}


/* ====== SHOW ALL CLIENTS ====== */

void header_table()
{
	cout << "\n__________________________________________________________________________________________________________________\n" << endl;
	cout << " | " << left << setw(20) << "Account Number";
	cout << " | " << left << setw(10) << "Pin Code";
	cout << " | " << left << setw(35) << "Client name";
	cout << " | " << left << setw(15) << "Phone Number";
	cout << " | " << left << setw(25) << "Balance" ;
	cout << "\n__________________________________________________________________________________________________________________\n" << endl;
}

void body_table(s_client client)
{
	cout << " | " << left << setw(20) << client.account_number;
	cout << " | " << left << setw(10) << client.pin_code;
	cout << " | " << left << setw(35) << client.full_name;
	cout << " | " << left << setw(15) << client.phone_number;
	cout << " | " << left << setw(25) << to_string(client.account_balance);
	cout << "\n__________________________________________________________________________________________________________________\n" << endl;
}

void show_all_clients(s_user& user_info)
{
	system("cls");

	if ((!check_for_permission(user_info.permission, show_client_list_permission)))
	{
		denied_func();
	}
	else
	{
		vector<s_client> clients = read_file();
		cout << "\n\t\t\t\t\t\t CLIENT LIST (" << clients.size() << ") Client(s). " << endl;
		header_table();
		for (s_client& C : clients)
			body_table(C);
	}

	custom_pause("Press Enter to go back to the main menu...");
}

bool check_account_number_existence(string client , vector<s_client>& all_clients)
{

	for (s_client& C : all_clients)
	{
		if (C.account_number == client)
			return true;
	}

	return false;

}

string read_account_number()
{
	string account_num;
	cout << "Enter an account number : ";
	cin >> account_num;
	return account_num;
}



/* ====== ADD NEW CLIENT ====== */

s_client read_new_client()
{
	s_client client; 

	cout << "Enter account number :  ";
	cin >> client.account_number; 

	cout << "Enter pin code :  ";
	cin >> client.pin_code; 

	cout << "Enter full name :  ";
	getline(cin >> ws, client.full_name);

	cout << "Enter phone number :  ";
	cin >> client.phone_number;

	cout << "Enter account balance :  ";
	cin >> client.account_balance;

	return client;
}

void add_new_client(s_user& user_info)
{
	if ((!check_for_permission(user_info.permission, add_new_client_permission)))
	{
		system("cls");

		denied_func();
	}
	else
	{
		vector<s_client> all_clients = read_file();
		char repeat = 'y';
		custom_header("ADD NEW CLIENT SCREEN");
		cout << "Adding new client :" << endl;
		do {

			s_client client = read_new_client();
			if (!check_account_number_existence(client.account_number, all_clients))
			{

				add_to_file(convert_struct_to_string(client));

				cout << "Client added successfully, you wanna add another client Y/N  : ";
				cin >> repeat;

			}
			else {
				cout << "\nThe account number already exists, try one more time... \n" << endl;
			}

		} while (tolower(repeat) == 'y');

	}

	custom_pause("Press Enter to go back to the main menu...");

}





/* ====== FIND CLIENT DETAILS ====== */

void display_client_details(s_client client)
{
	cout << "\nthe folowing are client details : " << endl;
	cout << "_____________________________________________________\n\n";
	cout << left << setw(20) << "Account number" << ":  " << client.account_number << endl;
	cout << left << setw(20) << "Pin code" << ":  " << client.pin_code << endl;
	cout << left << setw(20) << "Full Name" << ":  " << client.full_name << endl;
	cout << left << setw(20) << "Phone number" << ":  " << client.phone_number << endl;
	cout << left << setw(20) << "Account balance" << ":  " << to_string(client.account_balance) << endl;
	cout << "_____________________________________________________\n\n";
}

bool client_details(string Account_number)
{

	vector<s_client> all_clients = read_file();

	if (check_account_number_existence(Account_number , all_clients))
	{
		for (s_client& C : all_clients)
		{
			if (C.account_number == Account_number)
			{
				display_client_details(C);
				break;
			}
		}
		return true;
	}
	else {
		return false;
	}
}

void find_single_client(s_user& user_info)
{
	if ((!check_for_permission(user_info.permission, find_client_permission)))
	{
		system("cls");

		denied_func();
	}
	else
	{
		custom_header("FIND CLIENT SCREEN");
		string Account_number = read_account_number();
		if (!client_details(Account_number))
		{
			cout << "account number not found ! " << endl;
		}
	}

	custom_pause("Press Enter to go back to the main menu...");
}




/* ======= DELETE CLIENTS ======== */

vector<s_client> mark_delete_client(string account_num , vector<s_client> all_clients)
{

	for (s_client& C : all_clients)
	{
		if (C.account_number == account_num)
			C.delete_client = true;
	}

	return all_clients;
}

void delete_single_client(s_user& user_info)
{
	if ((!check_for_permission(user_info.permission, delete_client_permission)))
	{
		system("cls");
		denied_func();
	}
	else
	{
		vector<s_client> all_clients = read_file();

		custom_header("DELETE CLIENT SCREEN");
		string Account_number = read_account_number();

		if (client_details(Account_number))
		{

			vector<s_client> new_clients = mark_delete_client(Account_number, all_clients);
			write_file(new_clients);
			cout << "the account has been deleted successfully. \n";
		}
		else
		{
			cout << "account number not found !" << endl;
		}
	}

	custom_pause("Press Enter to go back to the main menu...");
	
}




/* ======= UPDATE CLIENTS ======== */

void update_client_X(string account_number , vector<s_client>& updated_clients)
{
	s_client new_data =  read_new_client();

	for (s_client& C : updated_clients)
	{
		if (C.account_number == account_number)
			C = new_data;
	}
	write_file(updated_clients);
}

void update_single_client(s_user& user_info)
{
	if ((!check_for_permission(user_info.permission, update_permission)))
	{
		system("cls");

		denied_func();
	}
	else
	{
		vector<s_client> updated_clients = read_file();

		custom_header("UPDATE CLIENT INFORMATION SCREEN");

		string Account_number = read_account_number();

		if (check_account_number_existence(Account_number, updated_clients))
		{
			char clearancy = 'y';

			client_details(Account_number);

			cout << "are you sure you want to update this client Y/N : ";

			cin >> clearancy;

			if (tolower(clearancy) == 'y')
			{

				update_client_X(Account_number, updated_clients);

			}
		}
		else
		{
			cout << "wrong account number ." << endl;
		}
	}

	custom_pause("Press Enter to go back to the main menu...");
}



/* ====== TRANSACTIONS ====== */

void transaction_menu()
{
	custom_header("TRANSACTIONS MENU SCREEN");
	printf("         [%d] Deposit. \n", 1);
	printf("         [%d] Withdraw. \n", 2);
	printf("         [%d] Total balances. \n\n", 3);
	printf("         [%d] Main menu. \n\n", 4);
	cout << "============================================================\n\n";
}

e_transaction read_transaction_choice()
{
	short number;
	do 
	{
		cout << "choose what do you want to do ? [1 to 4] : ";
		cin >> number;

	} while (number < 1 || number > 4);

	return (e_transaction)number;
}

void header_transaction_table()
{
	cout << "\n__________________________________________________________________________________________________________________\n" << endl;
	cout << " | " << left << setw(25) << "Account Number";
	cout << " | " << left << setw(45) << "Client name";
	cout << " | " << left << setw(35) << "Balance";
	cout << "\n__________________________________________________________________________________________________________________\n" << endl;
}

void body_transaction_table(s_client client)
{
	cout << " | " << left << setw(25) << client.account_number;
	cout << " | " << left << setw(45) << client.full_name;
	cout << " | " << left << setw(35) << to_string(client.account_balance);
	cout << "\n__________________________________________________________________________________________________________________\n" << endl;
}

void show_total_balance()
{
	system("cls");
	double calc_total_balance = 0;
	vector<s_client> all_clients = read_file();
	cout << "\n\t\t\t\t\t\t BALANCES LIST (" << all_clients.size() << ") Client(s). " << endl;
	header_transaction_table();
	for (s_client& client : all_clients)
	{
		body_transaction_table(client);
		calc_total_balance += client.account_balance;
	}
	cout << "\n\t\t\t\t\t Total balances = " << to_string(calc_total_balance) << endl << endl;
	custom_pause("Press Enter to go back to the transaction menu...");
}

int read_number(string msg)
{
	int num = 0; 
	do
	{
		cout << msg;
		cin >> num;

	} while (num <= 0);

	return num;
}

void calc_transaction(string Account_number , int amount_number , vector <s_client>& all_clients)
{

	for (s_client& C : all_clients)
	{
		if (C.account_number == Account_number)
			C.account_balance += (double)amount_number;
	}

	write_file(all_clients);

}

void deposit_action()
{
	system("cls");
	custom_header("DEPOSIT SCREEN");
	vector <s_client> all_clients = read_file();
	char repeat = 'y' , response = 'n';
	do
	{
		string Account_number = read_account_number();

		if (client_details(Account_number))
		{
			repeat = 'n';

			int amount_number = read_number("Please enter deposit amount ? :  ");

			cout << "Are you sure you want to perform this transaction ? Y/N  : ";

			cin >> response;

			if (tolower(response == 'y'))
			{
				calc_transaction(Account_number , amount_number , all_clients );
			}
		}
		else {
			cout << "Account number not found!" << endl;
			repeat = 'y';
		}

	} while (repeat == 'y');

	custom_pause("Press Enter to go back to the transaction menu...");
}

void withdraw_action()
{
	system("cls");
	custom_header("WITHDRAW SCREEN");
	vector <s_client> all_clients = read_file();
	char repeat = 'y', response = 'n';
	do
	{
		string Account_number = read_account_number();

		if (client_details(Account_number))
		{
			repeat = 'n';

			int amount_number = read_number("Please enter withdraw amount ? :  ");

			for (s_client& C : all_clients)
			{
				if (C.account_number == Account_number)
				{
					while (C.account_balance < double(amount_number))
					{
						cout << "Amount Exceeds the balance , you can withdraw up to : " << to_string(C.account_balance) << endl;
						amount_number = read_number("Please enter withdraw amount ? :  ");
					}
					break;
				}
			}

			cout << "Are you sure you want to perform this transaction ? Y/N  : ";

			cin >> response;

			if (tolower(response) == 'y')
				calc_transaction(Account_number, amount_number * -1, all_clients);

		}
		else {
			cout << "Account number not found!" << endl;
			repeat = 'y';
		}

	} while (repeat == 'y');

	custom_pause("Press Enter to go back to the transaction menu...");
}

void check_transaction_choice(e_transaction choice)
{
	switch (choice)
	{
	case e_transaction::total_balance: 
	{
		show_total_balance();
		break;
	}
	case e_transaction::deposit:
	{
		deposit_action();
		break;
	}
	case e_transaction::withdraw:
	{
		withdraw_action();
		break;
	}
	}
}

void transaction_section(s_user& user_info)
{
	if ((!check_for_permission(user_info.permission, transaction_permission)))
	{
		system("cls");

		denied_func();

		custom_pause("press enter key to return to the menu :");
	} 
	else
	{
		e_transaction transaction_choice;

		do
		{

			transaction_menu();
			transaction_choice = read_transaction_choice();
			check_transaction_choice(transaction_choice);

		} while (transaction_choice != e_transaction::main_menu);
	}

}




/* ====== FILES USERS ====== */

const string user_file = "users.txt";

string convert_struct_to_string(s_user user, string delim = "#//#")
{
	string text = "";

	text += user.username + delim;
	text += user.password + delim;
	text += to_string(user.permission);

	return text;
}

vector<s_user> read_file_user()
{
	vector<s_user> all_users;
	fstream My_file;
	My_file.open(user_file, ios::in);
	if (My_file.is_open())
	{
		string line;

		while (getline(My_file, line))
		{
			vector<string> user = convert_string_to_vector(line);
			s_user v_user = convert_vector_to_structUSER(user);
			all_users.push_back(v_user);
		}

		My_file.close();
	}
	return all_users;
}

void add_to_user_file(string record)
{
	fstream My_file;

	My_file.open(user_file, ios::out | ios::app);

	if (My_file.is_open())
	{
		My_file << record << endl;

		My_file.close();
	}

}

void write_user_file(vector<s_user>& all_users)
{
	fstream My_file;

	My_file.open(user_file, ios::out);

	if (My_file.is_open())
	{

		for (s_user& U : all_users)
		{

			if (U.delete_user == false)
				My_file << convert_struct_to_string(U) << endl;

		}


		My_file.close();
	}
}




/* ============== USERS SECTION  =============== */

s_user read_user_info()
{
	s_user USER; 

	cout << "Enter username : ";
	getline(cin, USER.username);

	cout << endl;

	cout << "Enter password : ";
	cin >> USER.password;

	return USER;
}

enum e_user {list_user = 1 , new_user , delete_user , update_user , find_user , Umain_menu };

void user_screen_menu()
{
	printf("         [%d] List Users. \n", 1);
	printf("         [%d] Add new User. \n", 2);
	printf("         [%d] Delete User. \n", 3);
	printf("         [%d] Update User information. \n", 4);
	printf("         [%d] Find User. \n\n", 5);
	printf("         [%d] Main menu. \n", 6);
	cout << "============================================================\n\n";
}

void header_user_table()
{
	cout << "\n__________________________________________________________________________________________________________________\n" << endl;
	cout << " | " << left << setw(25) << "Username";
	cout << " | " << left << setw(45) << "Password";
	cout << " | " << left << setw(35) << "Permission";
	cout << "\n__________________________________________________________________________________________________________________\n" << endl;
}

void body_user_table(s_user user)
{
	cout << " | " << left << setw(25) << user.username;
	cout << " | " << left << setw(45) << user.password;
	cout << " | " << left << setw(35) << to_string(user.permission);
	cout << "\n__________________________________________________________________________________________________________________\n" << endl;
}

void list_user_area()
{
	system("cls");
	vector<s_user> All_users = read_file_user();
	cout << "\t\t\t\t\t user list ( " <<All_users.size() << " ) user(s) ." << endl;
	header_user_table();
	for (s_user& U : All_users)
	{
		body_user_table(U);
	}
	custom_pause("Press any key to go back to user menu management");
}



/* ADD NEW USER */


bool check_username_exists(string username , vector<s_user>& All_users)
{
	for (s_user& U : All_users)
	{
		if (U.username == username)
			return true; 
	};
	return false;
}

s_user read_new_user(vector<s_user> &All_users)
{
	s_user user;

	cout << "Enter username : ";
	cin >> user.username; 

	while (check_username_exists(user.username, All_users))
	{
		cin.ignore(numeric_limits<streamsize>::max() , '\n');
		cout << "user { " << user.username << " } Already exists ! enter username again : ";
		cin >> user.username; 
	}

	cout << "Enter password : ";
	cin >> user.password;

	return user;
}

void add_single_permission(short & user_permission , short new_permission )
{
		user_permission += new_permission;
}

void given_permissions(s_user& user)
{
	char permission = 'y';
	cout << "Do you want to give access to : " << endl;

	cout << "Show client list y/n : ";
	cin >> permission; 
	if(tolower(permission) == 'y')
		add_single_permission(user.permission , show_client_list_permission);
	cin.ignore(1000, '\n');
	cout << endl;

	cout << "Add new client y/n : ";
	cin >> permission;
	if (tolower(permission) == 'y')
		add_single_permission(user.permission , add_new_client_permission);
	cin.ignore(1000, '\n');
	cout << endl;

	cout << "delete client y/n : ";
	cin >> permission;
	if (tolower(permission) == 'y')
		add_single_permission(user.permission, delete_client_permission);
	cin.ignore(1000, '\n');
	cout << endl;

	cout << "update client y/n : ";
	cin >> permission;
	if (tolower(permission) == 'y')
		add_single_permission(user.permission, update_permission);
	cin.ignore(1000, '\n');
	cout << endl;

	cout << "find client y/n : ";
	cin >> permission;
	if (tolower(permission) == 'y')
		add_single_permission(user.permission, find_client_permission);
	cin.ignore(1000, '\n');
	cout << endl;

	cout << "transactions y/n : ";
	cin >> permission;
	if (tolower(permission) == 'y')
		add_single_permission(user.permission, transaction_permission);
	cin.ignore(1000, '\n');
	cout << endl;

	cout << "manage users y/n : ";
	cin >> permission;
	if (tolower(permission) == 'y')
		add_single_permission(user.permission, manage_user_permission);
}

void add_new_user()
{
	custom_header("ADD NEW USER SCREEN");

	vector<s_user> All_users = read_file_user();
	
	char access = 'y';
	char repeat = 'y';

	do {

		s_user user = read_new_user(All_users);

		cout << "Do you want to give fill access ? Y/N : ";
		cin >> access;

		if (tolower(access) == 'y')
		{
			user.permission = -1;
			add_to_user_file(convert_struct_to_string(user));
			cout << "the user has been added successfully , do you want to add one more user ? Y/N : ";
			cin >> repeat;
		}
		else
		{
			given_permissions(user);
			add_to_user_file(convert_struct_to_string(user));
			cin >> repeat;
		}

	} while (repeat == 'y' || repeat == 'Y');

}


/* find user */

void display_user_info(s_user user)
{
	cout << "\n\n The following are user details : " << endl;
	cout << "--------------------------------------------" << endl;
	cout << "username      : " << user.username << endl;
	cout << "password      : " << user.password << endl;
	cout << "permissions   : " << to_string(user.permission) << endl;
	cout << "--------------------------------------------" << endl;
}

short find_user_option()
{
	custom_header("FIND USER SCREEN");

	vector<s_user> All_users = read_file_user();

	string username = "";

	cout << "please enter user name : ";
	cin >> username; 

	if (!check_username_exists(username, All_users))
	{
		cout << "user { " << username << " } does not exists ! ";
		custom_pause("press enter key to go back to user menu");
		return 0;
	};

	for (s_user& U : All_users)
	{
		if (U.username == username)
		{
			display_user_info(U);
			break;
		}
	}
	custom_pause("press enter key to go back to user menu");
	return 0;
}



/* delete user */

short delete_user_option()
{
	custom_header("DELETE USER SCREEN");

	vector<s_user> All_users = read_file_user();
	string username = "";
	char del = 'y';

	cout << "please enter username : ";
	cin >> username;

	if (!check_username_exists(username, All_users))
	{
		cout << "user { " << username << " } does not exists ! ";
		custom_pause("press enter key to go back to user menu");
		return 0;
	};

	for (s_user& U : All_users)
	{
		if (U.username == username)
		{
			display_user_info(U);
			cout << "are you sure you want to delete this user ? y/n : ";
			cin >> del;
			if (tolower(del) == 'y')
			{
				U.delete_user = true;
				write_user_file(All_users);
				cout << "user deleted successfully ." << endl;
			}
			break;
		}
	}

	custom_pause("press enter key to go back to user menu");

	return 0;
}



/* Update user */

short update_user_option()
{
	custom_header("UPDATE USER SCREEN");

	vector<s_user> All_users = read_file_user();
	string username = "";
	char up = 'y';

	cout << "please enter username : ";
	cin >> username;

	if (!check_username_exists(username, All_users))
	{
		cout << "user { " << username << " } does not exists ! ";
		custom_pause("press enter key to go back to user menu");
		return 0;
	};

	for (s_user& U : All_users)
	{
		if (U.username == username)
		{
			display_user_info(U);
			cout << "are you sure you want to update this user ? y/n : ";
			cin >> up;
			cin.ignore(1000, '\n');
			if (tolower(up) == 'y')
			{
				cout << "enter password : ";
				cin >> U.password; 
				cin.ignore(1000, '\n');
				given_permissions(U);
				write_user_file(All_users);
				cout << "user updated successfully ." << endl;
			}
			break;
		}
	}

	custom_pause("press enter key to go back to user menu");
	return 0;
}






void check_choice_for_user_menu(e_user choice)
{
	switch (choice)
	{
	case e_user::list_user :
	{
		list_user_area();
		break; 
	}
	case e_user::new_user : 
	{
		add_new_user(); 
		break; 
	}
	case e_user::delete_user:
	{
		delete_user_option();
		break;
	}
	case e_user::find_user:
	{
		find_user_option();
		break;
	}
	case e_user::update_user:
	{
		update_user_option();
		break;
	}
	}
}

e_user read_choice_for_user_menu()
{
	short number = 1;
	do {

		cout << "choose what do you want to do ? [ 1 to 6] : ";
		cin >> number;

	} while (number < 1 || number > 6);

	return (e_user)number;

}

void user_managment(s_user &user_info)
{
	if ((!check_for_permission(user_info.permission, manage_user_permission)))
	{
		system("cls");
		denied_func();
		custom_pause("press enter key to return to the menu :");
	}
	else
	{
		e_user choice;
		do
		{
			custom_header("MANAGE USERS MENU SCREEN");
			user_screen_menu();

			choice = read_choice_for_user_menu();

			check_choice_for_user_menu(choice);

		} while (choice != e_user::Umain_menu);
	}

}





/* ============ LOGIN ============= */


bool account_validation(s_user& user , vector<s_user>& all_users)
{
	for (s_user& U : all_users)
	{
		if (U.username == user.username && U.password == user.password)
		{
			user.permission = U.permission; 
			return true; 
		}
	}

	return false;
}

s_user login()
{

	custom_header("LOGIN SCREEN");
	
	s_user user_info = read_user_info();
	vector<s_user> All_users = read_file_user();

	while (!account_validation(user_info, All_users))
	{
		system("cls");
		custom_header("LOGIN SCREEN");
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "Unvalid username/password !!" << endl;
		user_info = read_user_info();

	}

	return user_info;

}




/*======= STARTS =======*/

void check_user_choice(e_choice choice , s_user &user_info)
{
	switch (choice)
	{
	case e_choice::show_clients:
	{
		show_all_clients(user_info);
		break;
	}
	case e_choice::add_client :
	{
		add_new_client(user_info);
		break;
	}
	case e_choice::delete_client:
	{
		delete_single_client(user_info);
		break;
	}
	case e_choice::update_client :
	{
		update_single_client(user_info);
		break;
	}
	case e_choice::find_client : 
	{
		find_single_client(user_info);
		break;
	}
	case e_choice::transaction:
	{
		transaction_section(user_info);
		break;
	}
	case e_choice::manage_users : 
	{
		user_managment(user_info);
		break;
	}
	}
}

void main_menu_screen()
{
	custom_header("MAIN MENU SCREEN");
	printf("         [%d] Show client list. \n", 1);
	printf("         [%d] Add new client. \n", 2);
	printf("         [%d] Delete client. \n", 3);
	printf("         [%d] Update client information. \n", 4);
	printf("         [%d] Find client. \n", 5);
	printf("         [%d] Transaction. \n", 6);
	printf("         [%d] Manage users. \n\n", 7);
	printf("         [%d] Logout. \n", 8);
	cout << "============================================================\n\n";
}

e_choice read_user_choice()
{
	short number = 1; 
	do {

		cout << "choose what do you want to do ? [ 1 to 8 ] : ";
		cin >> number;

	} while (number < 1 || number > 8);
	
	return (e_choice)number;
}

void start_program(s_user &user_info)
{
	e_choice choice;
	do {
		main_menu_screen();
		choice = read_user_choice();
		check_user_choice(choice , user_info);
	} while (choice != e_choice::exit_program);

}



int main()
{

	s_user user_info = login();

	start_program(user_info);

	return 0;
}