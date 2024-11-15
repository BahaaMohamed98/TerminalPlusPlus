#include "Terminal++.hpp"

int main() {
    Printer printer;
    // Print a simple message
    printer.println("Hello, Terminal++!");

    // Apply text styling (blue text, white background, bold)
    printer.setTextColor(Color::Blue)
           .setBackgroundColor(Color::White)
           .setTextStyle(TextStyle::Bold)
           .println("Styled text!");

    // Cursor control examples
    Cursor::moveTo(10, 5);                                // Move the cursor to (10, 5)
    Cursor::hide();                                       // Hide the cursor
    Cursor::show();                                       // Show the cursor
    Cursor::setStyle(Cursor::cursorStyle::BlinkingBlock); // Set cursor to blinking block style

    // Screen control examples
    Screen::clear(ClearType::All);    // Clear the entire screen including scrollback history
    Screen::enableAlternateScreen();  // Switch to the alternate screen buffer
    Screen::disableAlternateScreen(); // Switch back to the main screen buffer

    // Input handling examples
    char c = Input::getChar();                           // Read a single character from input
    std::string str = Input::getString("Enter text: ");  // Read a string with a prompt
    std::string line = Input::getLine("Enter a line: "); // Read a full line with a prompt
    if (Terminal::keyPressed()) {
        // Handle key press
    }

    // Asynchronous operations example
    Terminal term;
    term.nonBlock([]() {
        Printer().println("Processing...")
                 .setTextColor(Color::Green)
                 .println("Task complete!");
        Terminal::sleep(1000);
    });
    term.awaitCompletion(); // Wait for all background tasks to complete

    // Terminal information examples
    auto [width, height] = Terminal::size(); // Get the current terminal size
    if (term.isResized()) {
        // Handle resize
    }

    Terminal::setTitle("My Terminal App"); // Set the window title

    // Text styles examples
    printer.setTextStyle(TextStyle::Normal)
           .println("Normal text");
    printer.setTextStyle(TextStyle::Bold)
           .println("Bold text");
    printer.setTextStyle(TextStyle::Italic)
           .println("Italic text");

    // Basic color examples
    printer.setTextColor(Color::Red)
           .println("Red text");
    printer.setTextColor(Color::Green)
           .println("Green text");
    printer.setTextColor(Color::Blue)
           .println("Blue text");
    printer.setTextColor(Color::Reset)
           .println("Reset color");

    // 8-bit color examples
    printer.setTextColor(42)
           .println("Custom text color");
    printer.setBackgroundColor(200)
           .println("Custom background color");

    // RGB colors
    printer.setTextColor(Color::Rgb(255, 0, 0)) // red color
           .println("Red text (RGB)");

    Color::Rgb purpleColor(94, 60, 108);
    printer.setBackgroundColor(purpleColor)
           .setTextColor(Color::White)
           .println("Purple background (RGB)");

}
