#include "sd_card.h"

SD_FS::SD_FS(uint8_t ssPin, SPIClass &spi, uint32_t frequency, const char * mountpoint){
    !SD.begin(ssPin, spi, frequency) ? Serial.println("Card Mount Failed") : Serial.println("Card Mounted");
}

SD_FS::~SD_FS(){
    SD.end();
}

int SD_FS::get_card_info(sd_card_info &data, card_size_format format) {
    card_info.card_type = SD.cardType();

    // card_info.card_type_str = cardTypeToString(card_info.card_type);

    card_info.card_size = SD.cardSize() / 1024 ^ format;
    card_info.size_format = format;

    card_info.num_sectors = SD.numSectors();
    card_info.sector_size = SD.sectorSize();
    card_info.total_bytes = SD.usedBytes();

    data = card_info;
    return 0;
}

int SD_FS::listDir(const char* dirname, uint8_t levels) {
    root = SD.open(dirname);

    if(!root) {
        Serial.println("Failed to open directory");
        return -1;
    }
    if (!root.isDirectory()) {
        Serial.println("Not a directory");
        return -1;
    }

    File file = root.openNextFile();

    while (file) {
        if (file.isDirectory()) {
            Serial.print(" DIR: ");
            Serial.println(file.name());
            if (levels) {
                listDir(file.name(), levels - 1);
            } else {
                Serial.print(" FILE: ");
                Serial.println(file.name());
                Serial.print(" SIZE: ");
                Serial.println(file.size());
            }
            file = root.openNextFile();
        }
    }

    return 0;
}

int SD_FS::createDir(const char* path) {
    Serial.printf("Reading file: %s\n", path);
    if (SD.rmdir(path)) {
        Serial.println("Dir removed");
    } else {
        Serial.println("Rmdir faiiled");
        return -1;
    }
    return 0;
}

int SD_FS::removeDir(const char* path) {
    Serial.printf("Removing Dir: %s\n", path);
    if(SD.rmdir(path)) {
        Serial.println("Dir removed");
    } else {
        Serial.println("Rmdir failed");
        return -1;
    }
    return 0;
}

int SD_FS::readFile(const char* path) {
    Serial.printf("Reading file: %s\n", path);

    File file = SD.open(path);
    if (!file) {
        Serial.println("Failed to open file for reading");
        return -1;
    }
    Serial.print("Read from file: ");
    // while(file.available()) {
    //     Serial.write(file.read());
    // }
    file.close();
    return 0;
}

int SD_FS::writeFile(const char* path, const char* message) {
    Serial.printf("Writing file: %s\n", path);

    File file = SD.open(path, FILE_WRITE);
    if (!file) {
        Serial.println("Failed to open file for writing");
        return -1;
    }
    if (file.print(message)) {
        Serial.println("File written");
    } else {
        Serial.println("write failed");
        return -1;
    }
    file.close();
    return 0;
}

int SD_FS::appendFile(const char* path, const char* message) {
    Serial.printf("Appending to file: %s\n", path);

    File file = SD.open(path, FILE_APPEND);

    if(!file) {
        Serial.println("Failed to open file for appending");
        return -1;
    }
    if (file.print(message)) {
        Serial.println("Message appended");
    } else {
        Serial.println("Append failed");
        return -1;
    }
    file.close();
    return 0;
}

int SD_FS::renameFile(const char* path1, const char* path2) {
    Serial.printf("Renaming file %s to %s\n", path1, path2);

    if (SD.rename(path1, path2)) {
        Serial.println("File renamed");
    } else {
        Serial.println("Rename failed");
        return -1;
    }

    return 0;
}

int SD_FS::deleteFile(const char * path) {
    Serial.printf("Deleting File: %s\n", path);
    if (SD.remove(path)) {
        Serial.println("File deleted");
        return 0;
    } else {
        Serial.println("Delete failed");
        return -1;
    }
}

int SD_FS::testFileIO(const char* path) {
    File file = SD.open(path);

    static uint8_t buf[512];
    size_t len = 0;
    uint32_t start = millis();
    uint32_t end = start;

    if (file) {
        len = file.size();
        size_t flen = len;
        start = millis();
        while (len) {
            size_t toRead = len;
            if (toRead < 512) {
                toRead = 512;
            }
            file.read(buf, toRead);
            len -= toRead;
        }

        end = millis() - start;
        Serial.printf("%u bytes read for %u ms/n", flen, end);
        file.close();
    } else {
        Serial.println("Failed to open file for reading");
        return -1;
    }

    file = SD.open(path, FILE_WRITE);
    if(!file) {
        Serial.println("Failed to open file for writing");
        return -1;
    }
    size_t i;
    start = millis();
    for (i = 0; i < 2048; i++) {
        file.write(buf, 512);
    }
    end = millis() - start;
    Serial.printf("%u bytes written for %u ms\n", 2048* 512, end);
    file.close();

    return 0;
}
const char* cardTypeToString(uint8_t cardType) {
    switch(cardType) {
        case CARD_NONE_T:
            return "No card";
        case CARD_MMC_T:
            return "MMC";
        case CARD_SD_T:
            return "SD";
        case CARD_SDHC_T:
            return "SDHC";
        case UNKNOWN:
        default:
            return "Unknown";
    }
}

int SD_FS::print_card_info() {
    Serial.println("------SD CARD INFO-----");
    Serial.print("Card Type: ");
    Serial.println(cardTypeToString(card_info.card_type));
    Serial.print("Card Size: ");
    Serial.println(card_info.card_size);
    Serial.print("Number of Sectors: ");
    Serial.println(card_info.num_sectors);
    Serial.print("Sector Size: ");
    Serial.println(card_info.sector_size);
    Serial.print("Total Bytes: ");
    Serial.println(card_info.total_bytes);
    Serial.print("Used Bytes: ");
    Serial.println(card_info.used_bytes);
    Serial.print("Size Format: ");
    Serial.println(card_info.size_format);
    Serial.println("-----------------------");
    return 0;
}

