#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "claves.h"       
#include "clavesRPC.h"    

/*variable global para mantener la conexion con el servidor rpc*/
static CLIENT *cliente = NULL;

static int conectar_servidor() {
    char *server_ip;
    /*caso de que ya estamos conectados*/
    if (cliente != NULL) return 0;

    /*leemos la IP de la variable de entorno*/
    server_ip = getenv("IP_TUPLAS");
    if (server_ip == NULL) {
        printf("Error: Variable de entorno IP_TUPLAS no definida.\n");
        return -1;
    }

    /*creamos la conexión*/
    cliente = clnt_create(server_ip, CLAVES_PROG, CLAVES_VERS, "tcp");
    if (cliente == NULL) {
        clnt_pcreateerror(server_ip);
        return -1;
    }

    return 0;
}

int set_value(char *key, char *value1, int N_value2, float *V_value2, struct Paquete value3) {
    struct set_value_args args;
    enum clnt_stat resultado;
    int res = 0;

    /*verificación de parametros y tamaño*/
    if (key == NULL || value1 == NULL || V_value2 == NULL)
        return -1;
    if (N_value2 < 1 || N_value2 > 32)  
        return -1;
    if (strlen(key) > 255 || strlen(value1) > 255)
        return -1;
    /*cadenas vacías*/
    if (strlen(key) == 0 || strlen(value1) == 0)
    return -1;

    /*conectamos el servidor*/
    if (conectar_servidor() == -1) return -1;
    /*copiamos los datos a nuestra estructura RPC*/
    args.key = key;
    args.value1 = value1;
    args.N_value2 = N_value2;
    
    /*copiamos los arrays dinámicos :tamaño y datos*/
    args.V_value2.V_value2_len = N_value2;
    args.V_value2.V_value2_val = V_value2;
    
    /*copiamos nuestro struct inicial */
    args.value3.x_rpc = value3.x;
    args.value3.y_rpc = value3.y;
    args.value3.z_rpc = value3.z;

    resultado = set_value_1(args,&res, cliente);

    /*comprobamos errores de red */
    if (resultado != RPC_SUCCESS) {
        clnt_perror(cliente, "Error en set_value_1");
        return -1;
    }

    return res;
}

int delete_key(char *key) {
    enum clnt_stat resultado;
    int res = 0; 

    if (conectar_servidor() == -1) return -1;

    /*le pasamos la clave y el cliente*/
    resultado = delete_key_1(key, &res, cliente);

    if (resultado != RPC_SUCCESS) {
        clnt_perror(cliente, "Error en delete_key_1");
        return -1;
    }

    return res;
}

int get_value(char *key, char *value1, int *N_value2, float *V_value2, struct Paquete *value3) {
    enum clnt_stat resultado_rpc;
    get_value_res resultado; 
    if (conectar_servidor() == -1) return -1;

    memset(&resultado, 0, sizeof(resultado));

    resultado_rpc = get_value_1(key, &resultado, cliente);
    if (resultado_rpc != RPC_SUCCESS) {
        clnt_perror(cliente, "Error en get_value_1");
        return -1;
    }
    /*si el servidor nos dice que la clave no existe, devolvemos error */
    if (resultado.codigo_error == -1) {
        xdr_free((xdrproc_t)xdr_get_value_res, (char *)&resultado);
        return -1;
    }

    /*si existe, copiamos los datos que nos trajo RPC a nuestras variables locales */
    strcpy(value1, resultado.value1);
    *N_value2 = resultado.N_value2;
    
    /*copiamos el vector */
    memcpy(V_value2, resultado.V_value2.V_value2_val, (*N_value2) * sizeof(float));
    /*copiamos los valores de la estructura paquete*/
    value3->x = resultado.value3.x_rpc;
    value3->y = resultado.value3.y_rpc;
    value3->z = resultado.value3.z_rpc;

    xdr_free((xdrproc_t)xdr_get_value_res, (char *)&resultado);

    return 0;
}

int modify_value(char *key, char *value1, int N_value2, float *V_value2, struct Paquete value3){
    set_value_args args;
    enum clnt_stat resultado;
    int res = 0;
    /*verificación de parametros y tamaño*/
    if (key == NULL || value1 == NULL || V_value2 == NULL)
        return -1;
    if (N_value2 < 1 || N_value2 > 32)  
        return -1;
    if (strlen(key) > 255 || strlen(value1) > 255)
        return -1;
    /*cadenas vacías*/
    if (strlen(key) == 0 || strlen(value1) == 0)
    return -1;

    if (conectar_servidor() == -1) return -1;
    /*copiamos los datos a nuestra estructura RPC*/
    args.key = key;
    args.value1 = value1;
    args.N_value2 = N_value2;
    
    /*copiamos los arrays dinámicos*/
    args.V_value2.V_value2_len = N_value2;
    args.V_value2.V_value2_val = V_value2;
    /*copiamos nuestro struct inicial*/
    args.value3.x_rpc = value3.x;
    args.value3.y_rpc = value3.y;
    args.value3.z_rpc = value3.z;

    resultado = modify_value_1(args, &res, cliente);
    if (resultado != RPC_SUCCESS) {
        clnt_perror(cliente, "Error en modify_value_1");
        return -1;
    }
    return res;


}

int exist(char *key){
    enum clnt_stat resultado;
    int res = 0;

    if (conectar_servidor() == -1) return -1;
    resultado = exist_1(key, &res, cliente);
    if (resultado != RPC_SUCCESS) {
        clnt_perror(cliente, "Error en exist_1");
        return -1;
    }
    return res;
}

int destroy(void){
    enum clnt_stat resultado;
    int res = 0;

    if (conectar_servidor() == -1) return -1;
    resultado = destroy_1(&res, cliente);
    if (resultado != RPC_SUCCESS) {
        clnt_perror(cliente, "Error en destroy_1");
        return -1;
    }
    return res;

}