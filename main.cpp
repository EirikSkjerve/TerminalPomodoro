#include <iostream>
#include <iomanip>
#include <ctime>
#include <cmath>
#include <chrono>
#include <thread>
#include <Windows.h>
using namespace std;

int studyTime;
int pauseTime;
int totalTime;
int totalIntervals;
int intervalNum;

// Function to minimize the terminal window (Windows-specific)
void minimizeWindow() {
    HWND hwnd = GetConsoleWindow();
    ShowWindow(hwnd, SW_MINIMIZE);
}

// Function to check if the terminal window is minimized (Windows-specific)
bool isWindowMinimized() {
    HWND hwnd = GetConsoleWindow();
    return IsIconic(hwnd) != 0;
}

// Function to check if the terminal window is in the foreground (Windows-specific)
bool isWindowInForeground() {
    return GetForegroundWindow() == GetConsoleWindow();
}

// Function to bring the terminal window to the front
void bringToFront() {
    HWND hwnd = GetConsoleWindow();
    ShowWindow(hwnd, SW_RESTORE);
    SetForegroundWindow(hwnd);
}

// function to clear one line
void clearLine() {
    cout << "\033[2K\r" << "\033[2K\r" << "\033[2K\r" << flush;
}

// function to clear the entire terminal screen
void clearScreen() {
    system("cls");
    cout << "\n \n \n \n";
}

bool is_number(const std::string& s)
{
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}

// inquires the user for timer-settings
void inquire(){

    int tt = 0;
    int st = 0;
    int pt = 0;

    cout << "How long would you like to study for? (write minutes from 1-999)\n";
    cin >> tt;

    cout << "How long intervals do you want? (write minutes from 1-999)\n";
    cin >> st;

    cout << "How long breaks do you want? (write minutes from 1 and-999)\n";
    cin >> pt;

    // TODO check st, pt, tt for valid integers

    while (!(tt > 0 && tt < 999)){
        cout << "Invalid input, try again \n";
        cin >> tt;
    } 
    totalTime = tt;

    while (!(st > 0 && st <=totalTime)){
        cout << "Invalid input, try again \n";
        cin >> st;
    }
    studyTime = st;

    while(!(pt > 0 && pt <= (tt-st))){
        cout << "Invalid input, try again \n";
        cin >> pt;
    }
    pauseTime = pt;

    totalIntervals = ceil(totalTime / (studyTime+pauseTime));

    char yes_no;
    cout << "\nPlease confirm these settings (y/n) \n" 
        << "Total study time: " << totalTime << "m \n"
        << "Interval time: " << studyTime << "m \n"
        << "Pause time: " << pauseTime << "m \n"
        << "Number of intervals: " << totalIntervals << "\n";
    cin >> yes_no;

    if(yes_no == 'y' || yes_no == 'Y'){
        return;
    }

    cout << " \n ----------------------------- \n \n";
    clearScreen();
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
    int remWork = studyTime*60;
    cout << "Remaining: \n";

    while (remWork > 0){
        // TODO dynamically change setw() to log base 10 of remaining 
        cout << "\r" << setw(5) << setfill('0') << remWork << flush << "s";
        if(!isWindowInForeground){
            bringToFront();
            minimizeWindow();
        }
        this_thread::sleep_for(chrono::milliseconds(100));
        remWork--;

    }

    clearScreen();
    bringToFront();
}

void pause(){
    int remPause = pauseTime * 60;

    cout << "Remaining: \n";
    while (remPause > 0){
        // TODO dynamically change setw() to log base 10 of remaining 
        cout << "\r" << setw(5) << setfill('0') << remPause << flush << "s";
        this_thread::sleep_for(chrono::milliseconds(100));
        remPause--;

        if(!isWindowInForeground){

            bringToFront();
            minimizeWindow();
        }
    }

    clearScreen();
    bringToFront();
}

void mainLoop(){

    int remaining = totalTime*60; 
    intervalNum = 1;

    char start;
    cout << "\n" << "Press s to start pomodoro\n";
    cin >> start;

    if (start != 's' && start != 'S'){
        return;
    }

    clearScreen();

    while (remaining >= 0 && intervalNum <= totalIntervals){
        
        cout << "Working, interval " << intervalNum << "/" << totalIntervals << ". To get popup on pause, minimize this window. \n" << "Happy working! \n";
        working();
        remaining -= studyTime*60;
        
        
        cout << "Pause, interval "  << intervalNum << "/" << totalIntervals << ". To get popup on start again, minimize this window. \n" << "Enjoy your break! \n";
        pause();
        remaining -= pauseTime*60;
        intervalNum++;
    }
}
int main(){

    // prints welcome message and inquires for timer-settings
    welcome();
    
    // starts the main loop after retrieving timer-settings
    mainLoop();
    cout << "\nWell done!";
    return 0;
}
