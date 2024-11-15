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

class Color {
    std::string ansiCode; // the stored color ANSI code

public:
    // Enum for terminal text colors
    enum Code {
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

    // a class representing an RGB color
    class Rgb {
        uint8_t r;
        uint8_t g;
        uint8_t b;

    private:
        // converts an RGB color into an ANSI escape code
        [[nodiscard]] std::string toAnsi(const bool isBackground) const {
            return "\033[" +
                   std::string(isBackground ? "48" : "38") + ";2;" +
                   std::to_string(r) + ";" + std::to_string(g) + ";" + std::to_string(b) + "m";
        }

    public:
        Rgb(const uint8_t r, const uint8_t g, const uint8_t b): r(r), g(g), b(b) {}

        friend class Color;
    };

    // overloading the insertion operator
    friend std::ostream& operator <<(std::ostream& out, const Color& color) {
        return out << color.ansiCode;
    }

    friend class Printer;

private:
    // `Color::Code` to `Color`
    explicit Color(const Code& colorCode, const bool isBackground)
        : ansiCode(
            "\033[" + (colorCode == Reset ? "0" : std::to_string(isBackground ? colorCode + 10 : colorCode)) + "m"
        ) {}

    // `Rgb` to `Color`
    explicit Color(const uint8_t& ansiColor, const bool isBackground)
        : ansiCode("\033[" + std::string{isBackground ? "48" : "38"} + ";5;" + std::to_string(ansiColor) + "m") {}

    // ANSI color to `Color`
    explicit Color(const Rgb& rgbColor, const bool isBackground)
        : ansiCode(rgbColor.toAnsi(isBackground)) {}

    // clears the color
    void clear() {
        ansiCode.clear();
    }
};

class Cursor {
public:
    enum cursorStyle {
        Default,           // the default shape used by the user
        BlinkingBlock,     // a blinking block `█`
        SteadyBlock,       // a non blinking block `█`
        BlinkingUnderline, // a blinking underline `_`
        SteadyUnderline,   // a non blinking underline `_`
        BlinkingBar,       // a blinking bar `|`
        SteadyBar,         // a non blinking bar `|`
    };

    // Moves the cursor to the specified (x, y) position in the terminal
    // starting from (1, 1) in the top left corner of the terminal
    static void moveTo(const int& x, const int& y) {
        std::cout << "\033[" << y << ";" << x << "H";
    }

    // Hides the cursor
    static void hide() {
        std::cout << "\033[?25l";
    }

    // Shows the cursor
    static void show() {
        std::cout << "\033[?25h";
    }

    // Sets the cursor style to the specifed style from the `cursorStyle` enum
    static void setStyle(const cursorStyle& cursorStyle) {
        std::cout << "\033[" << static_cast<int>(cursorStyle) << " q" << std::flush;
    }
};

// Options for Screen::clear()
enum class ClearType {
    All,   //  complete clear including history
    Purge, //  clearing the visible screen while preserving history
    Line,  //  clearing just the current line
};

class Screen {
public:
    // Clears the terminal screen
    static void clear(const ClearType& cleartype = ClearType::All) {
        switch (cleartype) {
            case ClearType::All:
                std::cout << "\033[2J\033[H";
                break;
            case ClearType::Purge:
                std::cout << "\033[2J";
                break;
            case ClearType::Line:
                std::cout << "\33[2K\r";
                break;
        }

        std::cout.flush();
    }

    // enables the alternate screen buffer
    // the main screen buffer is saved and restored when switching back
    static void enableAlternateScreen() {
        std::cout << "\033[?1049h" << std::flush;
    }

    // disables the alternate screen buffer
    // switches back to the main screen
    static void disableAlternateScreen() {
        std::cout << "\033[?1049l" << std::flush;
    }
};

class TextStyle {
    std::string ansiCode;
    bool isNormal;

public:
    // Enum for text styles
    enum style {
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

    explicit TextStyle(const style& style)
        : ansiCode("\033[" + std::to_string(style) + "m"), isNormal(style == Normal) {}

    [[nodiscard]] bool isNormalStyle() const {
        return isNormal;
    }

    // insertion operator overload
    friend std::ostream& operator <<(std::ostream& out, const TextStyle& textStyle) {
        return out << textStyle.ansiCode;
    }
};

class Printer {
    Color textColor;       // current text color
    Color backgroundColor; // current background color
    Color reset;           // color reset
    TextStyle textStyle;   // current textStyle

public:
    explicit Printer(const Color::Code& textColor = Color::Reset, const Color::Code& backgroundColor = Color::Reset)
        : textColor(Color(textColor, false)),
          backgroundColor(Color(backgroundColor, true)),
          reset(Color(Color::Reset, false)),
          textStyle(TextStyle::Normal) {}

    // Prints multiple arguments to the terminal
    template<class T>
    Printer& print(const T& arg) {
        std::cout << backgroundColor;

        if (!textStyle.isNormalStyle())
            std::cout << textStyle;

        // if color is not reset, then set it as it will affect the background color
        if (textColor.ansiCode != reset.ansiCode)
            std::cout << textColor;

        std::cout << arg << reset; // reset colors again

        return *this;
    }

    // Prints multiple arguments to the terminal
    template<typename T, typename... Args>
    Printer& print(const T& arg, const Args&... args) {
        print(arg);
        print(args...);
        return *this;
    }

