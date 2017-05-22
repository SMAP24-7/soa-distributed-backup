# Backup distribuido
##Explicación

Es un sistema basado entre clientes de destino, cliente origen y un servidor, en el cuál usaremos para copiar un árbol del cliente origen, y replicarlo en los distintos clientes destino.

Este archivo Programa ha sido creado con QT Creator 4.2.1 (QT 5.8), en ubuntu.
##Pasos
1. Ejecutar el servidor, elegir un puerto e iniciar
2. Ejectuar clientes
<ul>
<li> Ejecutar uno o varios clientes de destino y/o de origen
<ul><li> Si al darle a conectar el botón no se desmarca, es porque no ha podido establecer la conexión.
<li> Cuando hayan clientes suficientes se copiará el arbol de directorios y archivos en los clientes de destino.
<li> Los archivos copiados se verán en la lista, tanto en el cliente de destino como en el de origen
<li> En caso de no poseer clientes suficientes para la copia de los archivos, no se copiará, solo se enviará un mensaje con el número de clientes.
</ul></ul>

##Características
1. Se ha usado un protocolo TCP.
2. Se ha usado un protocolo de texto.
3. Se ha implantado un formato propio.

##Protocolo y funcionamiento
Es un protocolo propio;
primero se conecta el servidor
Si se conecta un cliente desntion, este quedará a la escucha y el servidor lo añadirá a una lsita.
En caso de que sea un cliente de origen, esté al conectarse enviará un paquete con el tamaño total de archivos y el mínimo número de clientes que necesita para empezar a enviar clientes. (el origen se pone en modo escucha)
Cada vez que se conecte un cliente de destino al servidor y este tenga conectado un cliente de origen, enviará al cliente de origen el número de clientes destino que existen conectados.
Si el número de clientes destino es adecuado, el cliente origen comenzará a enviar archivos.
Cuando empieza el envío, se enviarán QBytearray serializados con el siguiente cuerpo:
<li>tamaño de los archivos
<li>número de archivos que se envían
<li>ruta del archivo
<li>nombre del archivo
<li>contenido del archivo</li>

##Errores
Al montar una ui sobre una clase WIdget, con ubn QTcpLocal, y sobre este el Qtcpsocket, al hacer punteros a la ui para cambiar el contenido en tiempo real dan fallos de segmentación.
El servidor no es capaz de quitar de la lista de clientes, aquellos clientes que se han desconectado.
Al recibir el número de clientes conectados al servidor, en el cliente de origen siempre lo lee como si fueran 0 clientes, en lugar de X, pero al tener los clientes necesarios si los envía correctamente.