#include "Terminal++.hpp"

int main() {
    // Create a Terminal object to work with
    Terminal terminal;

    // Clear the terminal screen
    Terminal::clearScreen();

    // Get and display the terminal size
    auto [width, height] = terminal.size();
    terminal.println("Terminal Size: ", width, " x ", height);

    // Set the text color to green and print a message
    terminal.setColor(Terminal::Color::Green);
    terminal.println("Welcome to Terminal++!");

    // Set color to blue and print another message
    terminal.setColor(Terminal::Color::Blue);
    terminal.println("This library supports various colors.");

    // Set color to yellow and bold
    terminal.setColor(Terminal::Color::Yellow, true);
    terminal.println("You can also print bold text!");

    // Reset to normal color
    terminal.setColor(Terminal::Color::Reset);

    // Print a message indicating how to use the library
    terminal.println("Use Terminal::clearScreen() to clear the screen.");
    terminal.println("Use Terminal::getChar() to read a character.");

    // Move the cursor to a specific position (10, 5) and print a message
    Terminal::moveTo(10, 5);
    terminal.println("Cursor moved to (10, 5)!");

    // Hide the cursor
    Terminal::hideCursor();
    terminal.println("The cursor is now hidden.");

    // Show the cursor again
    Terminal::showCursor();
    terminal.println("The cursor is now visible again.");

    // Finish by flushing the output
    Terminal::flush();

    return 0;
}
