# Blink web

Static project site for Blink Terminal Emulator.

## Run locally

From this directory:

```bash
python3 -m http.server 8080
```

Then visit `http://localhost:8080`.

The site uses one shared stylesheet and vanilla JavaScript. Installation tabs and copy buttons run in the browser; community notes are stored locally with `localStorage` and are not sent to a server.

## Build & Publish (recommended)

- Run `npm run build` to produce a minified production site in the `dist/` folder. The build bundles and minifies JavaScript, minifies CSS/HTML, and copies assets.
- Publish only the contents of `dist/` to your static hosting provider (GitHub Pages, Netlify, Vercel, etc.). Do not publish the `web/` source folder — it contains human-readable sources and dev dependencies.
- `package-lock.json` is included to lock dev tooling versions used for reproducible builds.

To preview the production output locally:

```bash
cd web/dist
python3 -m http.server 8080
# then open http://localhost:8080
```

This ensures users see only the minified production assets and not development source files.
