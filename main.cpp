#include<bits/stdc++.h>
using namespace std;
	
void showOptions(){
	//function to display the main menu
	cout<<"\nMAIN MENU"<<endl;
	cout<<"1.Load Cash to ATM"<<endl;
	cout<<"2.Show Customer Details"<<endl;
	cout<<"3.Show ATM Operations"<<endl;
	cout<<"4.Exit"<<endl;
}

//class for cash details in the atm machine
class Cash{
	public:
	//each member variable stores the count of each denomination
	int hundred,fiveHundred,twoThousand,total;
	Cash(){
		hundred = 0;
		fiveHundred = 0;
		twoThousand = 0;
		total = 0;
		saveCash();
	}
	
	//store the cash object in a text file.
	void saveCash(){
		ofstream cashobj;
		cashobj.open("atmcashdetails.txt",ios::ate);
		cashobj.write((char*)this,sizeof(this));
	}
	
	//read the file that stores the atm cash details.
	void readCash(){
		ifstream cashobj;
		cashobj.open("atmcashdetails.txt",ios::in);
		cashobj.read((char*)this,sizeof(this));
	}
	
	//add cash to the atm
	void addCash(){
		cout<<endl<<"ADD CASH"<<endl<<"Enter the denominations as 100's 500's and 2000's"<<endl;
		int a,b,c;
		cin>>a>>b>>c;
		
		hundred += a;
		fiveHundred += b;
		twoThousand += c;
		
		total = (hundred*100)+(fiveHundred*500)+(twoThousand*2000);
		saveCash();
		showCash();
	}
	
	//show the available cash in the atm
	void showCash(){
		//read the cash object since it might have been changed since the last read.
		readCash();
		cout<<"ATM BALANCE "<<endl;
		cout<<"Denomination\tNumber\t\tValue"<<endl;
		cout<<"100\t\t"<<hundred<<"\t\t"<<hundred*100<<endl;
		cout<<"500\t\t"<<fiveHundred<<"\t\t"<<fiveHundred*500<<endl;
		cout<<"2000\t\t"<<twoThousand<<"\t\t"<<twoThousand*2000<<endl;
		cout<<"Total amount available : "<<total<<endl<<endl;
	}
};

//contains individual account details
class Account{
	public:
	int accno,pin,balance;
	string name;
};

//class that contains customer details
class Customers{
	public:
	Account acc[5];

	Customers(){
		//populate the data about the customers
		int accarr[5] = {101,102,103,104,105};
		string namearr[5] = {"Suresh","Ganesh","Magesh","Naresh","Harish"};
		int pinarr[5] = {2343,5432,7854,2345,1907};
		int balarr[5] = {25234,34123,26100,80000,103400};
		for(int i = 0;i<5;i++){
			acc[i].accno = accarr[i];
			acc[i].name = namearr[i];
			acc[i].pin = pinarr[i];
			acc[i].balance = balarr[i];
		}	
		//save the customer data into the file
		saveCustomers();
	}
	
	//saves the object to a file.
	void saveCustomers(){
		ofstream custobj;
		custobj.open("customerdetails.txt",ios::ate);
		custobj.write((char*)this,sizeof(this));
	}
	
	//reads the customer data from the file
	void readCustomers(){
		ifstream custobj;
		custobj.open("customerdetails.txt",ios::in);
		custobj.read((char*)this,sizeof(this));
	}
	void showCustomers(){
		// read the object from the file since the object might have been changed since the last read.
		readCustomers();
		cout<<"\nCUSTOMER DETAILS\n\nAcc No\tAccount Holder\tPin No\tBalance"<<endl;
		for(int i = 0;i<5;i++){
			cout<<acc[i].accno<<"\t"<<acc[i].name<<"\t\t"<<acc[i].pin<<"\t"<<acc[i].balance<<endl;
		}
	}
};


Cash cash;
Customers customer;

class ATM{
	public:
		Account currentUser;
		ATM(){
			customer.readCustomers();
			cash.readCash();
		}
		//display the menu
		void showAtmOperations(){
			cout<<"ATM OPERATIONS"<<endl;
			cout<<"1.Check Balance"<<endl;
			cout<<"2.Withdraw Money"<<endl;
			cout<<"3.Transfer Money"<<endl;
			cout<<"4.Check ATM balance"<<endl;
			cout<<"5.Go to main menu"<<endl;
		}
		
