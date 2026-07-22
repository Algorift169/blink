(() => {
  const toggle = document.querySelector('.menu-toggle');
  const nav = document.querySelector('.site-nav');
  if (toggle && nav) toggle.addEventListener('click', () => {
    const open = nav.classList.toggle('open');
    toggle.setAttribute('aria-expanded', String(open));
  });
  document.querySelectorAll('[data-copy]').forEach((button) => {
    button.addEventListener('click', async () => {
      try { await navigator.clipboard.writeText(button.dataset.copy); } catch { return; }
      const original = button.textContent;
      button.textContent = 'Copied';
      setTimeout(() => { button.textContent = original; }, 1400);
    });
  });
  const gallery = document.querySelector('.preview-gallery');
  if (gallery) {
    const mainImage = gallery.querySelector('#gallery-main');
    const thumbs = gallery.querySelectorAll('.thumb');
    thumbs.forEach((thumb) => {
      thumb.addEventListener('click', () => {
        const src = thumb.dataset.src;
        if (!src || mainImage.src.endsWith(src)) return;
        thumbs.forEach((item) => item.classList.toggle('active', item === thumb));
        mainImage.classList.add('fade-out');
        const swap = () => {
          mainImage.removeEventListener('transitionend', swap);
          mainImage.src = src;
          requestAnimationFrame(() => mainImage.classList.remove('fade-out'));
        };
        mainImage.addEventListener('transitionend', swap);
      });
    });
  }
})();
