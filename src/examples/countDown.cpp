#include "Terminal++.hpp"

void countdown(const int seconds) {
    Printer printer;                   // Create a Terminal object for this thread
    printer.setTextColor(Color::Cyan); // Set text color to Cyan
    Screen::clear();
    Cursor::hide(); // hiding the cursor

    printer.println("Starting countdown...");
    Terminal::sleep(1000);
    Screen::clear();

    // Loop through the countdown
    for (int i = seconds; i > 0; --i) {
        Cursor::moveTo(1, 1); // Move cursor to the top-left corner
        printer.println("Countdown: ", i, " seconds remaining...");

        Printer::flush();      // Flush the output stream
        Terminal::sleep(1000); // Sleep for 1 second
    }

    Screen::clear();
    Cursor::hide();

    Cursor::moveTo(1, 1);               // Move cursor to the top-left corner
    printer.setTextColor(Color::Green); // Change color to Green for completion
    printer.println("Time's up!");      // Print completion message
}

int main() {
    Screen::clear(); // Clear the terminal screen

    Terminal terminal; // Create a Terminal object for the main thread
    Printer printer;
    printer.println("Welcome to the Countdown Timer!");
    printer.print("Enter the countdown time in seconds: ");

    int seconds;
    std::cin >> seconds; // Get user input for countdown time
    std::cin.ignore();   // Ignore the newline character after input

    // Start the countdown in a separate thread
    terminal.nonBlock([&]() {
        countdown(seconds);
    });

    // wait for the countdown to finish
    terminal.awaitCompletion();

    // Main thread can do other things or just wait
    printer.println("Press any key to exit...");

    // Wait for user input before exiting
    Input::getChar();

    Screen::clear();
    printer.println("Exiting the timer. Goodbye!");
}
