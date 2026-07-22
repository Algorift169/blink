# Blink Overview

Blink is a lightweight terminal application built with GTK and VTE. It opens a custom window with a title bar, terminal pane, and a macOS-inspired look.

## What it does

- Starts an interactive shell in a PTY-backed terminal.
- Displays a framed window with rounded corners and a custom title bar.
- Supports scrolling, text selection, clipboard actions, and terminal zoom.
- Forwards terminal mouse-reporting events to interactive TUIs such as lazygit.

## Project layout

- src/app: application bootstrap.
- src/ui: window, title bar, dialogs, and settings UI.
- src/terminal: PTY session, shell startup, rendering, and scrollback handling.
- src/clipboard: clipboard access and history helpers.
- src/menu: terminal context-menu creation and zoom actions.
- src/search: text search and regex helpers.
- src/utils: filesystem, logging, process, string, and time helpers.

See [Terminal Interactions](terminal-interactions.md) for the current mouse,
clipboard, context-menu, zoom, and TUI input behavior.
