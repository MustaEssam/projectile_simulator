#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <conio.h>
#include <Windows.h>
#include <chrono>
#include <thread>
#include<stdlib.h>
using namespace std;
using namespace std::this_thread;     // sleep_for
using namespace std::chrono_literals; // ns, us, ms, s, h, etc.
using std::chrono::system_clock;

// key codes
#define KEY_UP 72
#define KEY_DOWN 80
#define SPACE_BAR 32
#define ENTER 13

CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
HANDLE h;

// class used for parametrs 
class YourParameters
{
protected:
	float mass, b, mu, e, diameter;
	ofstream parameters;
	bool saved;
public:
	// Constactor with zero inithialization
	YourParameters() {
		mass = 0;
		b = 0;
		mu = 0;
		e = 0;
		diameter = 0;
		saved = false;
	}
	// Constactor with values
	YourParameters(float mass, float b, float mu, float e, float diameter) {
		this->mass = mass;
		this->b = b;
		this->mu = mu;
		this->e = e;
		this->diameter = diameter;
		saved = false;
	}
	// Setting values
	void setData(float mass, float b, float mu, float e, float diameter) {
		this->mass = mass;
		this->b = b;
		this->mu = mu;
		this->e = e;
		this->diameter = diameter;
		saved = false;
	}
	//check if the entered number is negative or not
	float isNegativ(float num) {
		while (num < 0) {
			cout << "please enter a positive number ---> ";
			cin >> num;
		}
		return num;
	}
	// saving the simulation parameters to a file
	void save()
	{
		parameters.open("ParametersData.txt", std::ios_base::out);
		parameters << mass << endl
			<< b << endl
			<< mu << endl
			<< e << endl
			<< diameter << endl;
		parameters.close();
		saved = true;
	}
	// Loading the saved Parameters
	void load() {
		ifstream file;
		file.open("ParametersData.txt", std::ios_base::in);
		if (!file.is_open()) {			//no file found
			cout << "No file saved." << endl;
			saved = false;
			return;
		}
		else {
			file >> mass
				>> b
				>> mu
				>> e
				>> diameter;
			file.close();
			cout << "Data is loaded successfuly" << endl;
			saved = true;
		}
	}
	bool isSaved() {
		return saved;
	}
	// Reset all varibles to zero
	void delet() { mass, b, mu, e, diameter = 0; }
	// setters & getters Functions
	void setMass(float mass) { this->mass = mass; }
	float getMass() { return mass; }
	void setB(float b) { this->b = b; }
	float getB() { return b; }
	void setMu(float mu) { this->mu = mu; }
	float getMu() { return mu; }
	void setE(float e) { this->e = e; }
	float getE() { return e; }
	void setDiameter(float diameter) { this->diameter = diameter; }
	float getDiameter() { return diameter; }
	// Destructor
	~YourParameters() {}
};  // end of class parameters



	// class used for simulation 
