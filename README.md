
# Terminal++

Terminal++ is a simple terminal utility in C++ designed for easy text formatting and cursor manipulation in terminal applications.

## Features

- Change text color
- Move the cursor
- Clear the terminal screen
- Hide/Show the cursor

## File Structure

```
├── Terminal++.hpp
└── example.cpp
```

- **Terminal++.hpp**: Header file with terminal utility functions.
- **example.cpp**: Example usage of the Terminal++ utility.

## Usage

To use Terminal++, follow these steps:

1. Include the `Terminal++.hpp` header in your C++ file.

### Example Code Snippets

#### Clear the Screen

To clear the terminal screen:

```cpp
Terminal::clearScreen();
```

#### Move the Cursor

To move the cursor to a specific position (row 2, column 5):

```cpp
Terminal::moveTo(2, 5);
```

#### Set Text Color

To change the text color to green:

```cpp
Terminal terminal; // Create a Terminal object
terminal.setColor(Terminal::Color::Green);
```

#### Print Text

To print "Hello, Terminal++!" at the current cursor position:

```cpp
terminal.println("Hello, Terminal++!");
```

#### Move and Print More Text

To move to another position (row 4, column 5) and print a message in cyan:

```cpp
Terminal::moveTo(4, 5);
terminal.setColor(Terminal::Color::Cyan);
terminal.println("This is a simple terminal utility.");
```

#### Reset Color and Print Closing Message

To reset the color and print a closing message:

```cpp
terminal.setColor(Terminal::Color::Reset);
Terminal::moveTo(6, 5);
terminal.println("Press any key to exit...");
```

#### Wait for User Input

To wait for a key press:

```cpp
Terminal::getChar();
```

## Compiling

To compile your code with the `Terminal++` utility, use the following command:

```bash
g++ -o example example.cpp
```

## Conclusion

Terminal++ is a straightforward utility that simplifies terminal interactions in C++. Feel free to use and modify it for your terminal applications!