			//validate the user with account number and pin number
		void validateUser(){
			//read the customer details from the file
			customer.readCustomers();
			
			bool val = false;
			
			cout<<"\n\nATM OPERATIONS \n";
			
			while(!val){
				cout<<"Enter the Account Number : ";
				int acc;
				cin>>acc;
				cout<<"Enter the Pin Number : ";
				int pin;
				cin>>pin;
				//loop through the customer list to check if any account matches with the account number and pin number entered by the user.
				for(int i = 0;i<5;i++){
					if(customer.acc[i].accno == acc && customer.acc[i].pin == pin) {
						currentUser = customer.acc[i];
						val = true;	
					}
				}
				
				//if there's no match loop untill you get a valid match
				if(!val){
					cout<<"Enter valid credentials"<<endl;
					continue;
				}
				
				showAtmOperations();
				//choices for atm operations
				while(1){
					int choice;
					cin>>choice;
					switch(choice){
						case 1 :
							checkBalance();
							break;
						case 2:
							withdraw();
							break;
						case 3:
							transfer();
							break;
						case 4:
							checkAtmBalance();
							break;
						case 5:
							return;		
					}
					showAtmOperations();		
				}
			}
		}
		
		void checkBalance(){
			cash.readCash();
			cout<<"Balance Amount : "<<currentUser.balance<<endl;
		}
		
		void withdraw(){
			cout<<"Withdraw"<<endl;
			int pin,amount;
			cout<<"Enter the pin number : ";
			cin>>pin;
			if(pin != currentUser.pin) return;
			
			cout<<"Enter the amount to be withrawn(100 to 10000): ";
			cin>>amount;
			//amount not in the range 100 to 10000
			while(amount < 100 || amount > 10000){
				cout<<endl<<"Enter valid amount in the range 1000 to 10000";
				cin>>amount;
			}
			
			//amount greater than user's balance
			while(amount > currentUser.balance){
				cout<<"Insufficient Balance in your account"<<endl;
				cin>>amount;
			}
			
			//amount greater than atm's total balance;
			while(amount > cash.total){
				cout<<endl<<"Insufficient funds in bank. Please enter amount less than "<<cash.total<<endl;
				cin>>amount;
			}
			int fromIndex;
			int i;
			for(i = 0;i<5;i++){
				if(currentUser.accno == customer.acc[i].accno){
					customer.acc[i].balance -= amount;
					break;
				};
			}
			cash.total = cash.total - amount;
			customer.saveCustomers();
			cash.saveCash();
		}
		
		void transfer(){
			//transfer money from one account to another
			cout<<"Transfer Money (1000 to 10000) "<<endl;
			int amount,acc,fromIndex,toIndex;
			cout<<"Enter the amount to be transferred : ";
			cin>>amount;
			
			//loop until the entered amount is valid
			while(amount < 1000 || amount > 10000 || amount > currentUser.balance){
				cout<<endl<<"Enter valid amount in the range 1000 to 10000 or amount less than your account balance: ";
				cin>>amount;
			}
			
			cout<<"Enter the account number to transfer : ";
			cin>>acc;
			
			for(int i = 0;i<5;i++){
				if(currentUser.accno == customer.acc[i].accno) fromIndex = i;
				else if(acc == customer.acc[i].accno) toIndex = i;
			}
			
			//transfer the amount from sender to receiver
			customer.acc[fromIndex].balance -= amount;
			customer.acc[toIndex].balance += amount;
			currentUser = customer.acc[fromIndex];
			customer.saveCustomers();
		}
		
		//prints the available amount in atm in total
		void checkAtmBalance(){
			cash.showCash();	
		}	
};


ATM atm;
int main(){
	int choice;
	showOptions();

	while(1){
	cout<<endl;
	cin>>choice;
		switch(choice){
			case 1 :
				cash.addCash();
				break;
			case 2:
				customer.showCustomers();
				break;
			case 3:
				atm.validateUser();
				break;
			case 4:
				exit(0);		
		}
	showOptions();		
	}
	return 0;
}
