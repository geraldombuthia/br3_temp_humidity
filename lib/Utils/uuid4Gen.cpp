#include "uuid4Gen.h"
#include "./Time/ntpClient.h"


uuid_error_t UUID4::begin(long int seed1, long int seed2, bool isVariant4) {
    if (isVariant4) {
        uuid.setVariant4Mode();
    } else {
        uuid.setRandomMode();
    }

    long epoch_time;
    NTP::get_default_instance()->getEpochTime(epoch_time);
    seed1 = static_cast<uint32_t>(epoch_time);
    uuid.seed(seed1, seed2);
    set_global_UUID();
    Serial.printf("Initialized UUID as %s\n", isVariant4? "Version 4 spec": "Random Mode");
    return UUID_INIT_OK;
}
uuid_error_t UUID4::gen_uniq_UUID(String &uuid_str){
    uuid.generate();
    char* uuid_buffer = uuid.toCharArray();
    uuid_str = String(uuid_buffer);

    if (uuid_str.isEmpty()) {
        Serial.printf("Unique UID generation failed");
        return UUID_GEN_ERROR;
    }
    Serial.printf("Unique UID generation success\n");
    return UUID_GEN_OK;
}
uuid_error_t UUID4::set_global_UUID(){
    _uuid = String(uuid.toCharArray());
    globalIsSet = true;
    return UUID_SET_OK;
}
uuid_error_t UUID4::get_global_UUID(String &global_uuid) {

    if (!globalIsSet) {
        set_global_UUID();
        globalIsSet = true;
    }
    global_uuid = _uuid;

    return UUID_SET_OK;
}