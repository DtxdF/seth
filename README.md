# seth (**Sólo demostrativo**)
> Seth es una pequeña herramienta para recopilar información de puntos de acceso a nuestro alrededor.

## Instalación
```bash
git clone https://github.com/DtxdF/seth.git
cd seth
chmod +x BUILD
sudo ./BUILD
...
```

## Uso
Ejecutamos el servidor "**Web Sockets**"

```bash
sudo python3 WSServer.py
...
```
Ejecutamos el servidor "**HTTP**"

```bash
sudo python3 HTTPServer.py
...
```

Es mejor si hacemos *«Port forwarding»* con **Ngrok** por ejemplo, para obtener el **HTTPS**, pero existe la posibilidad de usar una clave y un certificado arbitratrio si así se desea (**NO RECOMENDABLE**), pero es mejor que se use la primera opción que la segunda debído a que es posible que algunos navegadores no permitan usar la geolocalización del dispositivo en un contexto no seguro, cómo **HTTP** ó clave y certificado no válidos.

```bash
./ngrok http 8080 # WebSocket
...
./ngrok http 8081 # HTTP
...
```

Abrimos el índice de la página (**index.html**) y nos ubicamos en la línea **6**  que básicamente contendrá un script que permitirá obtener la ubicación del dispositivo que ingrese en ella.

```html
...
ws = new WebSocket('wss://<URL generada por Ngrok>');
...
```

Una vez se haya obtenido y registrado la ubicación, ejecute "seth" para empezar a capturar los puntos de acceso.

```bash
sudo ./program -h # Para requerir ayuda
...
sudo ./program -i <Nombre de la interfaz de red> -s <El tiempo de espera para reescanear> # Recomendado: 3
...
```

Ya finalizado. En la URL de Ngrok para HTTP puede ver un mapa interactivo con los puntos de acceso capturado junto con su información. Por ejemplo: \<URL de Ngrok\>/tracked

***Hay que tener en cuenta que el programa no registra la ubicación de los puntos de acceso, sino la ubicación en dónde se registra éstos. Es relativa según el GPS y la configuración del dispositivo.***

***Puede que haya un registro en dónde los puntos de acceso se superpongan, pero puede mover esos marcadores a su gusto.***

***Cómo ya se mencionó el programa es demostrativo, pero no significa que no se pueda jugar con él.***

### Imagen demostrativa:

![](https://imgur.com/BEJZbPL.png)

**Nota: Puede usar un parámetro GET llamado "*target*" para mostrar la ubicación de un BSSID que esté en la base de datos. Ejemplo: URL/tracked?target=\<BSSID\>**

Tenga en cuenta que las técnicas antes mencionadas son mejores en grupo, por lo que existe una herramienta para "unir" todos los datos que no estén en otra. [db_join.py](db_join.py) nos va a permitir hacer eso, unir las direcciones no existentes para aumentar nuestras posibilidades.

```bash
python3 db_join.py <Base de datos a unir>
...
```

\~ DtxdF
