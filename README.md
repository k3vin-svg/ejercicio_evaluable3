# ejercicio_evaluable3
el valor de retorono corresponde con la logica de nuestro RPC
TENER CUIDADO --> puede fallar RPC --> inicializar a True
porque por defecto al inicializar si no le damos un valor nos da 0 --> y eso es False


comandos -aNm --> 
rpcgen -aNM claveRPC.x
Diferencia entre sockets y RPC
En sockets teniammos que hacerlo de forma manual, en RPC no. Lo hace de forma automática


para sockets lo que chocaba es mandar numeros enteros --> faltaba eso
Ahora en rpc ya no hace falta


declaramos struct porque es una estructura completa --> muchos argumentos en la funcion
sin embargo cuando hay funciones con solo char *key --> 