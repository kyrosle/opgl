add_rules("mode.debug", "mode.release")

-- glfw: used to create or manage window, user input, program time, opengl context mangement
-- glad: Get the function address at runtime and save it in a function pointer for later use
add_requires("glfw", "glad")
add_packages("glfw", "glad")

add_rules("plugin.compile_commands.autoupdate", {outputdir = "build"})
set_targetdir("bin")

add_includedirs("lib")
add_files("lib/*.cpp")

targets = {
  "window",
  "triangle",
  "rectangle",
  "glsl",
  "texture"
}

for i, label in pairs(targets) do
  target(label)
    set_kind("binary")
    add_files("src/" .. label .. ".cpp")
end
