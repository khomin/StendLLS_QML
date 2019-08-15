#ifndef SELECTLLSTESTTYPE_H
#define SELECTLLSTESTTYPE_H

#include <QObject>
#include "dataBase/dataBase.h"
#include "dataBase/dataBaseProperty.h"
#include "selectLlsTestType/selectLlsTestModel.h"

class SelectLlsTestType : public QObject
{
    Q_OBJECT
public:
    explicit SelectLlsTestType(QObject *parent = nullptr);

    Q_PROPERTY(QString activeProfile READ getActiveProfile NOTIFY activeProfileChanged)

    Q_PROPERTY(int cntMax READ getCntMax NOTIFY cntMaxChanged)
    Q_PROPERTY(int CntMin READ getCntMin NOTIFY cntMinChanged)
    Q_PROPERTY(float volgateMax READ getVolgateMax NOTIFY volgateMaxChanged)
    Q_PROPERTY(float volgateMin READ getVolgateMin NOTIFY volgateMinChanged)
    Q_PROPERTY(float tempMax READ getTempMax NOTIFY tempMaxChanged)
    Q_PROPERTY(float tempMin READ getTempMin NOTIFY tempMinChanged)
    Q_PROPERTY(int levelEmptyMax READ getLevelEmpty NOTIFY levelEmptyChanged)
    Q_PROPERTY(int levelFullMax READ getLevelFull NOTIFY levelFullChanged)
    Q_PROPERTY(int tolerance READ getTolerance NOTIFY toleranceChanged)

    SelectLlsTestModel* getModel() {
        return &mModel;
    }

    Q_INVOKABLE void researchLlsTypes();

    Q_INVOKABLE void makeSelectedAsActive(int index);

    QString getActiveProfile() { return mActiveProfile;}
    void setActiveProfile(QString value) { mActiveProfile = value; activeProfileChanged(); }

    uint32_t getCntMax() { return mCntMax; }
    void setCntMax(uint32_t value) { mCntMax = value; emit cntMaxChanged(); }

    uint32_t getCntMin() { return mCntMin; }
    void setCntMin(uint32_t value) { mCntMin = value; emit cntMinChanged(); }

    float getVolgateMax() { return mVolgateMax; }
    void setVolgateMax(float value) { mVolgateMax = value; emit volgateMaxChanged(); }

    float getVolgateMin() { return mVolgateMin; }
    void setVolgateMin(float value) { mVolgateMin = value; emit volgateMinChanged(); }

    float getTempMax() { return mTempMax; }
    void setTempMax(float value) { mTempMax = value; emit tempMaxChanged(); }

    float getTempMin() { return mTempMin; }
    void setTempMin(float value) { mTempMin = value; emit tempMinChanged(); }

    int getLevelEmpty() { return mLevelEmpty; }
    void setLevelEmpty(int value) { mLevelEmpty = value; emit levelEmptyChanged(); }

    int getLevelFull() { return mLevelFull; }
    void setLevelFull(int value) { mLevelFull = value; emit levelFullChanged(); }

    float getTolerance() { return mTolerance; }
    void setTolerance(float value) { mTolerance = value; emit toleranceChanged(); }

signals:
    void cntMaxChanged();
    void cntMinChanged();
    void volgateMaxChanged();
    void volgateMinChanged();
    void tempMaxChanged();
    void tempMinChanged();
    void levelEmptyChanged();
    void levelFullChanged();
    void toleranceChanged();
    void activeProfileChanged();

    void dataBaseError(QString err);

private:
    SelectLlsTestModel mModel;
    QString mActiveProfile;
    uint32_t mCntMax;
    uint32_t mCntMin;
    float mVolgateMax;
    float mVolgateMin;
    float mTempMax;
    float mTempMin;
    int mLevelEmpty;
    int mLevelFull;
    float mTolerance;
};

#endif // SELECTLLSTESTTYPE_H
