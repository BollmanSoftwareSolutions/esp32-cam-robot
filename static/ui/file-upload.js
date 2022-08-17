document.addEventListener("DOMContentLoaded", function () {
  document.getElementById("submit-upload").addEventListener("click", (e) => {
    e.preventDefault();
    console.log("event", e);
    uploadFile();
    getById("upload-form").reset();
  });
});

function getById(el) {
  return document.getElementById(el);
}

function rebootButton() {
  document.getElementById("status").innerHTML = "Invoking Reboot ...";
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/reboot", true);
  xhr.send();
  window.open("/reboot", "_self");
}

function listFilesButton() {
  xmlhttp = new XMLHttpRequest();
  xmlhttp.open("GET", "/listfiles", false);
  xmlhttp.send();

  getById("file-list-container").hidden = false;
  getById("list-header").innerHTML = "<h3>Files<h3>";
  getById("list-details").innerHTML = xmlhttp.responseText;

  getById("upload-form-container").hidden = true;
  getById("create-directory-container").hidden = true;
}

function downloadDeleteButton(filename, action) {
  var urltocall = "/file?name=" + filename + "&action=" + action;
  xmlhttp = new XMLHttpRequest();

  if (action == "delete") {
    xmlhttp.open("GET", urltocall, false);
    xmlhttp.send();
    document.getElementById("status-header").innerHTML = xmlhttp.responseText;
    xmlhttp.open("GET", "/listfiles", false);
    xmlhttp.send();
    document.getElementById("list-details").innerHTML = xmlhttp.responseText;
  }

  if (action == "download") {
    document.getElementById("status-header").innerHTML = "";
    window.open(urltocall, "_blank");
  }
}

function showUploadButton(dir) {
  getById("form-header").innerHTML = "<h3>Upload File to '" + dir + "'<h3>";
  getById("status-header").innerHTML = "";
  getById("upload-form-container").hidden = false;

  getById("dir-field").value = dir;

  getById("file-list-container").hidden = true;
  getById("create-directory-container").hidden = true;
}

function uploadFile() {
  var file = getById("file1").files[0];
  var form = getById("upload-form");
  console.log("form", form);
  // alert(file.name+" | "+file.size+" | "+file.type);

  console.log("directory value", getById("dir-field").value);

  var formdata = new FormData();
  var fileName = getById("dir-field").value + "/" + file.name;
  formdata.append("file1", file, fileName);

  console.log("formdata...");
  for (var pair of formdata.entries()) {
    console.log("naught", pair[0]);
    console.log("one", pair[1]);
  }

  var ajax = new XMLHttpRequest();
  ajax.upload.addEventListener("progress", progressHandler, false);
  ajax.addEventListener("load", completeHandler, false); // doesnt appear to ever get called even upon success
  ajax.addEventListener("error", errorHandler, false);
  ajax.addEventListener("abort", abortHandler, false);
  ajax.open("POST", "/upload");
  ajax.send(formdata);
}

function progressHandler(event) {
  //_("loaded_n_total").innerHTML = "Uploaded " + event.loaded + " bytes of " + event.total; // event.total doesnt show accurate total file size
  getById("loaded_n_total").innerHTML = "Uploaded " + event.loaded + " bytes";
  var percent = (event.loaded / event.total) * 100;
  getById("progressBar").value = Math.round(percent);
  getById("status").innerHTML = Math.round(percent) + "% uploaded... please wait";

  if (percent >= 100) {
    getById("status").innerHTML = "Please wait, writing file to filesystem";
  }
}

function completeHandler(event) {
  getById("status-header").innerHTML = "Upload Complete";
  getById("progressBar").value = 0;
  listFilesButton();
}

function errorHandler(event) {
  getById("status-header").innerHTML = "Upload Failed";
}

function abortHandler(event) {
  getById("status-header").innerHTML = "inUpload Aborted";
}

function createDirectory() {
  getById("create-directory-container").hidden = false;
  getById("file-list-container").hidden = true;
  getById("upload-form-container").hidden = true;
}
