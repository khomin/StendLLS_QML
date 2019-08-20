#include "validateQchValues.h"
#include "settings.h"
#include <QRegularExpression>
#include "stendApi.h"

ValidateQchValues::ValidateQchValues(SelectLlsTestType * pSelectLlsType) {
    mSelectLlsType = pSelectLlsType;
    clearAllToFalse();
}

void ValidateQchValues::clearAllToFalse() {
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

    /* pull compare values from database */
    mFullCompare = mSelectLlsType->getLevelFull();
    mEmptyCompare = mSelectLlsType->getLevelEmpty();

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

    size_t snLen = 0;
    if(strlen(llsData.serial_number) >= sizeof(llsData.serial_number)) {
        snLen = sizeof(llsData.serial_number);
    } else {
        snLen = strlen(llsData.serial_number);
    }

    /* sn validate  */
    setSnValid(matchSnNumber(QString::fromLocal8Bit(llsData.serial_number, (int)snLen)));

    /* mcu validate */
    setMcuSnValid(matchMcuNumber(Globals::hexToString(llsData.mcu_serial_number, sizeof(llsData.mcu_serial_number))));

    /* temp validate */
    setTempValid(
                llsData.currentDataRs232.temperature >= mSelectLlsType->getTempMin()
                && llsData.currentDataRs232.temperature <= mSelectLlsType->getTempMax()
                );

    /* frequency validate */
    auto freq = getValidFrequncy(llsData.currentDataRs485, llsData.currentDataRs232);
    if(freq > mEmptyCompare && freq != 0xFFFE) {
        mEmptyCompare = freq;
    }

    /* level empty validate */
    setLevelEmpty(mEmptyCompare);
    setLevelEmptyTriggered(valuesLevelIsNormal(mEmptyCompare,
                                               mSelectLlsType->getLevelEmpty(),
                                               mSelectLlsType->getTolerance()));

    /* level empty validate */
    if(freq < mFullCompare && freq != 0xFFFE) {
        mFullCompare = freq;
    }
    setLevelFull(mFullCompare);
    setLevelFullTriggered(valuesLevelIsNormal(mFullCompare,
                                              mSelectLlsType->getLevelFull(),
                                              mSelectLlsType->getTolerance()));
    /* rs232 validate */
    setRs232IsNormal(llsData.currentDataRs232.isValid);

    /* rs485 validate */
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

bool ValidateQchValues::matchSnNumber(QString number) {
    QRegularExpression re("^[\\d]{1,32}$");
    if(number == "303030303030303030303030") {
        return false;
    }
    number.replace("\r", 0).append(0);
    QRegularExpressionMatch match = re.match(number);
    return match.hasMatch();
}

bool ValidateQchValues::matchMcuNumber(QString number) {
    QRegularExpression re("^[0-9-a-f-A-F]{1,32}$");
    if(number == "----------") {
        return false;
    }
    number.replace("\r", 0).append(0);
    QRegularExpressionMatch match = re.match(number);
    return match.hasMatch();
}
