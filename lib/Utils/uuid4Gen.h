#ifndef UUID4_GEN_H
#define UUID4_GEN_H

#include <Arduino.h>
#include "UUID.h"

/**
 * @brief UUID error codes
 */
typedef enum uuid_error: unsigned int {
    UUID_GEN_OK,        //UUID generation was successful
    UUID_SET_OK,        //UUID generation was successful
    UUID_GEN_ERROR,     //UUID generation failed
    UUID_SET_ERROR,     //UUID set failed
    UUID_INIT_OK        // UUID Initialization okay
} uuid_error_t;

/**
 * @brief Utility to generate UUIDs
 */
class UUID4 {
    public:
    /**
     * @brief Initializes the uuid to provide necessary configurations
     * @param isVariant4 True if to use version4 conformity, false to use random
     * @return UUID_GEN_OK if successful or otherwise
     */
    uuid_error_t begin(long int seed1 = 1, long int seed2 = 2, bool isVariant4 = true);

    /**
     * @brief Generates a unique UUID to be used once
     * @param uuid variable to write the unique uuid to
     * @return UUID_GEN_OK if successful or otherwise
     */
    uuid_error_t gen_uniq_UUID(String &uuid);

    /**
     * @brief Generates and sets a global UUID to be used by the device in its entirety
     * @return UUID_GEN_OK
     */
    uuid_error_t set_global_UUID();
    
    /**
     * @brief 
     */
    uuid_error_t get_global_UUID(String &global_uuid);
    private:
    UUID uuid;
    String _uuid; //Stores a global uuid
    bool globalIsSet = false;
};
#endif