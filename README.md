<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8" />
  <meta name="viewport" content="width=device-width, initial-scale=1.0"/>
  <title>Todo List</title>
  <style>
    * { margin: 0; padding: 0; box-sizing: border-box; }

    body {
      font-family: 'Segoe UI', sans-serif;
      background: #f0f2f5;
      display: flex;
      justify-content: center;
      padding: 50px 20px;
      min-height: 100vh;
    }

    .container {
      background: white;
      border-radius: 12px;
      box-shadow: 0 4px 20px rgba(0,0,0,0.1);
      padding: 30px;
      width: 100%;
      max-width: 480px;
      height: fit-content;
    }

    h1 {
      text-align: center;
      color: #333;
      margin-bottom: 24px;
      font-size: 1.8rem;
    }

    .input-area {
      display: flex;
      gap: 10px;
      margin-bottom: 20px;
    }

    input[type="text"] {
      flex: 1;
      padding: 10px 14px;
      border: 2px solid #ddd;
      border-radius: 8px;
      font-size: 1rem;
      outline: none;
      transition: border 0.2s;
    }

    input[type="text"]:focus { border-color: #6c63ff; }

    button.add-btn {
      padding: 10px 18px;
      background: #6c63ff;
      color: white;
      border: none;
      border-radius: 8px;
      font-size: 1rem;
      cursor: pointer;
      transition: background 0.2s;
    }

    button.add-btn:hover { background: #574fd6; }

    ul { list-style: none; }

    li {
      display: flex;
      align-items: center;
      justify-content: space-between;
      padding: 12px 14px;
      margin-bottom: 10px;
      background: #f9f9ff;
      border-radius: 8px;
      border-left: 4px solid #6c63ff;
      transition: opacity 0.2s;
    }

    li.done { opacity: 0.5; text-decoration: line-through; }

    li span { cursor: pointer; font-size: 0.95rem; color: #333; flex: 1; }

    button.del-btn {
      background: none;
      border: none;
      color: #e74c3c;
      font-size: 1.1rem;
      cursor: pointer;
      padding: 0 4px;
    }

    .empty {
      text-align: center;
      color: #aaa;
      font-size: 0.9rem;
      margin-top: 10px;
    }
  </style>
</head>
<body>
  <div class="container">
    <h1>📝 Todo List</h1>
    <div class="input-area">
      <input type="text" id="taskInput" placeholder="Add a new task..." />
      <button class="add-btn" onclick="addTask()">Add</button>
    </div>
    <ul id="taskList"></ul>
    <p class="empty" id="emptyMsg">No tasks yet. Add one above!</p>
  </div>

  <script>
    const input = document.getElementById('taskInput');
    const list  = document.getElementById('taskList');
    const empty = document.getElementById('emptyMsg');

    input.addEventListener('keydown', e => { if (e.key === 'Enter') addTask(); });

    function addTask() {
      const text = input.value.trim();
      if (!text) return;

      const li = document.createElement('li');
      li.innerHTML = `<span onclick="toggle(this)">${text}</span>
                      <button class="del-btn" onclick="remove(this)">✕</button>`;
      list.appendChild(li);
      input.value = '';
      updateEmpty();
    }

    function toggle(span) { span.parentElement.classList.toggle('done'); }
    function remove(btn)  { btn.parentElement.remove(); updateEmpty(); }
    function updateEmpty(){ empty.style.display = list.children.length ? 'none' : 'block'; }
  </script>
</body>
</html>
