## Una empresa que se dedica al desarrollo de software ofrece soluciones y utiliza las siguientes tecnologías y herramientas:
 	
### QtCreator como IDE
. Biblioteca Qt con C++ para aplicaciones Desktop.

. MySQL para base de datos y phpmyadmin para su gestión.

. FastAPI en Python para la API.

. Implementar en contenedores Mysql y FastAPI en un servidor Linux.

### Necesidad
. Se desea una aplicación desktop en Qt que tenga un login de usuarios.

. Cuando un usuario en la aplicación desktop ingrese un usuario válido obtendrá un access token desde la API desarrollada con FastAPI.

. Cuando se obtiene el access token, el usuario ya estará logueado, y podrá utilizar este token para próximas consultas a la API.

. Los usuarios se almacenarán en una tabla usuarios en MySQL.

. Se quiere montar FastAPI y MySQL en contenedores en la nube y que la aplicación desktop pueda loguearse contra FastAPI.

# Sistema de Login - Qt + FastAPI + MySQL

## Tecnologías utilizadas

* Qt (C++)
* FastAPI (Python)
* MySQL
* Docker


## Requisitos

* Docker Desktop
* Qt Creator
* Python (opcional para pruebas)


## Cómo ejecutar el proyecto

### 1. Levantar backend con Docker

Ir a la carpeta raíz del proyecto:

```
docker-compose up --build
```

Esto levantará:

* MySQL en contenedor
* API FastAPI en http://localhost:8000


### 2. Crear usuario en la base de datos

Abrir terminal:

```
docker exec -it mysql_container mysql -u root -p
```

Password:

```
root
```

Luego ejecutar:

```sql
USE empresa_db;

CREATE TABLE usuarios (
    id INT AUTO_INCREMENT PRIMARY KEY,
    username VARCHAR(50) NOT NULL UNIQUE,
    password VARCHAR(255) NOT NULL
);

INSERT INTO usuarios (username, password)
VALUES ('facu', '1234');
```


### 3. Ejecutar aplicación Qt

Abrir el proyecto en Qt Creator:

```
frontend/LoginApp
```

Ejecutar la aplicación.


## Uso

* Ingresar usuario: facu
* Contraseña: 1234
* Se obtiene token desde FastAPI
* Se muestra en segunda ventana


## Funcionalidades

* Login de usuario
* Validación contra MySQL
* Generación de token JWT
* Consumo de API desde Qt
* Interfaz gráfica en C++


## Autor

Facundo Zampetti
