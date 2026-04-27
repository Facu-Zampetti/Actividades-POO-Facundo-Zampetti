function calcular(){

let distancia = Number(document.getElementById("distancia").value);
let consumo = Number(document.getElementById("consumo").value);
let precio = Number(document.getElementById("precio").value);

let resultado = document.getElementById("resultado");

if(distancia <= 0 || consumo <= 0 || precio <= 0){

resultado.innerHTML =
"⚠️ Ingresa valores válidos en todos los campos.";

return;
}

let litros = (distancia * consumo) / 100;
let costo = litros * precio;

resultado.innerHTML =
"⛽ Combustible necesario: <b>" + litros.toFixed(2) + " L</b><br>" +
"💰 Costo estimado: <b>$" + costo.toFixed(2) + "</b>";

}

function limpiar(){

document.getElementById("distancia").value = "";
document.getElementById("consumo").value = "";
document.getElementById("precio").value = "";

document.getElementById("resultado").innerHTML =
"Ingresa los datos para calcular el costo del viaje";

}

document.addEventListener("keypress", function(event){

if(event.key === "Enter"){
calcular();
}

});