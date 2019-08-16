#include "validateQchValues.h"
#include "settings.h"
#include <QRegularExpression>
#include "stendApi.h"

ValidateQchValues::ValidateQchValues(SelectLlsTestType * pSelectLlsType) {
    mSelectLlsType = pSelectLlsType;
    clearAllToFalse();
}

void ValidateQchValues::clearAllToFalse() {
    mFullCompare = fullMaxValueConst;
    mEmptyCompare = 0;
    setPowerCurrentValid(false);
    setLevelValid(false);
    setLevelFull(0);
    setLevelEmpty(0);
    setSnValid(false);
    setMcuSnValid(false);
    setTempValid(false);
    setRs232IsNormal(false);
    setRs485IsNormal(false);
    setLevelFullTriggered(false);
    setLevelEmptyTriggered(false);

    emit invalidateValues();
}

void ValidateQchValues::insertLlsData(Globals::sDutBaseStruct llsData) {
    /* clear all progress after disconnect lls */
    if((!getRs232IsNormal() && llsData.currentDataRs232.isValid)
            || (!getRs485IsNormal() && llsData.currentDataRs485.isValid)) {
        clearAllToFalse();
    }

    setPowerCurrentValid(
                llsData.power_current >= Settings::Instance().getCurMin().toFloat()
                && llsData.power_current <= Settings::Instance().getCurMax().toFloat()
                );
    setLevelValid(
                llsData.cnt >= mSelectLlsType->getCntMin()
                && llsData.cnt <= mSelectLlsType->getCntMax()
                );
#ifdef USE_VALIDATE_VOLTAGE
    setVoltageValid
            (
                );
#endif

    setSnValid(matchNumber(QString::fromLocal8Bit(llsData.serial_number, strlen(llsData.serial_number)-1)));

    setMcuSnValid(matchNumber(Globals::hexToString(llsData.mcu_serial_number, sizeof(llsData.mcu_serial_number))));

    setTempValid(
                llsData.currentDataRs232.temperature >= mSelectLlsType->getTempMin()
                && llsData.currentDataRs232.temperature <= mSelectLlsType->getTempMax()
                );

    auto freq = getValidFrequncy(llsData.currentDataRs485, llsData.currentDataRs232);

    if(freq > mEmptyCompare && freq != 0xFFFE) {
        mEmptyCompare = freq;
    }
    setLevelEmpty(mEmptyCompare);
//    setLevelEmptyTriggered(valuesLevelIsNormal(mEmptyCompare,
//                                               mSelectLlsType->getLevelEmpty(),
//                                               mSelectLlsType->getTolerance()));
    setLevelEmptyTriggered(true);

    if(freq < mFullCompare && freq != 0xFFFE) {
        mFullCompare = freq;
    }
    setLevelFull(mFullCompare);
//    setLevelFullTriggered(valuesLevelIsNormal(mFullCompare,
//                                               mSelectLlsType->getLevelFull(),
//                                               mSelectLlsType->getTolerance()));
    setLevelFullTriggered(true);

    setRs232IsNormal(llsData.currentDataRs232.isValid);

    setRs485IsNormal(llsData.currentDataRs485.isValid);
}

int ValidateQchValues::getValidFrequncy(Globals::sLlsCurrentData rs485, Globals::sLlsCurrentData rs232) {
    int res = false;
    if(rs232.isValid) {
        res = rs232.freq;
    } else if(rs485.isValid) {
        res = rs485.freq;
    }
    return res;
}

bool ValidateQchValues::valuesLevelIsNormal(int value, int constCompVal, float tollerance) {
    float minValue = constCompVal * (1 - tollerance);
    float maxValue = constCompVal * (1 + tollerance);
    if(value >= minValue &&
            value <= maxValue ) {
        return true;
    }
    return false;
}

bool ValidateQchValues::matchNumber(QString number) {
    QRegularExpression re("^[0-9-a-f-A-F]{1,32}$");
    if(number == "303030303030303030303030") {
        return false;
    }
    QRegularExpressionMatch match = re.match(number);
    return match.hasMatch();
}
