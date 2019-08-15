#ifndef VALIDATEQCHVALUES_H
#define VALIDATEQCHVALUES_H

#include <QObject>
#include "globals.h"
#include "stendProperty.h"
#include "selectLlsTestType/selectLlsTesttype.h"
#include "globals.h"

class ValidateQchValues : public QObject
{
    Q_OBJECT
public:
    explicit ValidateQchValues(SelectLlsTestType * pSelectLlsType);

    Q_PROPERTY(bool powerCurrValid READ getPowerCurrentValid NOTIFY powerCurrentValidChanged)
    Q_PROPERTY(bool levelValid READ getLevelValid NOTIFY levelValidChanged)

#ifdef USE_VALIDATE_VOLTAGE
    Q_PROPERTY(bool voltageValid READ getVoltageValid NOTIFY voltageValidChanged)
#endif
    Q_PROPERTY(bool snValid READ getSnValid NOTIFY snValidChanged)
    Q_PROPERTY(bool mcuSnValid READ getMcuSnValid NOTIFY mcuSnValidChanged)
    Q_PROPERTY(bool tempValid READ getTempValid NOTIFY tempValidChanged)

    Q_PROPERTY(bool rs232IsNormal READ getRs232IsNormal NOTIFY rs232IsChanged)
    Q_PROPERTY(bool rs485IsNormal READ getRs485IsNormal NOTIFY rs485IsChanged)

    Q_PROPERTY(int levelEmptyValue READ getLevelEmpty NOTIFY levelEmptyChanged)
    Q_PROPERTY(int levelFullValue READ getLevelFull NOTIFY levelFullChanged)

    Q_PROPERTY(bool levelEmptyTriggered READ getLevelEmptyTriggered NOTIFY levelEmptyTriggeredChanged)
    Q_PROPERTY(bool levelFullTriggered READ getLevelFullTriggered NOTIFY levelFullTriggeredChanged)


    Q_INVOKABLE bool getPowerCurrentValid() { return mPowerCurrValid; }
    Q_INVOKABLE void setPowerCurrentValid(bool value) { mPowerCurrValid = value; emit powerCurrentValidChanged(); }

    Q_INVOKABLE bool getLevelValid() { return mLevelValid; }
    Q_INVOKABLE void setLevelValid(bool value) { mLevelValid = value; emit levelValidChanged(); }

    Q_INVOKABLE bool getVoltageValid() { return mVoltageValid; }
    Q_INVOKABLE void setVoltageValid(bool value) { mVoltageValid = value; emit voltageValidChanged(); }

    Q_INVOKABLE bool getSnValid() { return mSnValid; }
    Q_INVOKABLE void setSnValid(bool value) { mSnValid = value; emit snValidChanged(); }

    Q_INVOKABLE bool getMcuSnValid() { return mMcuSnValid; }
    Q_INVOKABLE void setMcuSnValid(bool value) { mMcuSnValid = value; emit mcuSnValidChanged(); }

    Q_INVOKABLE bool getTempValid() { return mTempCurrValid; }
    Q_INVOKABLE void setTempValid(bool value) { mTempCurrValid = value; emit tempValidChanged(); }

    Q_INVOKABLE bool getLevelEmptyTriggered() { return mLevelEmptyTriggered; }
    Q_INVOKABLE void setLevelEmptyTriggered(bool value) { mLevelEmptyTriggered = value; emit levelEmptyTriggeredChanged(); }

    Q_INVOKABLE bool getLevelFullTriggered() { return mLevelFullTriggered; }
    Q_INVOKABLE void setLevelFullTriggered(bool value) { mLevelFullTriggered = value; emit levelFullTriggeredChanged(); }

    Q_INVOKABLE int getLevelEmpty() { return mLevelEmptyValue; }
    Q_INVOKABLE void setLevelEmpty(int value) { mLevelEmptyValue = value; emit levelEmptyChanged(); }

    Q_INVOKABLE int getLevelFull() { return mLevelFullValue; }
    Q_INVOKABLE void setLevelFull(int value) { mLevelFullValue = value; emit levelFullChanged(); }

    Q_INVOKABLE bool getRs232IsNormal() { return mRs232IsNormal; }
    Q_INVOKABLE void setRs232IsNormal(bool value) { mRs232IsNormal = value; emit rs232IsChanged(); }

    Q_INVOKABLE bool getRs485IsNormal() { return mRs485IsNormal; }
    Q_INVOKABLE void setRs485IsNormal(bool value) { mRs485IsNormal = value; emit rs485IsChanged(); }

public slots:
    void insertLlsData(Globals::sDutBaseStruct llsData);

signals:
    bool powerCurrentValidChanged();
    bool levelValidChanged();
    bool voltageValidChanged();
    bool snValidChanged();
    bool mcuSnValidChanged();
    bool tempValidChanged();
    bool levelEmptyTriggeredChanged();
    bool levelFullTriggeredChanged();
    bool rs485IsChanged();
    bool rs232IsChanged();
    void levelEmptyChanged();
    void levelFullChanged();

private:
    void clearAllToFalse();

    bool matchNumber(QString number);

    bool valuesLevelIsNormal(int value, int constCompVal, float tollerance);

    int getValidFrequncy(Globals::sLlsCurrentData rs485, Globals::sLlsCurrentData rs232);

    bool mPowerCurrValid = false;
    bool mLevelValid = false;
    bool mVoltageValid = false;
    bool mSnValid = false;
    bool mMcuSnValid = false;
    bool mTempCurrValid = false;
    int mLevelEmptyValue = 0;
    int mLevelFullValue = 0;
    bool mLevelEmptyTriggered = false;
    bool mLevelFullTriggered = false;
    bool mRs232IsNormal = false;
    bool mRs485IsNormal = false;
    int mEmptyCompare = 0;
    int mFullCompare = 0;

    static constexpr uint16_t fullMaxValueConst = 0xFFFF;

    SelectLlsTestType * mSelectLlsType = nullptr;
};

#endif // VALIDATEQCHVALUES_H
