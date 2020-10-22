#pragma once

#include "mod_interface.hpp"
#include "HMUI/ViewController.hpp"
#include "extern/custom-types/shared/macros.hpp"
#include "extern/custom-types/shared/register.hpp"
#include "extern/custom-types/shared/types.hpp"

DECLARE_CLASS_CODEGEN(ScorePercentage, UIController, HMUI::ViewController,
    
    DECLARE_OVERRIDE_METHOD(void, DidActivate, il2cpp_utils::FindMethodUnsafe("HMUI", "ViewController", "DidActivate", 3), bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling);

    REGISTER_FUNCTION(UIController,
        REGISTER_METHOD(DidActivate);
    )
)
