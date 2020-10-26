#pragma once
#include "beatsaber-hook/shared/utils/utils.h"
#include "beatsaber-hook/shared/config/config-utils.hpp"

#include "customlogger.hpp"

Configuration& getConfig();

template <typename ValueType> 
class CustomConfigValue {
    protected:
        std::string name;
        ValueType value;
        
        virtual ValueType LoadValue() const = 0;
        
    public:
        CustomConfigValue(std::string name, ValueType defaultValue) {
            this->name = name;
            this->value = defaultValue;
        }
        virtual void SetValue(ValueType value) const = 0;

        void Load() {
            if(getConfig().config.HasMember(name)) {
                getLogger().info("CustomConfigValue: Load %s", name.c_str());
                value = LoadValue();
            } else {
                getLogger().info("CustomConfigValue: Create %s", name.c_str());
                if(getConfig().config.ObjectEmpty()) getConfig().config.SetObject();
                auto& allocator = getConfig().config.GetAllocator();
                getConfig().config.AddMember(rapidjson::Value(name.c_str(), allocator).Move(), value, allocator);
                getConfig().Write();
            }
        }

        ValueType GetValue() const {
            return value;
        }

        std::string GetName() const {
            return name;
        }
};

class CustomBoolConfigValue : public CustomConfigValue<bool> {
    protected:
        virtual bool LoadValue() const;
    public:
        CustomBoolConfigValue(std::string name, bool defaultValue) : CustomConfigValue<bool>(name, defaultValue) { }
        virtual void SetValue(bool value) const;
};
