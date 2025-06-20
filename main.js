const enpoint = "http://192.168.22.15"; 

function SetDapurLed() {
    fetch(enpoint + "/led", {
        method: "GET"
    }).then(response => response.text()).then(result => {
        if(result === "ON") {
            ledDapur.style.backgroundColor = "red";
            dapurLedImage.src = "./assets/led-on.png";
        } else {
            ledDapur.style.backgroundColor = "blue";
            dapurLedImage.src = "./assets/led-off.png";
        }
    });
}

function SetDapurLed() {
    fetch(enpoint + "/led", {
        method: "POST"
    })
    .then(response => response.text())
    .then(() => {
        location.reload();
        ledDapur.style.backgroundColor = "red";
        dapurLedImage.src = "./assets/led-on.png";
        alert("Nyala beroo!!!   ");
    })
    .catch(error => {
        alert("Gagal menghubungi ESP32: " + error);
        console.error(error);
    });
}