#include <iostream>
#include <vector>
#include <list>
#include <set>
#include <queue>
#include <algorithm> // for sort
using namespace std;

class patient_detail {
	// this is to enable cout print what inside the class of patient
	friend ostream& operator<<(ostream& COUT, const patient_detail& patient1);

public:
	set<long>idset;
	char gender;
	string arrival_time;
	long id;
	string type;
	list<char> genderlist = {'m', 'f'};
	list<string> typelist = {"Urgent", "Normal"};
	list<int> hourlist = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24};
	list<int> minutelist = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24,
	                        25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45,
	                        46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59
	                       };
//default constructor;
	patient_detail(): id(0), gender('m'), arrival_time("00:00"), type("N/A") {}
//parametrized constructor
	patient_detail(long id, char gender, string arrival_time, string type)
		: id(id), gender(gender), arrival_time(arrival_time), type(type) {}
//making a set to put randomly generated national ids for the patients in a sorted way and without repitition
	void randomid(int number_of_patients) {
		while (idset.size() != number_of_patients) {
			idset.insert((rand() % number_of_patients) + 30506182601697);
		}
	}
// random function that put for each patient a random national id, gender, type, arrival time
	patient_detail random(int number_of_patients) {
		id = *(idset.begin());//assuming it will be put in a for loop fo all partients in the main
		idset.erase(idset.begin());// erasing the element of the set to make the next one ready for the next patient.
		gender = getRandomElementFromList(genderlist);//call gender random maker function
		type = getRandomElementFromList(typelist);//type of condition(urgent, normal) gender random maker function
		arrival_time = gethours() + ":" + getminutes();//call arrival_time random maker functions( hours and minutes)
		return *this;
	}
// making it template to be used in our program for randomily generate gender, type by the same implementations.
	template <typename T>
	T getRandomElementFromList(const list<T>& lst) {
		vector<T> vec(lst.begin(), lst.end());
		int randomIndex = rand() % vec.size();
		return vec[randomIndex];
	}
// convert the int to string to use it in cout cases for arrival time as hours
	string gethours() {
		int hour = getRandomElementFromList(hourlist);
		return (hour < 10 ? "0" : "") + to_string(hour);
	}
// convert the int to string to use it in cout cases for arrival time as minutes
	string getminutes() {
		int minute = getRandomElementFromList(minutelist);
		return (minute < 10 ? "0" : "") + to_string(minute);
	}
// convert the string to int to use it in comparing cases of patients according to hour later
	int gethoursforpatient() const {
		string hourPart = arrival_time.substr(0, 2);
		return stoi(hourPart);
	}
// convert the string to int to use it in comparing cases of patients according to minutes later

	int getminutesforpatient() const {
		string minutePart = arrival_time.substr(3, 2);
		return stoi(minutePart);
	}
};
// implement how the cout operator in the main will work and print the object of the class
ostream& operator<<(ostream& COUT, const patient_detail& patient1) {
	COUT << "The ID is: " << patient1.id << endl;
	COUT << "Gender: " << patient1.gender << endl;
	COUT << "Arrival Time: " << patient1.arrival_time << endl;
	COUT << "Type: " << patient1.type << endl;
	return COUT;
}

