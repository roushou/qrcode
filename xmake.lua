set_languages("c++23")

add_rules("mode.debug", "mode.release")

add_requires("libqrencode")
add_requires("doctest")

-- Define binary target
target("qrcode")
set_kind("binary")
add_files("src/*.cpp")
add_packages("libqrencode")

-- Define test target
target("tests")
set_kind("binary")
add_files("tests/*.cpp")
add_includedirs("src")
add_packages("doctest")
