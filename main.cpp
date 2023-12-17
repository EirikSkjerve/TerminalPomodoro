#include <iostream>
#include <iomanip>
#include <ctime>
#include <chrono>
#include <thread>
#include <Windows.h>
using namespace std;

int studyTime;
int pauseTime;
int totalTime;

// Function to minimize the terminal window (Windows-specific)
void minimizeWindow() {
#ifdef _WIN32
    HWND hwnd = GetConsoleWindow();
    ShowWindow(hwnd, SW_MINIMIZE);
#endif
}

// Function to check if the terminal window is minimized (Windows-specific)
bool isWindowMinimized() {
#ifdef _WIN32
    HWND hwnd = GetConsoleWindow();
    return IsIconic(hwnd) != 0;
#else
    return false; // Placeholder for non-Windows systems
#endif
}

// Function to check if the terminal window is in the foreground (Windows-specific)
bool isWindowInForeground() {
#ifdef _WIN32
    return GetForegroundWindow() == GetConsoleWindow();
#else
    return true; // Placeholder for non-Windows systems
#endif
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
    cout << "\nPlease confirm these settings (y/n) \n" 
        << "Total study time: " << totalTime << "\n"
        << "Interval time: " << studyTime << "\n"
        << "Pause time: " << pauseTime << "\n";
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
    int remWork = studyTime;
    cout << "Remaining: \n";

    while (remWork > 0){
        cout << "\r" << setw(3) << setfill('0') << remWork << flush << "s";
        this_thread::sleep_for(chrono::milliseconds(1000));
        remWork--;
        if(!isWindowInForeground && !isWindowMinimized){
            minimizeWindow();
        }
    }

    clearScreen();
    bringToFront();
}

void pause(){
    int remPause = pauseTime;

    cout << "Remaining: \n";
    while (remPause > 0){
        cout << "\r" << setw(3) << setfill('0') << remPause << flush << "s";
        this_thread::sleep_for(chrono::milliseconds(1000));
        remPause--;

        if(!isWindowInForeground && !isWindowMinimized){
            minimizeWindow();
        }
    }

    clearScreen();
    bringToFront();
}

void mainLoop(){

    int remaining = totalTime; 
    char start;
    cout << "\r" << "Press s to start pomodoro\n";
    cin >> start;

    if (start != 's' && start != 'S'){
        return;
    }

    clearScreen();

    while (remaining >= 0){
        
        cout << "Working \n";
        working();
        remaining -= studyTime;
        
        
        cout << "Pause \n";
        pause();
        remaining -= pauseTime;
    }
}
int main(){

    // prints welcome message and inquires for timer-settings
    welcome();
    
    // starts the main loop after retrieving timer-settings
    mainLoop();
    return 0;
}
