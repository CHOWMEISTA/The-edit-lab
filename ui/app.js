(function(){
  new QWebChannel(qt.webChannelTransport, function(channel) {
    window.bridge = channel.objects.bridge;

    const playBtn = document.getElementById("playBtn");
    const pauseBtn = document.getElementById("pauseBtn");
    const loadBtn = document.getElementById("loadBtn");
    const fileInput = document.getElementById("file");
    const preview = document.getElementById("preview");
    const pluginsDiv = document.getElementById("plugins");
    const pluginFileInput = document.getElementById("pluginFile");
    const loadPluginBtn = document.getElementById("loadPluginBtn");

    playBtn.onclick = () => window.bridge.play();
    pauseBtn.onclick = () => window.bridge.pause();

    loadBtn.onclick = () => {
      if (fileInput.files.length === 0) return alert("Choose a media file first");
      const file = fileInput.files[0].path || fileInput.files[0].name;
      window.bridge.setSource(file);
    };

    window.bridge.pluginListUpdated.connect(function(list) {
      pluginsDiv.innerHTML = "";
      list.forEach(p => {
        const row = document.createElement("div");
        const cb = document.createElement("input");
        cb.type = "checkbox";
        cb.checked = p.enabled;
        cb.onchange = () => window.bridge.togglePlugin(p.id, cb.checked);
        const label = document.createElement("label");
        label.innerText = " " + p.name;
        row.appendChild(cb);
        row.appendChild(label);
        pluginsDiv.appendChild(row);
      });
    });

    loadPluginBtn.onclick = () => {
      if (pluginFileInput.files.length === 0) return alert("Choose a plugin file first");
      const file = pluginFileInput.files[0];
      const path = file.path || file.name;
      try {
        const ok = window.bridge.loadPlugin(path);
        if (ok) {
          alert("Plugin load requested — check plugin list.");
          const list = window.bridge.getPlugins();
          if (list) window.bridge.pluginListUpdated(list);
        } else {
          alert("Failed to load plugin (bridge returned false)");
        }
      } catch (e) {
        // some environments may not give a return value synchronously
        setTimeout(()=> {
          const list = window.bridge.getPlugins();
          if (list) window.bridge.pluginListUpdated(list);
        }, 250);
      }
    };

    window.bridge.frameReady.connect(function(base64) {
      preview.src = "data:image/png;base64," + base64;
    });

    document.getElementById("exportBtn").onclick = () => {
      // example: export 0 to 5000ms, you can extend the UI later to choose ranges
      const inputPath = fileInput.files.length ? (fileInput.files[0].path || fileInput.files[0].name) : "";
      if (!inputPath) return alert("Choose a media file first");
      window.bridge.exportRange(0, 5000, inputPath, inputPath + ".trim.mp4");
      alert("Export started (check ffmpeg output)");
    };

    // request initial plugin list
    setTimeout(()=> {
      const list = window.bridge.getPlugins();
      if (list) window.bridge.pluginListUpdated(list);
    }, 200);
  });
})();
