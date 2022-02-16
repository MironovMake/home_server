function updateHallPlant() {
  var xhr = new XMLHttpRequest();
  document.getElementById("HallPlant").innerHTML ="1";
  document.getElementById("HallPlant").style.background = document.getElementById("HallPlant").style.backgroundColor=="rgb(209, 230, 245)"? "rgb(19, 65, 95)":"rgb(209, 230, 245)";
  document.getElementById("HallPlant").style.color = document.getElementById("HallPlant").style.backgroundColor=="rgb(19, 65, 95)"? "white":"rgb(19, 65, 95)";
  xhr.open("GET", "/slider?PlantAtHall=1", true);
  xhr.send();
}

function updateMyWindow() {
  var xhr = new XMLHttpRequest();
  document.getElementById("MyWindow").innerHTML ="1";
  document.getElementById("MyWindow").style.background = document.getElementById("MyWindow").style.backgroundColor=="rgb(209, 230, 245)"? "rgb(19, 65, 95)":"rgb(209, 230, 245)";
  document.getElementById("MyWindow").style.color = document.getElementById("MyWindow").style.color=="red"? "rgb(209, 230, 245)":"red";
  document.getElementById("MyWindow").style.borderBlockColor = document.getElementById("MyWindow").style.borderBlockColor=="red"? "black":"red";
  xhr.open("GET", "/slider?MyWindow=1", true);
  xhr.send();
}

function updateMoon() {
  var xhr = new XMLHttpRequest();
  document.getElementById("Moon").innerHTML ="1";
  document.getElementById("Moon").style.background = document.getElementById("Moon").style.backgroundColor=="rgb(209, 230, 245)"? "rgb(19, 65, 95)":"rgb(209, 230, 245)";
  document.getElementById("Moon").style.color = document.getElementById("Moon").style.backgroundColor=="rgb(19, 65, 95)"? "white":"rgb(19, 65, 95)";
  xhr.open("GET", "/slider?Moon=1", true);
  xhr.send();
}

function updateBath() {
  var xhr = new XMLHttpRequest();
  document.getElementById("Bath").innerHTML ="1";
  document.getElementById("Bath").style.background = document.getElementById("Bath").style.backgroundColor=="rgb(209, 230, 245)"? "rgb(19, 65, 95)":"rgb(209, 230, 245)";
  document.getElementById("Bath").style.color = document.getElementById("Bath").style.backgroundColor=="rgb(19, 65, 95)"? "white":"rgb(19, 65, 95)";
  xhr.open("GET", "/slider?Bath=1", true);
  xhr.send();
  console.log("Bath event");
}

function updateRGB() {
var hex = document.getElementById("rgb").value;
  document.getElementById("rgb").innerHTML = hex;
  
  var r=['0x' + hex[1] + hex[2]| 0];
  var g=['0x' + hex[3] + hex[4]| 0];
  var b=['0x' + hex[5] + hex[6]| 0];

  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/slider?redColor=" + r, true);
  xhr.send();

  xhr = new XMLHttpRequest();
  xhr.open("GET", "/slider?blueColor=" + b, true);
  xhr.send();

  xhr = new XMLHttpRequest();
  xhr.open("GET", "/slider?greenColor=" + g, true);
  xhr.send();
  console.log("RGB event");
}

function updateslider() {
  var sliderValue = document.getElementById("SLIDER").value;
  document.getElementById("SLIDER").innerHTML = sliderValue;
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/slider?window=" + sliderValue, true);
  xhr.send();
}