project("TestBed")
kind("ConsoleApp")
language("C")
cdialect("c11")

targetdir("%{wks.location}/bin/%{cfg.buildcfg}")
objdir("%{wks.location}/obj/%{cfg.buildcfg}")

files({ "**.c", "**.h" })

includedirs({ "../Hex/include" })

links({ "Hex", "ncurses" })

filter("configurations:Debug")
defines({ "DEBUG" })
symbols("On")
-- buildoptions({ "-fsanitize=address" })
-- linkoptions({ "-fsanitize=address" })

filter("configurations:Release")
defines({ "NDEBUG" })
optimize("On")
