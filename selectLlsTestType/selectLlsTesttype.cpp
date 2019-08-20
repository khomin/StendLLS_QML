#include "selectLlsTesttype.h"
#include "dataBase/dataBase.h"
#include "settings.h"

SelectLlsTestType::SelectLlsTestType(QObject *parent) : QObject(parent)
{

}

void SelectLlsTestType::researchLlsTypes() {
    mModel.removeAll();
    auto res = DataBase::Instance().getDeviceFullDeviceList();
    if(!res.empty()) {
        for(auto i: res) {
            mModel.addItem(i.dts_name, i);
        }
    } else {
        emit dataBaseError(tr("Request returned zero. Please check the database"));
    }
}

void SelectLlsTestType::makeSelectedAsActive(int index) {
    int count = mModel.getAll().count();
    if(count) {
        if(count -1 >= index) {
            auto it = mModel.getAll().at(index);
            setActiveProfile(it.second.dts_name);
            setCntMax((uint32_t)it.second.values.cnt_value_max);
            setCntMin((uint32_t)it.second.values.cnt_value_min);
            setVolgateMax(it.second.values.voltage_value_max);
            setVolgateMin(it.second.values.voltage_value_min);
            setTempMax(Settings::Instance().getTempMax());
            setTempMin(Settings::Instance().getTempMin());
            setLevelEmpty(it.second.values.empty_value_minumum_correct);
            setLevelFull(it.second.values.full_value_minumum_correct);
            setTolerance(it.second.values.tolerance_level);
            setPowerCurrentNormal(it.second.values.nominal_current);
            setPowerCurrentTolerance(it.second.values.tolerance_current);
        }
    }
}
