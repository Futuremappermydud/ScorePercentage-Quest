#include "CustomConfigValue.hpp"

void CustomBoolConfigValue::SetValue(bool value) const {
    getConfig().config[name].SetBool(value);
    getConfig().Write();
}

bool CustomBoolConfigValue::LoadValue() const {
    return getConfig().config[name].GetBool();
}