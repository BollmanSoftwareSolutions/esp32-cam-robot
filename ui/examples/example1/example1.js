
   function toggleCheckbox(x) {
    var xhr = new XMLHttpRequest();
    xhr.open("GET", "/action?go=" + x, true);
    xhr.send();
  }
  window.onload = document.getElementById("photo").src = window.location.href.slice(0, -1) + ":81/stream";
 