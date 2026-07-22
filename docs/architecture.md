# Architecture

Blink is organized around a small set of cooperating components.

## Main flow

1. The application entry point in src/main.cpp creates a BlinkApplication.
2. BlinkApplication initializes a GTK application and opens BlinkWindow.
3. BlinkWindow creates the custom window UI and mounts the terminal widget.
4. TerminalWidget starts a shell through PtySession and streams output into the VTE terminal.
5. TerminalWidget forwards keyboard and VTE-generated input, including mouse
	 reporting escape sequences, back through PtySession to the shell.

## Key modules

- BlinkApplication: lifecycle and activation handling.
- BlinkWindow: window decoration, title bar, CSS, and terminal embedding.
- TerminalWidget: VTE terminal lifecycle, resizing, pointer handling, and input
	forwarding.
- Clipboard actions: VTE-backed copy and paste operations shared by shortcuts
	and the context menu.
- PtySession: PTY process management and shell I/O.

## Input flow

VTE receives shell output through `vte_terminal_feed`. Keyboard input is
encoded by `TerminalWidget` and written to `PtySession`. When a terminal
application enables mouse reporting, VTE emits the corresponding input through
its `commit` signal; Blink writes that data to the same PTY so the application
can process the mouse event.