class simulation : public YourParameters
{
private:
	float ax, ay, vx, vy, deltaVx, deltaVy, deltaX, deltaY, y
		, theta, initialVelocity, x, T;
	int factor = 0;
	const float g = 9.81;
	const double pi = 22 / 7;
	const float deltaT = 0.01;
	ofstream simulationFile;
	ifstream CSVfile;
public:
	// Constactor with zero inithialization
	simulation()
	{
		theta = 0.0;
		y = 0.0;
		initialVelocity = 0.0;
		x = 0.0;
		T = 0.0;
	}
	// constructor with values 
	simulation(float initialAngle, float v, float h)
	{
		theta = initialAngle;
		initialVelocity = v;
		y = h;
		T = 0.0;
		x = 0.0;
	}
	// setting values using function 
	void setSimulationData(float initialAngle, float v, float h)
	{
		theta = initialAngle;
		initialVelocity = v;
		y = h;
		T = 0.0;
		x = 0.0;
	}
	// Setting hight
	void setHight(float h) {
		y = h;
	}
	// projectile calculation 
	float Ymove(float Vy) {
		vy = Vy;
		ay = ((-mass*g) - (b*vy)) / mass;
		deltaVy = ay * deltaT;
		vy += deltaVy;
		deltaY = vy * deltaT;
		y += deltaY;
		return y;
	}
	float Xmove(float Vx) {
		vx = Vx;
		ax = (-b*vx) / mass;
		deltaVx = ax * deltaT;
		vx += deltaVx;
		deltaX = vx * deltaT;
		x += deltaX;
		return x;
	}
	// geting the angle using arrows
	void angle() {
		int c = 0;
		print("Angle : ", theta);
		while (true)
		{
			switch ((c = _getch())) {
			case KEY_UP:
				if (theta < 90) {
					theta++;
					print("Angle : ", theta);
				}
				break;
			case KEY_DOWN:
				if (theta > -90) {
					theta--;
					print("Angle : ", theta);
				}
				break;
			case ENTER:
				return;
			}
		}
	}
	// Getting the speed using the space bar
	void spaceHit() {
		print("velocity : ", initialVelocity);
		_getch();
		sleep_for(1s);
		while (_kbhit()) {
			if (_getch() == SPACE_BAR) {
				initialVelocity++;
			}
			sleep_for(150ms);
			print("velocity : ", initialVelocity);
		}
	}
	// Getting Cursor position
	void getCursor() {
		h = GetStdHandle(STD_OUTPUT_HANDLE);
		GetConsoleScreenBufferInfo(h, &bufferInfo);
	}
	// printing in the same position (Rewrite on the same line)
	void print(string varible, float value) {
		SetConsoleCursorPosition(h, bufferInfo.dwCursorPosition);
		if (value > 0) {
			cout << varible << '+' << value << "   " << endl;
		}
		else {
			cout << varible << value << "   " << endl;
		}
	}
	// Simulating and saving the data to a desktop file or to the D:\ drive
	void calculate() {
		if (initialVelocity <= 10) { factor = 15; }
		else if (initialVelocity <= 20) { factor = 7; }
		else if (initialVelocity <= 40) { factor = 5; }
		else { factor = 2; }
		simulationFile.open("SimulationFile.csv", std::ios_base::out);
		vx = initialVelocity*cos(theta*pi / 180);
		vy = initialVelocity*sin(theta*pi / 180);
		simulationFile << 'x' << ',' << 'y' << ',' << 't' << endl;
		printSimulation(x * factor, y * factor);
		sleep_for(100us);
		do {
			y = Ymove(vy);
			x = Xmove(vx);
			T += deltaT;
			simulationFile << x << ',' << y << ',' << T << endl;
			printSimulation(x * factor, y * factor);
			sleep_for(200ms);
			if (y <= 0) {
				y = 0;
				vy = -e*vy;
				vx = vx + mu*(e - 1)*vx;
			}
		} while (vx >= 0.0001);			 // end of do-while loop
		simulationFile.close();
		cout << endl << endl << "This Simulation is 1:" << factor << " (m:pixel)" << endl << endl
			<< "Your simulation data file is in the application folder " << endl;
	} // end of calculation function 
	  // function used to print the simulation data on screen
	void printSimulation(int xp, int yp) {
		h = GetStdHandle(STD_OUTPUT_HANDLE);
		_COORD pos;
		pos.X = xp;
		pos.Y = 69 - yp;
		SetConsoleCursorPosition(h, pos);
		cout << '*';
	}
	// ground line
	void setGround() {
		h = GetStdHandle(STD_OUTPUT_HANDLE);
		_COORD pos;
		pos.Y = 70;
		pos.X = 0;
		while (pos.X < 200) {
			SetConsoleCursorPosition(h, pos);
			cout << '/';
			pos.X++;
		}
	}
};	//end of simulation class

