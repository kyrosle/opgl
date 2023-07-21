add_rules("mode.debug", "mode.release")

-- glfw: used to create or manage window, user input, program time, opengl context mangement
-- glad: Get the function address at runtime and save it in a function pointer for later use
add_requires("glfw", "glad", "glm", "freetype")
add_packages("glfw", "glad", "glm", "freetype")

add_rules("plugin.compile_commands.autoupdate", {outputdir = "build"})
set_targetdir("bin")

add_includedirs("lib")
add_files("lib/*.cpp")

target("game")
  set_kind("binary")
  add_includedirs("game2d")
  add_files("game2d/*.cpp")
  add_files("game2d/program.cpp")

targets = {
  "window",
  "triangle",
  "rectangle",
  "glsl",
  "texture",
  "3d",
  "camera",
  "color",
  "lighting",
  "material",
  "lightcaster",
  "multiplelight"
}

for i, label in pairs(targets) do
  target(label)
    set_kind("binary")
    add_files("src/" .. label .. ".cpp")
end
