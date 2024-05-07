add_rules("mode.debug", "mode.release")

add_requires("ncurses")

target("HexViewer")
set_kind("binary")
set_toolset("cc", "gcc")
set_languages("c11")

add_includedirs("Hex/include", { public = true })
add_files(
	"Hex/src/Core/*.c",
	"Hex/src/Utils/*.c",
	{ public = true }
)

add_packages("ncurses")

if is_mode("debug") then
	add_cflags("-Wall", "-Wextra", "-Wshadow", "-Wunused", "-Wno-gnu-zero-line-directive", { force = true })
	set_symbols("debug")
	add_defines("DEBUG")
end

if is_mode("release") then
	set_symbols("hidden")
	set_optimize("fastest")
	set_strip("all")
	add_defines("NDEBUG")
end