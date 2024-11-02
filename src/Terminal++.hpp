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

// Enum for terminal text colors
enum class Color {
    Black = 30,
    Red,
    Green,
    Yellow,
    Blue,
    Magenta,
    Cyan,
    White,
    Reset = 0, // Resets to the normal color
};

// Enum for text styles
enum class Style {
    Normal,
    Bold,
    Dim,
    Italic,
    Underline,
    Blink,
    Reverse,
    Hidden,
    Strike,
};

// Enum for keyboard buttons' keyCodes
enum keyCode {
#ifdef _WIN32
    Backspace = 8,
    Enter     = 13,
#else
    Backspace = 127,
    Enter     = 10,
#endif
    Esc   = 27,
    Tab   = 9,
    Space = 32,
};

// Options for clearScreen()
// "All" for a complete clear including history
// "Purge" for clearing the visible screen while preserving scrollback
// "Line" for clearing just the current line
enum ClearType {
    All,   // clear all plus history
    Purge, // clear the screen leaving history
    Line,  // clear the current line
};

class Terminal {
    std::string textColor;       // Holds the current text color
    std::string backgroundColor; // Holds the current background color
    Style style;

    struct TerminalSize {
        int width;
        int height;

        bool operator!=(const TerminalSize& other) const {
            return width != other.width or height != other.height;
        }
    };

    TerminalSize dimensions;
    std::vector<std::thread> threads;

    // Converts a number into an ANSI escape code
    static std::string ansiToString(const int& code) {
        return "\033[" + std::to_string(code) + "m";
    }

    // Converts a Color into a ANSI escape code for background
    static std::string backgroundColorToString(const Color& color) {
        if (color == Color::Reset)
            return ansiToString(static_cast<int>(Color::Reset));
        return ansiToString(static_cast<int>(color) + 10);
    }

public:
    explicit Terminal(const Color& textColor = Color::Reset, const Color& backgroundColor = Color::Reset)
        : textColor(ansiToString(static_cast<int>(textColor))),
          backgroundColor(backgroundColorToString(backgroundColor)),
          style(Style::Normal), dimensions(size()) {}

    // Destructor ensures that all spawned threads are joined before the object is destroyed
    // to prevent potential crashes from detached threads running after the object is deleted
    ~Terminal() {
        awaitCompletion();
    }

    // Waits for all non-blocking tasks to finish before continuing
    // Call this after starting tasks with nonBlock()
    Terminal& awaitCompletion() {
        for (auto& thread : threads) {
            if (thread.joinable()) {
                thread.join();
            }
        }
        threads.clear(); // Clear the threads vector after joining
        return *this;
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

    // returns true if the terminal was resized
    // should be called from within a loop
    [[nodiscard]] bool isResized() {
        const auto ndimensions = size();

        const bool resized = ndimensions != dimensions;
        dimensions = ndimensions;

        return resized;
    }

    // assigns the new widht and height to the given paramters by refrence
    [[nodiscard]] bool isResized(int& nWidth, int& nHeight) {
        const auto ndimensions = size();
        nWidth = ndimensions.width, nHeight = ndimensions.height;

        const bool resized = ndimensions != dimensions;
        dimensions = ndimensions;

        return resized;
    }

    // returns true if the terminal was resized
    // should be called using nonBlock as it blocks the thread
    // should be called from within a loop
    [[nodiscard]] static bool keyPressed() {
#ifdef _WIN32
        return _kbhit();  // Windows
#else
        termios oldt{}, newt{};
        int bytesAvailable = 0;
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        ioctl(STDIN_FILENO, FIONREAD, &bytesAvailable);
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        return bytesAvailable > 0; // Linux/macOS
#endif
    }

    // Clears the terminal screen
    static void clearScreen(const ClearType& cleartype = ClearType::All) {
        switch (cleartype) {
            case All:
#ifdef _WIN32 // For Windows
                system("cls");
#else // for linux and macOS
                system("clear");
#endif
                break;
            case Purge:
                std::cout << "\033[2J" << std::flush;
                break;
            case Line:
                std::cout << "\33[2K\r" << std::flush;
                break;
        }
    }

    // Sleeps for specified number of milliseconds
    static void sleep(const int& milliseconds) {
        std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
    }

    // Runs a given lambda function on a separate thread
    Terminal& nonBlock(const std::function<void()>& task) {
        // Adds a new thread to the threads vector for the task to be executed asynchronously
        // This ensures that the thread can be joined later when the Terminal object is destroyed
        threads.emplace_back(task);
        return *this;
    }

    // Moves the cursor to the specified (x, y) position in the terminal
    // starting from (1, 1) at the top left corner of the terminal
    static void moveTo(const int& x, const int& y) {
        std::cout << "\033[" << y << ";" << x << "H";
    }

    // Prints multiple arguments to the terminal
    template<class T>
    Terminal& print(const T& arg) {
        std::cout << backgroundColor;

        if (style != Style::Normal)
            std::cout << ansiToString(static_cast<int>(style));

        if (textColor != "\033[0m") // if color is not reset then set it as it will affect the background color
            std::cout << textColor;

        std::cout << arg
                << ansiToString(static_cast<int>(Color::Reset)); // reset colors again

        return *this;
    }

    template<typename T, typename... Args>
    Terminal& print(const T& arg, const Args&... args) {
        print(arg);
        print(args...);
        return *this;
    }

    Terminal& println() {
        print("\n");
        return *this;
    }

    // Prints multiple arguments followed by a newline
    template<typename... Args>
    Terminal& println(const Args&... args) {
        print(args...);
        print("\n");
        return *this;
    }

    // Flushes the output stream
    static void flush() {
        std::cout.flush();
    }

    //Hides the cursor
    static void hideCursor() {
        std::cout << "\033[?25l";
    }

    // Shows cursor
    static void showCursor() {
        std::cout << "\033[?25h";
    }

    // Sets the current text color and boldness
    Terminal& setTextColor(const Color& textColor) {
        this->textColor = ansiToString(static_cast<int>(textColor));
        return *this;
    }

    // takes a number between 0 and 255 and sets it as text color
    Terminal& setTextColor(const int& textColor) {
        this->textColor = "\033[38;5;" + std::to_string(textColor) + "m";
        return *this;
    }

    Terminal& setBackgroundColor(const Color& backgroundColor) {
        this->backgroundColor = backgroundColorToString(backgroundColor);
        return *this;
    }

    // takes a number between 0 and 255 and sets it as background color
    Terminal& setBackgroundColor(const int& backgroundColor) {
        this->backgroundColor = "\033[48;5;" + std::to_string(backgroundColor) + "m";
        return *this;
    }

    // sets the text style
    Terminal& setStyle(const Style& style) {
        this->style = style;
        return *this;
    }

    // sets the terminal title
    // may print unwanted text on some terminals
    static void setTitle(const std::string& title) {
        std::cout << "\033]2;" << title << "\007";
    }

    // Reads a single character from the terminal unbuffered input
    static char getChar() {
#ifdef _WIN32
        return getch();
#else
        termios oldattr{}, newattr{};
        tcgetattr(STDIN_FILENO, &oldattr); // Get current terminal attributes
        newattr = oldattr;
        newattr.c_lflag &= ~(ICANON | ECHO);        // Disable canonical mode and echoing
        tcsetattr(STDIN_FILENO, TCSANOW, &newattr); // Apply new attributes immediately
        const int ch = getchar();                   // Read a single character
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
