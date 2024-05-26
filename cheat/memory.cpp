#include "memory.h"

// Définition de la fonction getMemory
Memory& getMemory() {
    static Memory memory("ac_client.exe"); // instance unique
    return memory;
}