int main() {
	system("mode 650");			//to open in Full screen
								// declearing virables
	float mass, b, mu, e, diameter, initialVelocity, initialHight, theta, vx, vy, x, y;
	int mainChoice, inMethod, factor = 0;
	float T = 0.0;
	string parametersData;
	simulation s1;


	cout << "Welcome to Projectile simulator" << endl
		<< "Loading saved data ...." << endl;
	s1.load();			//loading saved data as a default

						// do-while to make the program working continously 
	do {
		//The Menu
		cout << "*******************************************************" << endl
			<< " Menu : " << endl
			<< " 1- Load parameters" << endl
			<< " 2- Enter parameters" << endl
			<< " 3- Save parameters " << endl
			<< " 4- Do simulation" << endl
			<< " 5- Reset data" << endl
			<< "Enter the number of your choice" << endl
			<< "-------> ";
		cin >> mainChoice;								// getting the user choice
		switch (mainChoice)								// testing the user choice
		{

		case 1:										// Loading the data from a file
			s1.load();
			break;


		case 2:										// Entering the parameters

			cout << " Please enter your parameter " << endl;
			cout << " mass of the ball: \t (Kg) " << endl << "----> ";
			cin >> mass;
			mass = s1.isNegativ(mass);
			cout << " viscous coefficient with air 'b': \t (N.s/m^2)" << endl << "----> ";
			cin >> b;
			b = s1.isNegativ(b);
			cout << " coefficient of friction of the ball with the ground 'mu' :" << endl << "----> ";
			cin >> mu;
			mu = s1.isNegativ(mu);
			cout << " coefficient of restitution with ground 'e' : " << endl << "----> ";
			cin >> e;
			e = s1.isNegativ(e);
			cout << " Diameter of the ball : \t (m)" << endl << "----> ";
			cin >> diameter;
			diameter = s1.isNegativ(diameter);
			s1.setData(mass, b, mu, e, diameter);				//setting the data to the object
			char ch;
			cout << "Do u want to save this data ?\t y/n" << endl;
			cin >> ch;
			if (ch == 'y') {
				s1.save();
				cout << "Your data is saved " << endl;
			}
			else if (ch == 'n')
			{
				cout << "Your data is not saved" << endl;
			}
			else
			{
				cout << " Please retry by chosing y /n " << endl;
			}

			break;


		case 3:										// Saving parameters
			s1.save();
			cout << "Your data is saved successfuly " << endl;
			break;


		case 4:										// Doing the Simulation
			if (s1.isSaved()) {
				cout << "You can enter the values of the speed"
					<< " and the angle or by using the arrows and space bar" << endl
					<< "1- Enter by value." << endl
					<< "2- using the arrows and the space bar" << endl
					<< "----> ";
				cin >> inMethod;
				if (inMethod == 1) {
					cout << " Please enter :" << endl
						<< " Initial velocity = \t (m/s)" << endl
						<< "----->";
					cin >> initialVelocity;
					initialVelocity = s1.isNegativ(initialVelocity);
					cout << " Initial Hight = \t (m)" << endl
						<< "----->";
					cin >> initialHight;
					cout << " Initila Angle = \t (degrees)" << endl
						<< "----->";
					cin >> theta;
					while (theta < -90 || theta > 90) {
						cout << "Please enter theta from -90 : 90 degrees ---> ";
						cin >> theta;
					}
					s1.setSimulationData(theta, initialVelocity, initialHight);
				}
				else if (inMethod == 2) {
					cout << "Use Up and Down arrows to choose the angle" << endl
						<< "Press enter to continue" << endl;
					s1.getCursor();
					s1.angle();
					cout << "Press the space bar for the velocity " << endl
						<< "Press the space bar until you see the value" << endl;
					s1.getCursor();
					s1.spaceHit();
					cout << " Initial Hight = \t (m)" << endl
						<< "----->";
					cin >> initialHight;
					s1.setHight(initialHight);
					cout << "Please dont Move the window UNTIL the simulation finish" << endl;
				}
				else {
					cout << "Please choose 1 or 2 " << endl;
					break;
				}
				cout << endl << endl << "The Simulation will start Shortly" << endl;
				sleep_for(2s);
				system("cls");
				s1.setGround();
				s1.calculate();
				cout << "Open it in matlab or excel programs to see the simulation clearly" << endl
					<< "Thanks for using our program " << endl << endl
					<< "Enter 0 to Exit " << endl;
				int exi;
				cin >> exi;
				if (exi == 0) { exit(0); }
			}
			else {
				cout << "please enter parameter first by chosing the second option" << endl;
			}
			break;


		case 5:										// Resetting the values of the parameters
			int ch2;
			cout << "Do u want to : " << endl
				<< "1- reset the whole data from the beginning ." << endl
				<< "2- reset a specific parameter" << endl
				<< "---->";
			cin >> ch2;
			if (ch == 1) {			// delet all the parameters
				s1.delet();
				cout << "Your data is deleted successfully " << endl;
			}
			else if (ch2 == 2)		// changing the parameter which the user need to change
			{
				int ch3;
				cout << " Do u want to reset :" << endl
					<< "1- mass" << endl
					<< "2- viscous coefficient with air" << endl
					<< "3- coefficient of friction of the ball with the ground " << endl
					<< "4- coefficient of restitution with ground  " << endl
					<< "5- Diameter" << endl
					<< "-----> ";
				cin >> ch3;
				switch (ch3) {
				case 1:						//Mass
					cout << "Mass= ";
					cin >> mass;
					s1.setMass(mass);
					cout << "Mass value is changed successfully" << endl;
					break;
				case 2:						//(b) viscous coefficient
					cout << "viscous coefficient with air= ";
					cin >> b;
					s1.setB(b);
					cout << " viscous coefficient with air is changed successfully" << endl;
					break;
				case 3:						//(Mu)coefficient of friction
					cout << "coefficient of friction of the ball with the ground = ";
					cin >> mu;
					s1.setMu(mu);
					cout << "coefficient of friction of the ball with the ground is changed successfully" << endl;
					break;
				case 4:						// (e) GND coefficient 
					cout << "coefficient of restitution with ground = ";
					cin >> e;
					s1.setE(e);
					cout << " coefficient of restitution with ground is changed successfully" << endl;
					break;
				case 5:						//Diameter
					cout << "Diameter of the ball = ";
					cin >> diameter;
					s1.setDiameter(diameter);
					cout << " Diameter is changed successfully " << endl;
					break;
				default:
					cout << "Please choose the right number " << endl;
					break;
				} // end of choosing which parameter to reset
			}  // end of changing a parameter else-if statment
			break;		// end of case 5

		default:
			exit(0);
		}  // end of main switch case 
	} while (mainChoice > 0 && mainChoice < 6);	// end of do-While

	system("PAUSE");
} // end of Main