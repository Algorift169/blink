# Architecture

Blink is organized around a small set of cooperating components.

## Main flow

1. The application entry point in src/main.cpp creates a BlinkApplication.
2. BlinkApplication initializes a GTK application and opens BlinkWindow.
3. BlinkWindow creates the custom window UI and mounts the terminal widget.
4. TerminalWidget starts a shell through PtySession and streams output into the VTE terminal.

## Key modules

- BlinkApplication: lifecycle and activation handling.
- BlinkWindow: window decoration, title bar, CSS, and terminal embedding.
- TerminalWidget: VTE terminal lifecycle and resizing.
- PtySession: PTY process management and shell I/O.
