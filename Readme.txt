			Cuarto Inteligente.

	INSTITUTO TECNOLÓGICO NACIONAL DE MÉXICO
    	INSTITUTO TECNOLÓGICO DE LEÓN

    	Materia: Sistemas Programables.

    	   Fecha: 06/Dic/2018

Integrantes: 
	- Alvarez Castro Cynthia Aideé.
	- Hernández Márquez Isaac Ulises.
	- Pérez Segura Isaac.

Objetivo: 
	Diseñar un sistema de control en red 
	basado en el sistema arduino. El 
	escenario, la aplicación, el objetivo
	y la motivación deben ser dadas por los alumnos.

Materiales:
	- Reed switch.
	- Imanes.
	- LEDs de colores.
	- Arduino UNO.
	- Cables o jumpers.
	- 1 motor a pasos.
	- Sensor de temperatura.
	- Sensor de humedad.
	- Fotoresistencia.

Criterios a evaluar:
	° Los requisitos propuestos deben ser cumplidos:
		-	Incluir 3 sensores y 1 actuador.
		- 	Se debe identificar puertos y protocolos
			de comunicación.
		- 	El arduino debe estar conectado a la red.
		- 	El sistema debe mostrar estadísticas de 
			los sensores y tomar acciones en 
			consecuencia.


Nota importante:
	Antes de hacer el funcionamiento del cuarto 
	inteligente, debe sobreescribir el arduino UNO 
	con la clase ArduinoNetworking para llevar a
	 cabo su buen funcionamiento.

Funcionamiento:
	Se simuló un cuarto donde se resguarda un servidor
	web. Para esto el reed switch funciona como un 
	ayudante anti-intrusos. 

	En caso de que estés en tu casa, no necesariamente
	debes activar la alarma. Caso contrario, en la 
	interfaz que nos arroja nuestro servidor podemos hacer 
	esa modificación por medio de un botón automático. ¿En
	qué consta esta parte? 
	Si alguien llegara por casualidad y abre de una forma
	forzosa la puerta donde se encuentre el servidor, 
	la alarma al ser automática detectará que está abierta
	y encenderá un led que indica que alguien entró al 
	cuarto del servidor. Solo puede ser desactivada por medio
	de la interfaz web.

	Además cuenta con algunos sensores, como el de 
	temperatura y humedad, donde nos indicará la temperatura 
	del cuarto. En caso de que sea mayor a 18 grados y se 
	haya dejado el ventilador en modo automático, se encenderá 
	para bajar la temperatura y que esté el servidor en 
	buen estado.

	Si no hay luz en el cuarto, gracias a la fotoresistencia,
	podemos saber que tanta iluminación hay dentro. Para eso
	simulamos con un LED el foco del cuarto. 
	Tanto la alarma y la activación del ventilador, tienen
	funcionalidad automática.

