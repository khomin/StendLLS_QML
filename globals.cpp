#include "globals.h"

Globals::Globals() {}

Globals::~Globals() {

}

QString Globals::hexToString(char *pdata, uint16_t len) {
    QString res;
    for(uint8_t i=0; i<len; i++) {
        res += QString::number(pdata[i], 16);
    }
    return res;
}
