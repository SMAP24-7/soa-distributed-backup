# Backup distribuido
##Explicación

Es un sistema basado entre clientes de destino, cliente origen y un servidor, en el cuál usaremos para copiar un árbol del cliente origen, y replicarlo en los distintos clientes destino.

Este archivo Programa ha sido creado con QT Creator 4.2.1 (QT 5.8), en ubuntu.
##Pasos
1. Ejecutar el servidor, elegir un puerto e iniciar
2. Ejectuar clientes
<ul>
<li> Ejecutar uno o varios clientes de destino.
<li> Ejecutar un cliente de origen.
<ul><li> Si al darle a conectar al cliente de origen, no sucede nada, esperar a que hayan clientes suficientes.
<li> Cuando hayan clientes suficientes y el cliente origen conecte, se copiará el arbol de directorios y archivos en los clientes de destino.
<li> Los archivos copiados se verán en la lista.
</ul></ul>

##Características
1. Se ha usado un protocolo TCP.
2. Se ha usado un protocolo de texto.
3. Se ha implantado un formato propio.
