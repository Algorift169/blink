const { promises: fs } = require('fs');
const path = require('path');
const esbuild = require('esbuild');
const { minify } = require('html-minifier-terser');

const root = __dirname;
const dist = path.join(root, 'dist');
const pages = ['index.html', 'updates.html', 'installation.html', 'community.html'];

const htmlMinifyOptions = {
  collapseWhitespace: true,
  removeComments: true,
  removeRedundantAttributes: true,
  removeOptionalTags: true,
  minifyCSS: true,
  minifyJS: true,
  useShortDoctype: true,
  keepClosingSlash: true,
};

async function ensureDir(dir) {
  await fs.mkdir(dir, { recursive: true });
}

async function copyDir(src, dest) {
  const entries = await fs.readdir(src, { withFileTypes: true });
  await ensureDir(dest);
  for (const entry of entries) {
    const srcPath = path.join(src, entry.name);
    const destPath = path.join(dest, entry.name);
    if (entry.isDirectory()) {
      await copyDir(srcPath, destPath);
    } else {
      await fs.copyFile(srcPath, destPath);
    }
  }
}

async function build() {
  await fs.rm(dist, { recursive: true, force: true });
  await ensureDir(dist);

  await esbuild.build({
    entryPoints: [path.join(root, 'js', 'main.js'), path.join(root, 'js', 'installation.js')],
    bundle: true,
    minify: true,
    sourcemap: false,
    format: 'iife',
    outfile: path.join(dist, 'js', 'app.js'),
    legalComments: 'none',
  });

  await esbuild.build({
    entryPoints: [path.join(root, 'styles.css')],
    bundle: true,
    minify: true,
    sourcemap: false,
    outfile: path.join(dist, 'styles.css'),
    legalComments: 'none',
  });

  await copyDir(path.join(root, 'assets'), path.join(dist, 'assets'));

  for (const page of pages) {
    const htmlPath = path.join(root, page);
    const html = await fs.readFile(htmlPath, 'utf8');
    const updated = html
      .replace(/<script src="js\/main\.js"><\/script>/g, '<script src="js/app.js"></script>')
      .replace(/<script src="js\/installation\.js"><\/script>/g, '');
    const minified = await minify(updated, htmlMinifyOptions);
    await fs.writeFile(path.join(dist, page), minified, 'utf8');
  }

  console.log('Production build complete. Deploy the contents of dist/.');
}

build().catch((error) => {
  console.error(error);
  process.exit(1);
});
