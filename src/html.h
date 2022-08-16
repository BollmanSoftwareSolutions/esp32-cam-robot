#include "Arduino.h"

const char *getRobotIndexHTML()
{
  return R"rawliteral(
<!DOCTYPE html>
<html>
  <head>
    <title>ESP32-CAM Car Control</title>
    <meta name="description" />
    <meta content="control page for your ESP32-CAM based car" />
    <meta name="viewport" content="user-scalable=0" />

    <!-- <link rel="stylesheet" href="index.css" /> -->
    <style>
      body {--directional-radius: 40%;--circle-radius: 50%;--button-short: 30%;--button-tall: 80px;--forward-reverse-width: 50%;--button-active: red;height: calc(100vh - 16px);display: grid;grid-template-rows: auto 95%;margin: 0;padding: 8px;font-size: 3.5rem;text-align: center;}button {border-style: solid;border-color: gray;}button:active {background-color: rgb(112, 112, 112);color: rgb(212, 212, 212);}.content {display: grid;grid-template-rows: 40% 5% 40% 15%;grid-template-columns: 100%;justify-content: space-around;align-items: center;height: 100%;padding: 0 8px 0 8px;}.light-toggle {display: flex;justify-content: end;height: 100%;width: 100%;}.light-toggle button {height: 100%;width: 25%;font-size: 3.5rem;}.stream-container {display: flex;align-items: center;justify-content: center;min-height: 144px;height: 100%;width: 100%;margin-bottom: 16px;border: solid 1px darkgray;}.stream-container img {width: 100%;height: auto;}.nav-buttons {display: grid;grid-template: "left forward right" 240px "left stop right" 180px "left reverse right" 240px / 25% auto 25%;}.nav-buttons button {font-size: 3.5rem;}.nav-buttons .forward {grid-area: forward;height: 95%;align-self: start;border-radius: 50% 50% 10px 10px;}.nav-buttons .left {grid-area: left;margin-right: 16px;height: 70%;align-self: center;border-radius: 50% 10px 10px 50%;}.nav-buttons .stop {grid-area: stop;border-radius: 16px;align-self: center;justify-self: center;height: 90%;width: 90%;}.nav-buttons .right {grid-area: right;margin-left: 16px;height: 70%;align-self: center;border-radius: 10px 50% 50% 10px;}.nav-buttons .reverse {grid-area: reverse;height: 95%;align-self: end;border-radius: 10px 10px 50% 50%;}.settings {margin-top: 16px;width: 100%;}.settings-input {display: flex;flex-direction: row;justify-content: space-between;align-items: center;width: 100%;padding: 8px 0 8px 0;margin: 16px 0;}.settings-input input, .settings-input select {height: 100%;width: 65%;font-size: 3rem;}option {font-size: 0.75rem;}.settings-input input[type="range"] {-webkit-appearance: none;height: 35px;background-color: #2ea932af;cursor: pointer;margin: 0;}.settings-input input[type="range"]::-webkit-slider-thumb {-webkit-appearance: none;border: 1px solid rgba(0, 0, 30, 0);height: 48px;width: 22px;border-radius: 5px;background: #016f05;cursor: pointer;}
    </style>
  </head>
  <body>
    <div id="header" class="header">ESP32-CAM Car</div>
    <div class="content">
      <div class="stream-container">
        <img id="stream" src="" />
      </div>
      <div class="light-toggle"><button onclick="send('light')">Light</button></div>
      <div id="nav-buttons" class="nav-buttons">
        <button class="forward" data-action="direction" data-value="forward">Forward</button>
        <button class="left" data-action="direction" data-value="left">Left</button>
        <button class="stop" data-action="direction" data-value="stop">Stop</button>
        <button class="right" data-action="direction" data-value="right">Right</button>
        <button class="reverse" data-action="direction" data-value="reverse">Reverse</button>
      </div>
      <div class="settings">
        <div class="settings-input">
          <div>PWM:</div>
          <input id="pwm" type="range" min="0" max="255" value="192" onchange="send('pwm', this.value);" />
        </div>
        <div class="settings-input">
          <div>Video Quality:</div>
          <select id="quality" onchange="send('quality', this.value);">
            <option value="UXGA">UXGA (1600 x 1200)</option>
            <option value="SXGA">SXGA (1280 x 1024)</option>
            <option value="XGA">XGA (1024 x 768)</option>
            <option value="SVGA">SVGA (800 x 600)</option>
            <option value="VGA">VGA (640 x 480)</option>
            <option value="CIF">CIF (352 x 288)</option>
            <option value="QVGA">QVGA (320 x 240)</option>
          </select>
        </div>
      </div>
    </div>
    <script src="index.js"></script>
    <script>
      var origin=document.location.origin;document.addEventListener('DOMContentLoaded',function(){send('direction','stop');send('pwm',192);send('quality','VGA');document.querySelectorAll('#nav-buttons > button').forEach((button)=>{['mousedown','touchstart'].forEach((eventName)=>{button.addEventListener(eventName,function(){send(button.dataset['action'],button.dataset['value']);});});['mouseup','touchend'].forEach((eventName)=>{button.addEventListener(eventName,function(){send('direction','stop');});});button.addEventListener('contextmenu',(e)=>e.preventDefault());});document.getElementById('stream').src=`${origin}:81/stream`;fetch(`${origin}/info`).then((response)=>{document.getElementById('header').innerHTML=`${response}`;}).catch((e)=>console.log('error getting header info',e));});function send(action,value){var url=`${origin}/${action}${value ? '?value=' + value : ''}`;console.log('action url',url);fetch(url);}
    </script>
  </body>
</html>

  )rawliteral";
};