void dispatchPatients(vector<patient_detail>& patients, queue<patient_detail>& urgentQueue, queue<patient_detail>& normalQueue) {
	// Sort patients by arrival time (already done in your original code)
	sort(patients.begin(), patients.end(), [](const patient_detail& a, const patient_detail& b) {
		if (a.gethoursforpatient() == b.gethoursforpatient()) {
			return a.getminutesforpatient() < b.getminutesforpatient();
		}
		return a.gethoursforpatient() < b.gethoursforpatient();
	});

	// Initialize current time
	int currentHour = 0;
	int currentMinute = 0;

	// Start dispatching patients based on time
	for (int time = 0; time < 24 * 60; ++time) {  // Simulate 24 hours (1440 minutes)
		currentMinute = time % 60;
		currentHour = time / 60;

		// Move patients whose arrival time matches the current time
		for (auto& patient : patients) {
			int patientHour = patient.gethoursforpatient();
			int patientMinute = patient.getminutesforpatient();

			// If the patient's arrival time matches the current time, move them to the appropriate queue
			if (patientHour == currentHour && patientMinute == currentMinute) {
				if (patient.type == "Urgent") {
					urgentQueue.push(patient);  // Add urgent patient to the urgent queue
				} else {
					normalQueue.push(patient);  // Add normal patient to the normal queue
				}
			}
		}
	}
}
void servingpatients(int maxServingTime, queue<patient_detail>& urgentQueue, queue<patient_detail>& normalQueue, int N) {
    int timeOfServing = 0;
    int totalPatientsServed = 0;
    int totalWaitingTime = 0; // In minutes

    while (timeOfServing < maxServingTime) {
        int servedCount = 0;  // To count how many patients are served in this minute

        // Serve urgent patients first
        while (!urgentQueue.empty() && servedCount < N) {
            patient_detail current = urgentQueue.front();
            int currentMinutes = timeOfServing;  // Current time in minutes
            int patientArrivalMinutes = current.gethoursforpatient() * 60 + current.getminutesforpatient();  // Patient arrival time in minutes

            int waitTime = currentMinutes - patientArrivalMinutes;
            totalWaitingTime += waitTime;  // Add the waiting time of the current patient

            cout << "Serving urgent patient: " << endl << current << endl;
            urgentQueue.pop();
            servedCount++;
            totalPatientsServed++;
        }

        // Serve normal patients if there are still spots
        while (!normalQueue.empty() && servedCount < N) {
            patient_detail current = normalQueue.front();
            int currentMinutes = timeOfServing;  // Current time in minutes
            int patientArrivalMinutes = current.gethoursforpatient() * 60 + current.getminutesforpatient();  // Patient arrival time in minutes

            int waitTime = currentMinutes + patientArrivalMinutes;
            totalWaitingTime += max(0, waitTime);// Avoid negative waiting time
            totalWaitingTime += waitTime;  // Add the waiting time of the current patient

            cout << "Serving normal patient: " << endl << current << endl;
            normalQueue.pop();
            servedCount++;
            totalPatientsServed++;
        }

        // Print remaining patients in both queues
        cout << "Remaining Urgent Patients: " << urgentQueue.size() << endl;
        cout << "Remaining Normal Patients: " << normalQueue.size() << endl;

        // If no more patients to serve, break the loop
        if (urgentQueue.empty() && normalQueue.empty()) {
            cout << "No more patients to serve." << endl;
            break;
        }

        // Increment the serving time (each iteration represents one minute)
        timeOfServing++;

        // Print a separator for clarity
        cout << "----------------------" << endl;
    }

    // After serving all patients, calculate the average waiting time
    if (totalPatientsServed > 0) {
        double avgWaitingTime = static_cast<double>(totalWaitingTime) / totalPatientsServed;
        cout << "Simulation Summary:" << endl;
        cout << "Total Patients Served: " << totalPatientsServed << endl;
        cout << "Average Waiting Time: " << avgWaitingTime << " minutes" << endl;
    }
}

