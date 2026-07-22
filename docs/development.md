# Development Guide

## Build

```bash
make
```

## Run

```bash
./scripts/run.sh
```

## Dependencies

The project depends on GTK 3 and VTE 2.91. Install them with the helper script:

```bash
./scripts/deps.sh
```

## Interaction checks

After building Blink, verify the terminal interaction path manually:

1. Start Blink with `./scripts/run.sh`.
2. Left-click and drag across terminal text, then use `Ctrl+Shift+C` and
	`Ctrl+Shift+V` to check selection and clipboard behavior.
3. Right-click inside the terminal and check Copy, Paste, Zoom In, Zoom Out,
	and Zoom Reset.
4. Run a mouse-aware terminal application such as `lazygit` and click an item
	to verify that the application receives the mouse event.

The implementation details are described in
[Terminal Interactions](terminal-interactions.md) and
[Architecture](architecture.md).
```