const char *getUploadIndexHTML()
{
  return R"rawliteral(
<!DOCTYPE HTML>
<html lang="en">
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <meta charset="UTF-8">
</head>
<body>
  <p>Main page</p>
  <p>Firmware: %FIRMWARE%</p>
  <p>Free Storage: <span id="freespiffs">%FREESPIFFS%</span> | Used Storage: <span id="usedspiffs">%USEDSPIFFS%</span> | Total Storage: <span id="totalspiffs">%TOTALSPIFFS%</span></p>
  <p>
  <button onclick="rebootButton()">Reboot</button>
  <button onclick="listFilesButton()">List Files</button>
  <button onclick="showUploadButtonFancy()">Upload File</button>
  </p>
  <p id="status"></p>
  <p id="detailsheader"></p>
  <p id="details"></p>
<script>
function rebootButton() {
  document.getElementById("status").innerHTML = "Invoking Reboot ...";
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/reboot", true);
  xhr.send();
  window.open("/reboot","_self");
}
function listFilesButton() {
  xmlhttp=new XMLHttpRequest();
  xmlhttp.open("GET", "/listfiles", false);
  xmlhttp.send();
  document.getElementById("detailsheader").innerHTML = "<h3>Files<h3>";
  document.getElementById("details").innerHTML = xmlhttp.responseText;
}
function downloadDeleteButton(filename, action) {
  var urltocall = "/file?name=" + filename + "&action=" + action;
  xmlhttp=new XMLHttpRequest();
  if (action == "delete") {
    xmlhttp.open("GET", urltocall, false);
    xmlhttp.send();
    document.getElementById("status").innerHTML = xmlhttp.responseText;
    xmlhttp.open("GET", "/listfiles", false);
    xmlhttp.send();
    document.getElementById("details").innerHTML = xmlhttp.responseText;
  }
  if (action == "download") {
    document.getElementById("status").innerHTML = "";
    window.open(urltocall,"_blank");
  }
}
function showUploadButtonFancy() {
  document.getElementById("detailsheader").innerHTML = "<h3>Upload File<h3>"
  document.getElementById("status").innerHTML = "";
  var uploadform = "<form method = \"POST\" action = \"/\" enctype=\"multipart/form-data\"><input type=\"file\" name=\"data\"/><input type=\"submit\" name=\"upload\" value=\"Upload\" title = \"Upload File\"></form>"
  document.getElementById("details").innerHTML = uploadform;
  var uploadform =
  "<form id=\"upload_form\" enctype=\"multipart/form-data\" method=\"post\">" +
  "<input type=\"file\" name=\"file1\" id=\"file1\" onchange=\"uploadFile()\"><br>" +
  "<progress id=\"progressBar\" value=\"0\" max=\"100\" style=\"width:300px;\"></progress>" +
  "<h3 id=\"status\"></h3>" +
  "<p id=\"loaded_n_total\"></p>" +
  "</form>";
  document.getElementById("details").innerHTML = uploadform;
}
function _(el) {
  return document.getElementById(el);
}
function uploadFile() {
  var file = _("file1").files[0];
  // alert(file.name+" | "+file.size+" | "+file.type);
  var formdata = new FormData();
  formdata.append("file1", file);
  var ajax = new XMLHttpRequest();
  ajax.upload.addEventListener("progress", progressHandler, false);
  ajax.addEventListener("load", completeHandler, false); // doesnt appear to ever get called even upon success
  ajax.addEventListener("error", errorHandler, false);
  ajax.addEventListener("abort", abortHandler, false);
  ajax.open("POST", "/");
  ajax.send(formdata);
}
function progressHandler(event) {
  //_("loaded_n_total").innerHTML = "Uploaded " + event.loaded + " bytes of " + event.total; // event.total doesnt show accurate total file size
  _("loaded_n_total").innerHTML = "Uploaded " + event.loaded + " bytes";
  var percent = (event.loaded / event.total) * 100;
  _("progressBar").value = Math.round(percent);
  _("status").innerHTML = Math.round(percent) + "% uploaded... please wait";
  if (percent >= 100) {
    _("status").innerHTML = "Please wait, writing file to filesystem";
  }
}
function completeHandler(event) {
  _("status").innerHTML = "Upload Complete";
  _("progressBar").value = 0;
  xmlhttp=new XMLHttpRequest();
  xmlhttp.open("GET", "/listfiles", false);
  xmlhttp.send();
  document.getElementById("status").innerHTML = "File Uploaded";
  document.getElementById("detailsheader").innerHTML = "<h3>Files<h3>";
  document.getElementById("details").innerHTML = xmlhttp.responseText;
}
function errorHandler(event) {
  _("status").innerHTML = "Upload Failed";
}
function abortHandler(event) {
  _("status").innerHTML = "inUpload Aborted";
}
</script>
</body>
</html>
)rawliteral";
}

// reboot.html base upon https://gist.github.com/Joel-James/62d98e8cb3a1b6b05102
const char *getRebootHTML()
{
  return R"rawliteral(
<!DOCTYPE HTML>
<html lang="en">
<head>
  <meta charset="UTF-8">
</head>
<body>
<h3>
  Rebooting, returning to main page in <span id="countdown">30</span> seconds
</h3>
<script type="text/javascript">
  var seconds = 20;
  function countdown() {
    seconds = seconds - 1;
    if (seconds < 0) {
      window.location = "/file-upload";
    } else {
      document.getElementById("countdown").innerHTML = seconds;
      window.setTimeout("countdown()", 1000);
    }
  }
  countdown();
</script>
</body>
</html>
)rawliteral";
}