#include <iostream>
#include <thread>
#include <mutex>
#include <windows.h>
#include <conio.h>

using namespace std;

std::mutex mu;
static int timer = 0;
int count = 0;
static bool is_running = false;
static bool flagged = false; //returns true if the decrement_time() is running  
void decrement_time()
{
	cout<<"*** Cooking ***"<<endl;
	is_running = true;
	
	while(timer>0)
	{
		mu.lock();
		timer--;
		mu.unlock();
		cout<<" --> "<<timer<<endl;
		Sleep(1000); // Sleeps for 1 second.
	}	
	
	cout<<"*** Cooking End ***"<<endl;
	if(timer==0)
		is_running = false;
}

void cook() //adds 60 seconds of cooking to timer
{
	mu.lock();
	timer+=60;
	if(is_running == true) //if decrement_time() is still running, this will do nothing 
		{
			cout<<"cooking time increased by one minute "<<endl;
			mu.unlock();
		}
	else	// if decrement_time() is not running, that means it has to create a new thread and run decrement_time() to continue decrementing time. 
		{
			cout<<"cooking time set to one minute "<<endl;
			std::thread t4(decrement_time);
			mu.unlock();
			t4.detach();
		}
}

void cancel()//this will just set time to 0. that means abruptly turning off cooking even it has time left to cook.
{
	mu.lock();
	count -= timer;
	timer = 0;
	mu.unlock();
}
int main()
{
	int val;
	cout<<"                        *** WELCOME ***"<<endl;
	cout<<" *** Please enter 1 to start your cooking for one minute ***"<<endl;
	cout<<" *** Please enter 2 again to increase your cooking time by one more minute***"<<endl;
	cout<<" *** Please enter 3 to cancel your cooking ***"<<endl; 
	while(1)
		{
			cin>>val;
			switch(val)
			{	
				case 1:
				{
					count+=60;
					std::thread t1(cook);
					t1.detach();
					break;
				}
				case 2:
				{
					count+=60;
					std::thread t1(cook);
					t1.detach();
					break;
				}
				case 3:
				{
					std::thread t3(cancel);
					t3.detach();
					cout<<"*** Your cooking is done ***"<<endl;
					cout<<" Total cooking time: "<<count<<" seconds"<<endl;
					cout<<"*** THANK YOU ***"<<endl;
					exit(0);					
				}		
			}			
		}
	return 0;
}
