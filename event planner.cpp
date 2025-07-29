#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <ctime>
#include <cstdlib>
using namespace std;


bool isLeap(int year) {
    if((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))
    return true;
    else
    return false;
}


int checkDate(string date) 
{
	int i,year,month,day;
    if (date.length() != 10 || date[4] != '-' || date[7] != '-')
        return 1;

    for (int i=0; i<10;i++)
    {
       if (i==4 || i==7)
	   continue;
	  if (!isdigit(date[i]))
            return 1;
   }
    year = stoi(date.substr(0, 4));
    if (year < 2025 || year > 2200)  
        return 2;

    month = stoi(date.substr(5, 2));
    if (month < 1 || month > 12)
        return 3;

    day = stoi(date.substr(8, 2));
    int daysInMonth[12] = {31,28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (isLeap(year))
       daysInMonth[1]=29;
   
    if (day < 1 || day > daysInMonth[month - 1])
     return 4;

    return 0;
}


int checkTime(string time) 
{
	int i,hour,minute;
    if (time.length() != 5 || time[2] != ':')
        return 1;

    for (i=0; i<5;i++)
    {
       if (i==2)
	   continue;
	  if (!isdigit(time[i]))
            return 1;
   }
    hour = stoi(time.substr(0, 2));
    if (hour < 0 || hour > 23)
        return 2;

    minute = stoi(time.substr(3, 2));
    if (minute < 0 || minute > 59)
        return 3;

    return 0;
}

string getCurrentDate() {
    time_t t = time(0);
    tm* now = localtime(&t);
    char cdate[11];
    strftime(cdate, sizeof(cdate), "%Y-%m-%d", now);
    return string(cdate);
}

pair<int, int> getCurrentTime() {
    time_t t = time(0);
    tm* now = localtime(&t);
    return { now->tm_hour, now->tm_min };
}


bool todaydate(string date) {
    if( date >= getCurrentDate())
    return true;
    else
    return false;
}

bool todaytime(string date,string time) 
{
    if (checkTime(time) != 0) return false;

        string currentDate = getCurrentDate();
    if (date > currentDate) {
        
        return true;
    } 
    else if (date == currentDate)
    {
        
        
        auto [curH, curM] = getCurrentTime();
        int hour = stoi(time.substr(0, 2));
        int minute = stoi(time.substr(3, 2));

        if (hour > curH) return true;
        if (hour == curH && minute >= curM) return true;
        return false;
    }
    // Past date
    return false;
}


class Event {
private:
    string title, date, time, location;

public:
    void createEvent() {
    
        cout << "Enter event title: ";
        getline(cin, title);

        while (true) {
            cout << "Enter date (YYYY-MM-DD): ";
            getline(cin, date);

            int dateStatus = checkDate(date);
            if (dateStatus != 0) 
			{
                if (dateStatus == 4) 
				{
                    cout << "Invalid date format!"<<endl;
                }
                switch(dateStatus) {
                    case 1: cout << "Invalid date format!"<<endl; break;
                    case 2: cout << "Invalid year!"<<endl; break;
                    case 3: cout << "Invalid month!"<<endl; break;
                    case 4: cout << "Invalid day!"<<endl; break;
                }
                continue;
            }

            if (!todaydate(date)) {
                cout << "You cannot add past events!"<<endl;
                continue;
            }

            break;
        }

        while (true) 
		{
            cout << "Enter time (HH:MM): ";
            getline(cin, time);

            int timeStatus = checkTime(time);
            if (timeStatus != 0) {
                switch(timeStatus) {
                    case 1: cout << "Invalid time format!"<<endl; break;
                    case 2: cout << "Invalid hour!"<<endl; break;
                    case 3: cout << "Invalid minute1"<<endl; break;
                }
                continue;
            }

            if (!todaytime(date, time)) 
			{
                cout << "Time must be current or future for today's date.\n";
                continue;
            }

            break;
        }

        cout << "Enter location: ";
        getline(cin, location);
    }

    void displayEvent(int index) const {
        cout << "\nEvent #" << index + 1 << endl;
        cout << "Title    : " << title << endl;
        cout << "Date     : " << date << endl;
        cout << "Time     : " << time << endl;
        cout << "Location : " << location << endl;
    }

    void editEvent() {
        cout << "\nEditing Event...\n";
        createEvent();
    }

    string getDate() const {
        return date;
    }

    string toFileString() const {
        return title + "," + date + "," + time + "," + location + "\n";
    }

    void fromFileString(const string& line) {
        stringstream ss(line);
        getline(ss, title, ',');
        getline(ss, date, ',');
        getline(ss, time, ',');
        getline(ss, location, '\n');
    }
};


class EventManager {
private:
    vector<Event> events;
    const string filename = "events.txt";

public:
    EventManager() {
        loadFromFile();
    }

    ~EventManager() {
        saveToFile();
    }

    void addEvent() {
        Event e;
        cin.ignore();
        e.createEvent();
        events.push_back(e);
        cout << "Event added!\n";
    }

    void viewEvents() {
        if (events.empty()) {
            cout << "\nNo events to display.\n";
            return;
        }
        for (size_t i = 0; i < events.size(); ++i)
            events[i].displayEvent(i);
    }

    void editEvent() {
        if (events.empty()) {
            cout << "\nNo events to edit.\n";
            return;
        }

        viewEvents();
        int index;
        cout << "\nEnter event number to edit: ";
        cin >> index;

        if (index < 1 || index > (int)events.size()) {
            cout << "Invalid event number.\n";
            return;
        }

        cin.ignore();
        events[index - 1].editEvent();
        cout << "Event updated!\n";
    }

    void deleteEvent() {
        if (events.empty()) {
            cout << "\nNo events to delete.\n";
            return;
        }

        viewEvents();
        int index;
        cout << "\nEnter event number to delete: ";
        cin >> index;

        if (index < 1 || index > (int)events.size()) {
            cout << "Invalid event number.\n";
            return;
        }

        events.erase(events.begin() + index - 1);
        cout << "Event deleted!\n";
    }

    void searchByDate() {
        if (events.empty()) {
            cout << "\nNo events available.\n";
            return;
        }

        string searchDate;
        cout << "Enter date to search (YYYY-MM-DD): ";
        cin.ignore();
        getline(cin, searchDate);

        int dateStatus = checkDate(searchDate);
        if (dateStatus != 0) {
            if (dateStatus == 4) {
                cout << "Invalid date format.\n";
            }
            switch(dateStatus) {
                case 1: cout << "Invalid date format.\n"; break;
                case 2: cout << "Invalid year.\n"; break;
                case 3: cout << "Invalid month.\n"; break;
                case 4: cout << "Invalid day.\n"; break;
            }
            return;
        }

        bool found = false;
        for (size_t i = 0; i < events.size(); ++i) {
            if (events[i].getDate() == searchDate) {
                events[i].displayEvent(i);
                found = true;
            }
        }

        if (!found) cout << "No events found on this date.\n";
    }

    void loadFromFile() {
        ifstream inFile(filename);
        if (!inFile.is_open()) return;

        string line;
        while (getline(inFile, line)) {
            Event e;
            e.fromFileString(line);
            events.push_back(e);
        }
        inFile.close();
    }

    void saveToFile() {
        ofstream outFile(filename);
        for (const Event& e : events) {
            outFile << e.toFileString();
        }
        outFile.close();
    }
};


int main() {
    EventManager planner;
    int choice;

    do {
        system("cls");  // clear screen (Windows only)
        cout << "\n===== Event Planner Menu =====\n";
        cout << "1. Add Event\n";
        cout << "2. View All Events\n";
        cout << "3. Edit Event\n";
        cout << "4. Delete Event\n";
        cout << "5. Search Event by Date\n";
        cout << "0. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        system("cls");
        switch (choice) {
            case 1: planner.addEvent(); break;
            case 2: planner.viewEvents(); break;
            case 3: planner.editEvent(); break;
            case 4: planner.deleteEvent(); break;
            case 5: planner.searchByDate(); break;
            case 0: cout << "Goodbye!\n"; break;
            default: cout << "Invalid option.\n";
        }

        if (choice != 0) {
            cout << "\nPress Enter to continue...";
            cin.ignore();
            cin.get();
        }

    } while (choice != 0);

    return 0;
}
