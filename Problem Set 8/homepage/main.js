

let name = document.getElementById("nameInput").value;
let greeting = "Hello, " + name + "!";
let greetingWindow = window.open("", "GreetingWindow", "width=400,height=200");
let greetingElement = greetingWindow.document.createElement("p");
greetingElement.innerText = greeting;
greetingWindow.document.body.appendChild(greetingElement);