// this is for odering the patients in the urgentqueue according to their arrival time ascending
void orderUrgentPatients(queue<patient_detail>& urgentQueue) {
	vector<patient_detail> tempPatients;

	// Move patients from queue to vector
	while (!urgentQueue.empty()) {
		tempPatients.push_back(urgentQueue.front());
		urgentQueue.pop();
	}

	// Sort patients by arrival time
	sort(tempPatients.begin(), tempPatients.end(), [](const patient_detail& a, const patient_detail& b) {
		if (a.gethoursforpatient() == b.gethoursforpatient()) {
			return a.getminutesforpatient() < b.getminutesforpatient();
		}
		return a.gethoursforpatient() < b.gethoursforpatient();
	});

	// Move sorted patients back to the queue
	for (const auto& patient : tempPatients) {
		urgentQueue.push(patient);
	}
}
// this is for odering the patients in the normalqueue according to their arrival time ascending
void orderNormalPatients(queue<patient_detail>& normalQueue) {
	vector<patient_detail> tempPatients;

	// Move patients from queue to vector
	while (!normalQueue.empty()) {
		tempPatients.push_back(normalQueue.front());
		normalQueue.pop();
	}

	// Sort patients by arrival time
	sort(tempPatients.begin(), tempPatients.end(), [](const patient_detail& a, const patient_detail& b) {
		if (a.gethoursforpatient() == b.gethoursforpatient()) {
			return a.getminutesforpatient() < b.getminutesforpatient();
		}
		return a.gethoursforpatient() < b.gethoursforpatient();
	});

	// Move sorted patients back to the queue
	for (const auto& patient : tempPatients) {
		normalQueue.push(patient);
	}
}
// this is for prinitng what inside the urgentqueue
void printUrgentQueue(queue<patient_detail> urgentQueue) {
	cout << "Printing all patients in the urgent queue:" << endl;
	while (!urgentQueue.empty()) {
		const patient_detail& patient = urgentQueue.front();
		cout << patient; // Using the overloaded operator<<
		urgentQueue.pop();
	}
	cout << "End of urgent queue." << endl;
}
// this is for prinitng what inside the normalqueue
void printNormalQueue(queue<patient_detail> normalQueue) {
	cout << "Printing all patients in the normal queue:" << endl;
	while (!normalQueue.empty()) {
		const patient_detail& patient = normalQueue.front();
		cout << patient; // Using the overloaded operator<<
		normalQueue.pop();
	}
	cout << "End of normal queue." << endl;
}
int main() {
	const int maxservingtime = 1440;
	vector<patient_detail> patients;// put all the generated patients in the vector whether ones wiht urgent or normal type of
	patient_detail p1;
	int whichscenario;
	int numberofpatients;

	cout << "choose one scenarioe of three : "<<endl;
	cout << "for simple one,100 patients, press 1"<<endl;
	cout << "for moderate one,300 patients, press 2"<<endl;
	cout << "for croweded one,700 patients, press 3"<<endl;
	// to assure that the user choose one of the scenarioes
	while(whichscenario!=1&&whichscenario!=2&&whichscenario!=3) {
		cin >> whichscenario;
		if(whichscenario==1)
			numberofpatients = 100;
		else if(whichscenario==2)
			numberofpatients = 300;
		else
			numberofpatients = 700;
	};

	// filling the set inside this object of the class with ids for a number of patients chosen.
	p1.randomid(numberofpatients);

	//  filling the vector, of type patient_detail, with the ids in the object p1.
	for (int i = 0; i < numberofpatients; i++) {
		patients.push_back(p1.random(numberofpatients));
	}

try {
        for (int i = 0; i < numberofpatients; i++) {
            if (i >= patients.size()) {
                throw out_of_range("Index " + to_string(i) + " is out of range");
            }
            cout << "Patient " << i + 1 << ": " << patients[i] << endl;
        }
    } catch (const out_of_range& e) {
        cerr << "Exception caught: " << e.what() << endl;
    }

//making two queues one for the urgent and the other for the normal
	queue<patient_detail> urgentQueue;
	queue<patient_detail> normalQueue;


//Move patients to queues based on arrival time every minute.
	dispatchPatients(patients, urgentQueue, normalQueue);

	// the patients in each queue according to their arrival time
	orderNormalPatients(normalQueue);
	orderUrgentPatients(urgentQueue);

	/*this is for printing the patients in urgentqueue to check if  splitintourgentandnormal worked well
	printUrgentQueue(urgentQueue);
	this is for printing the patients in normalqueue to check if  splitintourgentandnormal worked well
	printNormalQueue(normalQueue);*/
int numberofpatientsservedatminute;
cout << " please enter how many patients should be served per minute"<<endl;
while(numberofpatientsservedatminute<5||numberofpatientsservedatminute>10)
{
cin >> numberofpatientsservedatminute;
};
	servingpatients(maxservingtime, urgentQueue, normalQueue, numberofpatientsservedatminute);
	return 0;
}
