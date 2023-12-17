#include <iostream>
#include <iomanip>
#include <ctime>
#include <chrono>
#include <thread>

using namespace std;

int studyTime;
int pauseTime;
int totalTime;

bool is_number(const std::string& s)
{
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}

void inquire(){

    int tt = 0;
    int st = 0;
    int pt = 0;
    bool rec = false;

    cout << "How long would you like to study for? (Write number from 1-999 seconds (in dev mode))\n";
    cin >> tt;

    cout << "How long intervals do you want? (write number from 1-999 seconds (in dev mode))\n";
    cin >> st;

    cout << "How long breaks do you want? (write number from 1-999 seconds (in dev mode))\n";
    cin >> pt;

    // TODO check st, pt, tt for valid integers

    studyTime = st;
    pauseTime = pt;
    totalTime = tt;

    char yes_no;
    cout << "\nPlease confirm these settings (y if yes, any other otherwise) \n" 
        << "Total study time: " << totalTime << "\n"
        << "Interval time: " << studyTime << "\n"
        << "Pause time:" << pauseTime << "\n";
    cin >> yes_no;
    if(yes_no == 'y' || yes_no == 'Y'){
        return;
    }
    cout << " \n ----------------------------- \n \n";
    inquire();
}
void welcome(){

    auto start = chrono::system_clock::now();
    time_t startTime = chrono::system_clock::to_time_t(start);
    cout << "Welcome to my pomodoro app! \n";
    cout << "Current time: " << ctime(&startTime) << "\n";
    inquire();
}



void working(){
    int remWork = studyTime;
    cout << "Working \n";
    cout << "Remaining: \n";
    while (remWork > 0){
        cout << "\r" << setw(3) << setfill('0') << remWork << flush << "s";
        this_thread::sleep_for(chrono::milliseconds(75));
        remWork--;

    }
    cout << "\n";
}

void pause(){
    int remPause = pauseTime;
}

void mainLoop(){

    int remaining = totalTime; 

    cout << "\r" << "Starting Pomodoro \n";

    while (remaining >= 0){
        
        working();
        pause();
        /*
        cout << "\r" << setw(3) << setfill('0') << remaining << flush << "\b";
        this_thread::sleep_for(chrono::milliseconds(75));
        */
    }
}
int main(){

    //welcome();
    
    // dev
    totalTime = 300; 
    studyTime = 100;
    mainLoop();
    return 0;
}
