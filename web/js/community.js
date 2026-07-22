(() => {
  const form = document.querySelector('#comment-form');
  const list = document.querySelector('#comments-list');
  const count = document.querySelector('#comment-count');
  if (!form || !list) return;
  const stored = JSON.parse(localStorage.getItem('blink-comments') || '[]');
  const render = (comment) => {
    const initials = comment.name.trim().split(/\s+/).map((part) => part[0]).join('').slice(0, 2).toUpperCase();
    const card = document.createElement('article'); card.className = 'comment-card';
    card.innerHTML = `<div class="avatar avatar-cyan">${initials}</div><div><div class="comment-top"><strong>${comment.name.replace(/[<>]/g, '')}</strong><time>just now</time></div><p>${comment.message.replace(/[<>]/g, '')}</p></div>`;
    list.prepend(card);
  };
  stored.forEach(render);
  count.textContent = `${String(stored.length).padStart(2, '0')} notes`;
  form.addEventListener('submit', (event) => {
    event.preventDefault();
    const data = { name: form.elements.name.value.trim(), message: form.elements.message.value.trim() };
    if (!data.name || !data.message) return;
    stored.push(data); localStorage.setItem('blink-comments', JSON.stringify(stored)); render(data);
    count.textContent = `${String(stored.length).padStart(2, '0')} notes`; form.reset();
  });
})();
