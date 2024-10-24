#include "Terminal.hpp"

int main() {
    Terminal terminal;

    // Clear the screen
    Terminal::clearScreen();

    // hides the cursor
    Terminal::hideCursor();

    // Move the cursor to a specific position
    Terminal::moveTo(5, 5);

    // Print with multiple arguments
    terminal.print("Hello, ", "World!", ' ', 123);
    terminal.println(); // Prints a newline
    Terminal::flush();

    // Print in a different color
    terminal.setColor(Terminal::Color::Red);
    terminal.println("This text is red!");
    terminal.setColor(Terminal::Color::Reset);
    terminal.println();

    // Print in a different color
    terminal.setColor(Terminal::Color::Red, true);
    terminal.println("This text is bold red!");
    terminal.setColor(Terminal::Color::Reset);
    terminal.println();

    // Get a character from the user
    terminal.println("Press any key...");
    const char c = Terminal::getChar();
    terminal.println("You pressed: ", c);

    // Get the terminal size
    auto [width, height] = terminal.size();
    terminal.println("Terminal size is: ", width, "x", height);

    //shows the cursor again
    Terminal::showCursor();
}
