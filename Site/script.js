 import { initializeApp } from "https://www.gstatic.com/firebasejs/9.10.0/firebase-app.js";
        // TODO: Add SDKs for Firebase products that you want to use
        // https://firebase.google.com/docs/web/setup#available-libraries

        // Your web app's Firebase configuration
        const firebaseConfig = {
            apiKey: "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
            authDomain: "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
            databaseURL: "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
            projectId: "XXXXXXXXXXXXXXXXXXXXXXXX",
            storageBucket: "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
            messagingSenderId: "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
            appId: "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
        };

        // Initialize Firebase
        const app = initializeApp(firebaseConfig);

 import {getDatabase, ref, child, get, onValue}
 from "https://www.gstatic.com/firebasejs/9.10.0/firebase-database.js"

 var umidade, temperatura, LevelVal, ControlAeration   

 const db = getDatabase();

 const firebaseRef1 = ref(db,"Control/");
 onValue(firebaseRef1, (snapshot)=>{
    
        umidade = snapshot.val().Humid;
        temperatura = snapshot.val().Temp;

        document.getElementById("humidityValue").innerText = `${umidade}%`;
        document.getElementById("tempValue").innerText = `${temperatura}°C`
 });

 const firebaseRef2 = ref(db,"Control/");
 onValue(firebaseRef2, (snapshot)=>{
    
        LevelVal = snapshot.val().level;

        changeColor(LevelVal);
 });

 const firebaseRef3 = ref(db,"Control/");
 onValue(firebaseRef3, (snapshot)=>{
    
        ControlAeration = snapshot.val().Aeration;

        changeColorCircle(ControlAeration);
 });

const element = document.getElementById("silo");
const elementCircle = document.getElementById("circle");

function changeColor (selector) {
    if (selector == 'R') {
        element.classList.add("siloRed")
        element.classList.remove("siloYellow", "siloGreen", "siloWhite")
    } else if (selector == 'Y') {
        element.classList.add("siloYellow")
        element.classList.remove("siloRed", "siloGreen", "siloWhite")
    } else if (selector == 'G') {
        element.classList.add("siloGreen")
        element.classList.remove("siloYellow", "siloRed", "siloWhite")
    } else if (selector == 'W') {
        element.classList.add("siloWhite")
        element.classList.remove("siloYellow", "siloGreen", "siloRed")
    }
}

function changeColorCircle (selector){
    if (selector == 'G') {
        elementCircle.classList.add("circleGreen")
        elementCircle.classList.remove("circleRed")
    } else if (selector == 'R') {
        elementCircle.classList.add("circleRed")
        elementCircle.classList.remove("circleGreen")
    }
}

setInterval(function(){
    const time = document.querySelector("#time");
    let date = new Date();
    let hours = date.getHours();
    let minutes = date.getMinutes();
    let seconds = date.getSeconds();
    let day_night = "AM"

    if (hours < 10) hours = '0' + hours;

    if (minutes < 10) minutes = '0' + minutes;

    if (seconds < 10) seconds = '0' + seconds;

    if(hours > 12){
        day_night = "PM";
        hours = hours - 12;
    }
    
    time.textContent = hours + ":" + minutes + ":" + seconds + " " + day_night;
})

