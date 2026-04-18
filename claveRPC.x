/* Estructura equivalente a struct Paquete de claves.h */
struct paquete_rpc {
    int x_rpc;
    int y_rpc;
    int valor3;
};

/* Argumentos para empaquetar los datos de set_value y modify_value */
struct set_value_args {
    string key<256>;         /* Cadena de hasta 255 caracteres + \0 */
    string value1<256>;      /* Cadena de hasta 255 caracteres + \0 */
    int N_value2;            /* Número de elementos del vector */
    float V_value2<32>;      /* Vector dinámico de floats (máximo 32) */
    paquete_rpc value3;      /* El struct paquete */
};

/* Estructura para devolver los datos en get_value */
struct get_value_res {
    int codigo_error;        /* 0 si éxito, -1 si error */
    string value1<256>;
    int N_value2;
    float V_value2<32>;
    paquete_rpc value3;
};

/* Definición del programa RPC y sus funciones remotas */
program CLAVES_PROG {
    version CLAVES_VERS {
        int SET_VALUE(set_value_args) = 1;
        get_value_res GET_VALUE(string key<256>) = 2;
        int MODIFY_VALUE(set_value_args) = 3;
        int DELETE_KEY(string key<256>) = 4;
        int EXIST(string key<256>) = 5;
        int DESTROY(void) = 6;
    } = 1;
} = 0x20000001; /* Número de programa único */