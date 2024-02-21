project("Hex")
kind("StaticLib")
language("C")
cdialect("c11")

targetdir("%{wks.location}/bin/%{cfg.buildcfg}")
objdir("%{wks.location}/obj/%{cfg.buildcfg}")

includedirs({ "./include" })

pchheader("include/Utils/types.h")
pchsource("src/Utils/types.c")

files({
	"include/**.h",
	"src/**.c",
})

links({ "ncurses" })
defines({ "EDITOR_NCURSES_API" })

filter("configurations:Debug")
defines({ "DEBUG" })
symbols("On")
buildoptions({
	"-Wno-gnu-zero-variadic-macro-arguments",
	"-Wall",
	"-Werror",
	"-Wextra",
	"-Wshadow",
	"-Wformat=2",
	"-Wconversion",
	"-Wstrict-prototypes",
	"-pedantic",
	"-Wno-format-nonliteral",
	"-Wgnu-folding-constant",
	-- "-fsanitize=address",
})
-- linkoptions({ "-fsanitize=address" })

filter("configurations:Release")
defines({ "NDEBUG" })
optimize("On")
