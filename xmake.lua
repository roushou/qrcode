set_languages("c++23")

add_rules("mode.debug", "mode.release")

add_requires("libqrencode")

-- Define binary target
target("qrcode")
set_kind("binary")
add_files("src/*.cpp")
add_packages("libqrencode")
