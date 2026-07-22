(() => {
  const tabs = document.querySelectorAll('[data-install-tab]');
  const panels = document.querySelectorAll('[data-install-panel]');
  tabs.forEach((tab) => tab.addEventListener('click', () => {
    const target = tab.dataset.installTab;
    tabs.forEach((item) => { const active = item === tab; item.classList.toggle('active', active); item.setAttribute('aria-selected', String(active)); });
    panels.forEach((panel) => panel.classList.toggle('active', panel.dataset.installPanel === target));
  }));
})();