    Printer& println() {
        print("\n");
        return *this;
    }

    // Prints multiple arguments followed by a newline
    template<typename... Args>
    Printer& println(const Args&... args) {
        print(args...);
        print("\n");
        return *this;
    }

    // Flushes the output stream
    static void flush() {
        std::cout.flush();
    }

    // Sets the current text color
    Printer& setTextColor(const Color::Code& colorCode) {
        this->textColor = Color(colorCode, false);
        return *this;
    }

    // takes a number between 0 and 255 and sets it as text color
    Printer& setTextColor(const uint8_t& ansiColor) {
        this->textColor.ansiCode = "\033[38;5;" + std::to_string(ansiColor) + "m";
        return *this;
    }

    // Sets the text color to a given Rgb value
    Printer& setTextColor(const Color::Rgb& rgbColor) {
        this->textColor = Color(rgbColor, false);
        return *this;
    }

    // Sets the text background color to a given ColorCode
    Printer& setBackgroundColor(const Color::Code& colorCode) {
        this->backgroundColor = Color(colorCode, true);
        return *this;
    }

    // takes a number between 0 and 255 and sets it as background color
    Printer& setBackgroundColor(const uint8_t& ansiColor) {
        this->backgroundColor = Color(ansiColor, true);
        return *this;
    }

    // Sets the background color to a given Rgb value
    Printer& setBackgroundColor(const Color::Rgb& rgbColor) {
        this->backgroundColor = Color(rgbColor, true);
        return *this;
    }

    // resets text and background colors
    Printer& resetColors() {
        textColor.clear();
        backgroundColor.clear();
        return *this;
    }

    // sets the text style
    Printer& setTextStyle(const TextStyle::style& textStyle) {
        this->textStyle = TextStyle(textStyle);
        return *this;
    }
};

class Terminal {
    struct TerminalSize {
        int width;
        int height;

        bool operator!=(const TerminalSize& other) const {
            return width != other.width or height != other.height;
        }
    };

    TerminalSize dimensions;          // current terminal dimensions
    std::vector<std::thread> threads; // storing all the nonBlocking functions to join them later

public:
    Terminal(): dimensions(size()) {}

    // Destructor ensures that all spawned threads are joined before the object is destroyed
    // to prevent potential crashes from detached threads running after the object is deleted
    ~Terminal() {
        awaitCompletion();
    }

    // Runs a given lambda function on a separate thread
    Terminal& nonBlock(const std::function<void()>& task, const bool runInBackground = false) {
        if (runInBackground) { // Launches the task in a background thread that runs independently of this object
            std::thread(task).detach(); // detaching the thread
        } else {
            // Adds a new thread to the vector of threads for the task to be executed asynchronously
            // This ensures that the thread can be joined later when the Terminal object is destroyed
            threads.emplace_back(task);
        }
        return *this;
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
        CONSOLE_SCREEN_BUFFER_INFO screenBufferInfo;
        if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &screenBufferInfo)) {
            size.width = screenBufferInfo.srWindow.Right - screenBufferInfo.srWindow.Left + 1;
            size.height = screenBufferInfo.srWindow.Bottom - screenBufferInfo.srWindow.Top + 1;
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
        const auto newDimensions = size();

        const bool resized = newDimensions != dimensions;
        dimensions = newDimensions;

        return resized;
    }

    // assigns the new width and height to the given parameters by reference
    [[nodiscard]] bool isResized(int& nWidth, int& nHeight) {
        const auto newDimensions = size();
        nWidth = newDimensions.width, nHeight = newDimensions.height;

        const bool resized = newDimensions != dimensions;
        dimensions = newDimensions;

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

    // Sleeps for specified number of milliseconds
    static void sleep(const int& milliseconds) {
        std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
    }

    // sets the terminal title
    // may print unwanted text on some terminals
    static void setTitle(const std::string& title) {
        std::cout << "\033]2;" << title << "\007";
    }

    // resets all terminal attributes
    static void reset() {
        std::cout << "\033c" << std::flush;
    }
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

    // arrow keys
#ifdef _WIN32
    ArrowUp    = 72,
    ArrowDown  = 80,
    ArrowRight = 77,
    ArrowLeft  = 75,
#else
    ArrowUp = 65,
    ArrowDown,
    ArrowRight,
    ArrowLeft,
#endif
};

class Input {
public:
    // Reads a single character from the terminal's unbuffered input without any processing.
    // On Windows, uses getch() from <conio.h>.
    // On Unix-like systems, it disables canonical mode and echoing temporarily to capture the input.
    // This function is low-level; only use it if you plan to handle input processing manually.
    static char getRawChar() {
#ifdef _WIN32
        return (char)getch();
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

    // Reads a single character with arrow key support for Unix-like systems.
    //  Captures arrow keys by recognizing ESC sequences and interprets them accordingly.
    //  Uses getRawChar() internally to handle cross-platform compatibility.
    static char getChar() {
#ifdef _WIN32
        return  getRawChar();
#else
        char input = getRawChar();
        if (input == 27 and Terminal::keyPressed())
            input = getRawChar();
        if (input == '[' and Terminal::keyPressed())
            input = getRawChar();
        return input;
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
