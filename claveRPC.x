/*estructura Paquete de claves.h */
struct paquete_rpc {
    int x_rpc;
    int y_rpc;
    int z_rpc;
};

/*argumentos para empaquetar los datos de set_value y modify_value */
struct set_value_args {
    string key<256>;        
    string value1<256>;     
    int N_value2;           
    float V_value2<32>;     
    paquete_rpc value3;     
};

/*estructura para devolver los datos en get_value */
struct get_value_res {
    int codigo_error;       
    string value1<256>;
    int N_value2;
    float V_value2<32>;
    paquete_rpc value3;
};

/*definición del programa RPC y sus funciones*/
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