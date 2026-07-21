# Blink Overview

Blink is a lightweight terminal application built with GTK and VTE. It opens a custom window with a title bar, terminal pane, and a macOS-inspired look.

## What it does

- Starts an interactive shell in a PTY-backed terminal.
- Displays a framed window with rounded corners and a custom title bar.
- Supports scrolling and basic terminal interactions.

## Project layout

- src/app: application bootstrap.
- src/ui: window, title bar, dialogs, and settings UI.
- src/terminal: PTY session, shell startup, rendering, and scrollback handling.
- src/clipboard: clipboard access and history helpers.
- src/search: text search and regex helpers.
- src/utils: filesystem, logging, process, string, and time helpers.
