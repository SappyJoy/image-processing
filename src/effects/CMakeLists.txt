add_library(DitheringEffect SHARED DitheringEffect.cpp)
add_library(GlitchEffect SHARED GlitchEffect.cpp)
add_library(GrayScaleEffect SHARED GrayScaleEffect.cpp)
add_library(InvertEffect SHARED InvertEffect.cpp)

set_property(TARGET DitheringEffect PROPERTY POSITION_INDEPENDENT_CODE ON)
set_property(TARGET GlitchEffect PROPERTY POSITION_INDEPENDENT_CODE ON)
set_property(TARGET GrayScaleEffect PROPERTY POSITION_INDEPENDENT_CODE ON)
set_property(TARGET InvertEffect PROPERTY POSITION_INDEPENDENT_CODE ON)