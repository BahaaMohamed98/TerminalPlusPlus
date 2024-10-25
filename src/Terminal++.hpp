#ifndef TERMINAL_HPP
#define TERMINAL_HPP

/*
         _____                   _             _
        |_   _|__ _ __ _ __ ___ (_)_ __   __ _| |  _     _
          | |/ _ \ '__| '_ ` _ \| | '_ \ / _` | |_| |_ _| |_
          | |  __/ |  | | | | | | | | | | (_| | |_   _|_   _|
          |_|\___|_|  |_| |_| |_|_|_| |_|\__,_|_| |_|   |_|

        Author: BahaaMohamed98
 */

#include <cstdlib>
#include <functional>
#include <iostream>
#include <thread>

#ifdef _WIN32

#include <conio.h>
#include <windows.h>

#else

#include <cstdio>
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>

#endif

enum class Color { // Enum for terminal text colors
    Reset, // Resets to the normal color
    Red = 31,
    Green,
    Yellow,
    Blue,
    Magenta,
    Cyan,
    White,
};

class Terminal {
    Color currentColor; // Holds the current text color
    bool boldColor; // Indicates if the current text is bold

    std::vector<std::thread> threads;

    struct TerminalSize {
        int width;
        int height;
    };

    // Converts a color and boldness state to ANSI escape codes
    static std::string ColorToString(const Color& color, const bool& isBold = false) {
        if (color == Color::Reset)
            return "\033[0m";
        return "\033[" + std::to_string(isBold) + ";" + std::to_string(static_cast<int>(color)) + "m";
    }

public:
    explicit Terminal(const Color& color = Color::Reset)
        : currentColor(color), boldColor(false) {}

    // Destructor ensures that all spawned threads are joined before the object is destroyed
    // to prevent potential crashes from detached threads running after the object is deleted
    ~Terminal() {
        awaitCompletion();
    }

    // Waits for all non-blocking tasks to finish before continuing
    // Call this after starting tasks with nonBlock()
    void awaitCompletion() {
        for (auto& thread : threads) {
            if (thread.joinable()) {
                thread.join();
            }
        }
        threads.clear(); // Clear the threads vector after joining
    }

    // returns terminal size struct of (width, height)
    static TerminalSize size() {
        TerminalSize size{0, 0};

#ifdef _WIN32
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
            size.width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
            size.height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
        }
#else
        winsize w{};
        if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == 0) {
            size.height = w.ws_row;
            size.width = w.ws_col;
        }
#endif

        return size;
    }

    // Clears the terminal screen
    static void clearScreen() {
#ifdef _WIN32 // For Windows
            system("cls");
#else // for linux and macOS
        system("clear");
#endif
    }

    // Sleeps for specified number of milliseconds
    static void sleep(const int& milliseconds) {
        std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
    }

    // Runs a given lambda function on a separate thread
    void nonBlock(const std::function<void()>& task) {
        // Adds a new thread to the threads vector for the task to be executed asynchronously
        // This ensures that the thread can be joined later when the Terminal object is destroyed
        threads.emplace_back(task);
    }

    // Moves the cursor to the specified (x, y) position in the terminal
    // starting from (1, 1) at the top left corner of the terminal
    static void moveTo(const int& x, const int& y) {
        std::cout << "\033[" << y << ";" << x << "H";
    }

    template<class T>
    void print(const T& arg) {
        std::cout << ColorToString(currentColor, boldColor) << arg;
    }

    // Prints multiple arguments to the terminal
    template<typename T, typename... Args>
    void print(const T& arg, const Args&... args) {
        print(arg);
        print(args...);
    }

    void println() {
        print("\n");
    }

    // Prints multiple arguments followed by a newline
    template<typename... Args>
    void println(const Args&... args) {
        print(args...);
        print("\n");
    }

    // Flushes the output stream
    static void flush() {
        std::cout.flush();
    }

    static void hideCursor() {
        std::cout << "\033[?25l";
    }

    static void showCursor() {
        std::cout << "\033[?25h";
    }

    // Sets the current text color and boldness
    void setColor(const Color& color, const bool& isBold = false) {
        currentColor = color;
        boldColor = isBold;
    }

    // Reads a single character from the terminal unbuffered input
    static char getChar() {
#ifdef _WIN32
        return getch();
#else
        struct termios oldattr{}, newattr{};
        tcgetattr(STDIN_FILENO, &oldattr); // Get current terminal attributes
        newattr = oldattr;
        newattr.c_lflag &= ~(ICANON | ECHO); // Disable canonical mode and echoing
        tcsetattr(STDIN_FILENO, TCSANOW, &newattr); // Apply new attributes immediately
        const int ch = getchar(); // Read a single character
        tcsetattr(STDIN_FILENO, TCSANOW, &oldattr); // Restore original attributes
        return static_cast<char>(ch);
#endif

    }

    // gets a string from stdin
    static std::string getString(const std::string& prompt = "") {
        std::cout << prompt;
        std::string str;
        std::cin >> str;
        std::cin.ignore();
        return str;
    }

    // gets a full line from stdin
    static std::string getLine(const std::string& prompt = "") {
        std::cout << prompt;
        std::string line;
        getline(std::cin, line, '\n');
        return line;
    }
};

#endif //TERMINAL_HPP
