#ifndef SD_CARD_H
#define SD_CARD_H

#include "FS.h"
#include "SD.h"
#include "SPI.h"

enum card_type: uint8_t {
    CARD_NONE_T = 0,
    CARD_MMC_T,
    CARD_SD_T,
    CARD_SDHC_T,
    UNKNOWN
};

enum card_size_format: unsigned int {
    BYTE = 0,
    KILO_BYTE,
    MEGA_BYTE,
    GIGA_BYTE,
    TERA_BYTE
};

struct sd_card_info {
    const char* card_type_str;
    uint8_t card_type;
    uint64_t card_size;
    size_t num_sectors;
    size_t sector_size;
    uint64_t total_bytes;
    uint64_t used_bytes;
    card_size_format size_format;
};

class SD_FS
{
    public:
    SD_FS(uint8_t ssPin=5, SPIClass &spi=SPI, uint32_t frequency=4000000, const char * mountpoint="/sd");
    ~SD_FS();
    // unsigned int card_type();
    int get_card_info(sd_card_info &data, card_size_format format = card_size_format::MEGA_BYTE);
    int readRAW(uint8_t* buffer, uint32_t sector);
    int writeRAW(uint8_t* buffer, uint32_t sector);
    int listDir(const char * dirname, uint8_t levels);
    int createDir(const char * path);
    int removeDir(const char * path);
    int readFile(const char * path);
    int writeFile(const char * path, const char * message);
    int appendFile(const char * path, const char * message);
    int renameFile(const char * path1, const char * path2);
    int deleteFile(const char * path);
    int testFileIO(const char * path);
    int print_card_info();
    private:
    File root;
    sd_card_info card_info;
     

};

#endif // SD_CARD_H