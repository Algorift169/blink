const fs = require('fs');
const path = require('path');
const esbuild = require('esbuild');
const { minify } = require('html-minifier-terser');

const srcDir = path.resolve(__dirname, '..');
const distDir = path.join(srcDir, 'dist');

function rmrf(dir) {
  if (!fs.existsSync(dir)) return;
  for (const entry of fs.readdirSync(dir)) {
    const p = path.join(dir, entry);
    if (fs.lstatSync(p).isDirectory()) rmrf(p);
    else fs.unlinkSync(p);
  }
  fs.rmdirSync(dir);
}

function copyDir(src, dest) {
  if (!fs.existsSync(dest)) fs.mkdirSync(dest, { recursive: true });
  for (const item of fs.readdirSync(src)) {
    const s = path.join(src, item);
    const d = path.join(dest, item);
    if (fs.lstatSync(s).isDirectory()) copyDir(s, d);
    else fs.copyFileSync(s, d);
  }
}

(async () => {
  try {
    rmrf(distDir);
    fs.mkdirSync(distDir, { recursive: true });

    // Bundle and minify JS with esbuild
    const jsOut = path.join(distDir, 'js');
    fs.mkdirSync(jsOut, { recursive: true });
    esbuild.buildSync({
      entryPoints: [path.join(srcDir, 'js', 'main.js'), path.join(srcDir, 'js', 'installation.js'), path.join(srcDir, 'js', 'community.js')],
      bundle: true,
      minify: true,
      sourcemap: false,
      outdir: jsOut,
      target: ['es2017']
    });

    // Minify CSS (simple whitespace/comment removal)
    const cssSrc = path.join(srcDir, 'styles.css');
    const cssDest = path.join(distDir, 'styles.css');
    if (fs.existsSync(cssSrc)) {
      let css = fs.readFileSync(cssSrc, 'utf8');
      css = css.replace(/\/\*[^*]*\*+([^/*][^*]*\*+)*\//g, '');
      css = css.replace(/\s+/g, ' ');
      css = css.replace(/\s*([{}:;,])\s*/g, '$1');
      fs.writeFileSync(cssDest, css, 'utf8');
    }

    // Copy assets (icons, images)
    const assetsSrc = path.join(srcDir, 'assets');
    if (fs.existsSync(assetsSrc)) copyDir(assetsSrc, path.join(distDir, 'assets'));

    // Process HTML files in root of web/
    const htmlFiles = fs.readdirSync(srcDir).filter((f) => f.endsWith('.html'));
    for (const h of htmlFiles) {
      const htmlPath = path.join(srcDir, h);
      let html = fs.readFileSync(htmlPath, 'utf8');

      // Ensure we reference the bundled JS and minified CSS
      html = html.replace(/<link rel="stylesheet" href="styles.css">/g, '<link rel="stylesheet" href="styles.css">');
      html = html.replace(/<script src="js\//g, '<script src="js/');

      // Minify HTML
      const min = await minify(html, {
        collapseWhitespace: true,
        removeComments: true,
        removeRedundantAttributes: true,
        removeEmptyAttributes: true,
        minifyCSS: true,
        minifyJS: true
      });

      fs.writeFileSync(path.join(distDir, h), min, 'utf8');
    }

    console.log('Build complete. Output in', distDir);
  } catch (err) {
    console.error('Build failed:', err);
    process.exit(1);
  }
})();
