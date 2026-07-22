# Terminal Interactions

Blink's terminal pane is backed by VTE and supports both regular terminal
selection and applications that use terminal mouse reporting, such as lazygit.

## Mouse input

- **Left click and drag**: Select terminal text using VTE's native selection
  behavior.
- **Right click**: Open the terminal context menu at the pointer position.
- **TUI mouse reporting**: When an application enables mouse reporting, VTE's
  generated mouse escape sequences are forwarded through the active PTY. This
  allows clicks, movement, and other supported mouse actions to reach programs
  such as lazygit.

Right-click is handled by Blink for the context menu. Other button events are
left available to VTE so native selection and terminal mouse reporting can
continue to work.

## Context menu

The right-click menu provides:

- **Copy**: Copy the current terminal selection as text.
- **Paste**: Paste the system clipboard into the terminal session.
- **Zoom In**: Increase the terminal font scale by `0.1`.
- **Zoom Out**: Decrease the terminal font scale by `0.1`.
- **Zoom Reset**: Restore the default terminal font scale of `0.94`.

Zoom is limited to a range of `0.5` through `2.0`.

## Keyboard clipboard shortcuts

The existing terminal shortcuts remain available:

- `Ctrl+Shift+C`: Copy the current selection.
- `Ctrl+Shift+V`: Paste the system clipboard.

## Implementation

- `src/clipboard/actions.cpp` contains the VTE clipboard actions used by both
  keyboard shortcuts and the context menu.
- `src/menu/context_menu.cpp` creates the GTK context menu and owns the zoom
  actions.
- `src/terminal/terminal.cpp` connects pointer events and the VTE `commit`
  signal. The `commit` callback forwards VTE-generated input to
  `PtySession::write`, which is required for TUI mouse reporting.