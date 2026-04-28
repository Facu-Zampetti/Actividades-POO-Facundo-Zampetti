# Actividad introductoria

Realizar un sistema a elección por el estudiante. Tanto el IDE, como los lenguajes a utilizar son totalmente por preferencia del estudiante. Lo importante es hacer un sistema de manera eficiente y eficaz, en un día, con la ayuda de la IA.

#Calculadora de Combustible
## Descripción
Aplicación web sencilla que permite calcular el **costo estimado de combustible** para un viaje, en base a la distancia, el consumo del vehículo y el precio actual de la nafta.

---

## Funcionalidades
- Ingreso de la **distancia del viaje** en kilómetros.
- Ingreso del **consumo del auto** en litros cada 100 km.
- Ingreso del **precio por litro** de nafta.
- Cálculo automático de:
  - Litros de combustible necesarios.
  - Costo total estimado del viaje.
- Botón **Limpiar** para reiniciar todos los campos.
- Soporte para calcular presionando la tecla **Enter**.
- Validación de datos: muestra advertencia si algún campo es inválido o vacío.

---

## Tecnologías utilizadas

| Tecnología | Uso |
|------------|-----|
| HTML5      | Estructura de la página |
| CSS3       | Estilos, animaciones y diseño responsivo |
| JavaScript | Lógica de cálculo e interacción con el DOM |

---

## Estructura del proyecto

```
calculadora-combustible/
├── index.html   → Estructura principal de la interfaz
├── style.css    → Estilos visuales y animaciones
└── script.js    → Lógica de cálculo y manejo de eventos
```

---

## Cómo usar

1. Abrir el archivo `index.html` en cualquier navegador web.
2. Completar los tres campos:
   - **Distancia del viaje** (ej: `350` km)
   - **Consumo del auto** (ej: `8` L/100km)
   - **Precio del litro de nafta** (ej: `950`)
3. Presionar el botón **Calcular** o la tecla **Enter**.
4. Ver el resultado con los litros necesarios y el costo estimado.
