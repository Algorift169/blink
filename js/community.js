(() => {
  const form = document.querySelector('#comment-form');
  const list = document.querySelector('#comments-list');
  const count = document.querySelector('#comment-count');
  if (!form || !list) return;

  // Hardcoded comments array in the codebase
  const defaultComments = [
    { name: "Sheikh Muhammad Israfil", message: "Blink is incredibly fast! I love the GTK integration." },
    { name: "Alex Rivera", message: "Finally, a lightweight C++ terminal emulator that doesn't hog memory. Highly recommended." },
    { name: "Elena Rostova", message: "The custom decorations look so sleek. Great work on the design!" }
  ];

  // Load custom comments from localStorage
  const stored = JSON.parse(localStorage.getItem('blink-comments') || '[]');

  // Helper to render a comment
  const render = (comment) => {
    const initials = comment.name.trim().split(/\s+/).map((part) => part[0]).join('').slice(0, 2).toUpperCase();
    const card = document.createElement('article');
    card.className = 'comment-card';
    card.innerHTML = `<div class="avatar avatar-cyan">${initials}</div><div><div class="comment-top"><strong>${comment.name.replace(/[<>]/g, '')}</strong><time>just now</time></div><p>${comment.message.replace(/[<>]/g, '')}</p></div>`;
    list.prepend(card);
  };

  // Render both default and custom comments
  const allComments = [...defaultComments, ...stored];
  allComments.forEach(render);
  count.textContent = `${String(allComments.length).padStart(2, '0')} notes`;

  // Handle new comment submission
  form.addEventListener('submit', (event) => {
    event.preventDefault();
    const data = {
      name: form.elements.name.value.trim(),
      message: form.elements.message.value.trim()
    };
    if (!data.name || !data.message) return;

    // Save to custom comments in localStorage
    stored.push(data);
    localStorage.setItem('blink-comments', JSON.stringify(stored));

    // Render new comment
    render(data);

    // Update count
    const totalCount = defaultComments.length + stored.length;
    count.textContent = `${String(totalCount).padStart(2, '0')} notes`;
    form.reset();
  });
})();
