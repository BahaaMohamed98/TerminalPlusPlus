# Terminal++

<div align="center">
  <img src="assets/logo.svg" alt="Terminal++ Logo" width="600" style="margin: 20px 0"/>
</div>

A lightweight, cross-platform C++ library for creating rich terminal applications with color support, cursor manipulation, and asynchronous operations. Works seamlessly on Windows and UNIX-like systems.

## Table of Contents
- [Installation](#installation)
- [Basic Usage](#basic-usage)
- [Text Styling](#text-styling)
- [Colors](#colors)
- [Terminal Operations](#terminal-operations)
- [Input Handling](#input-handling)
- [Asynchronous Operations](#asynchronous-operations)
- [Reference Charts](#reference-charts)

## Installation

Include the header file in your project:
```cpp
#include "Terminal.hpp"
```

Compile with:
```bash
g++ -std=c++17 your_file.cpp -o your_program
```

## Basic Usage

Basic printing operations:
```cpp
term.println("Hello, Terminal++!");
```

Chain multiple operations:
```cpp
term.setTextColor(Color::Blue)
    .setBackgroundColor(Color::White)
    .println("Styled text!");
```

## Text Styling

Available text styles through the `Style` enum:

- `Style::Normal` - Default text style
- `Style::Bold` - Bold weight text
- `Style::Dim` - Dimmed text intensity
- `Style::Italic` - Italic text
- `Style::Underline` - Underlined text
- `Style::Blink` - Blinking text (see animation in style reference)
- `Style::Reverse` - Reversed foreground/background colors
- `Style::Hidden` - Hidden text
- `Style::Strike` - Strikethrough text

## Colors

### Basic Colors
Available through the `Color` enum:

- `Color::Black`
- `Color::Red`
- `Color::Green`
- `Color::Yellow`
- `Color::Blue`
- `Color::Magenta`
- `Color::Cyan`
- `Color::White`
- `Color::Reset` - Resets both text and background colors to terminal defaults

### 8-bit Colors
Use any color from 0-255:
```cpp
term.setTextColor(42);        // Text color
term.setBackgroundColor(200); // Background color
```

[8-bit Color Reference Chart](#8-bit-Color-Chart)

## Terminal Operations

### Screen Control
Clear operations available through `ClearType` enum:

- `ClearType::All` - Clear screen and history
- `ClearType::Purge` - Clear visible screen only
- `ClearType::Line` - Clear current line

Example:
```cpp
Terminal::clearScreen(ClearType::All);
```

### Cursor Control

#### Move Cursor
```cpp
Terminal::moveTo(10, 5);
```
Coordinates start at (1, 1) in the top-left corner of the terminal.

#### Hide Cursor
```cpp
Terminal::hideCursor();
```

#### Show Cursor
```cpp
Terminal::showCursor();
```

### Terminal Information

#### Get Size
```cpp
auto [width, height] = Terminal::size();
```

#### Check Resize
```cpp
if (term.isResized()) {
    // Handle resize
}
```

#### Get New Dimensions
```cpp
int newWidth, newHeight;
term.isResized(newWidth, newHeight);
```

### Window Title
```cpp
Terminal::setTitle("My Terminal App");
```

### Sleep Operation
```cpp
Terminal::sleep(1000); // Sleep for 1 second
```

## Input Handling

### Supported keyboard Buttons

- **Backspace**: `keyCode::Backspace`
- **Enter**: `keyCode::Enter`
- **Escape**: `keyCode::Esc`
- **Tab**: `keyCode::Tab`
- **Space**: `keyCode::Space`

### Input Methods

#### Read Single Character
```cpp
char c = Terminal::getChar();
```
Gets a character from unbuffered input - no need to press Enter key.

#### Read String
```cpp
std::string str = Terminal::getString("Enter text: ");
```

#### Read Line
```cpp
std::string line = Terminal::getLine("Enter a line: ");
```

#### Check Key Press
```cpp
if (Terminal::keyPressed()) {
    // Handle key press
}
```

## Asynchronous Operations

Run background task:
```cpp
term.nonBlock([]() {
    term.println("Processing...")
        .setTextColor(Color::Green)
        .println("Task complete!");
    Terminal::sleep(1000);
});
```

Wait for completion (optional):
```cpp
term.awaitCompletion();
```
All background threads are automatically waited for when the Terminal instance is destroyed.

## Reference Charts

### Text Styles Animation
<div align="center">
  <img src="assets/stylesPreview.gif" alt="Styles preview" width="500"/>
</div>

### 8-bit Color Chart
<div align="center">
  <img src="assets/8-bit Color Reference Chart.png" alt="8-bit Color Chart"/>
</div>

## Platform Support

- Windows
- Linux
- macOS

## Examples

For complete working examples, see [example.cpp](src/examples/example.cpp)