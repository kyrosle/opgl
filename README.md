# OpenGL

platform: `Fedora`
code tool: `vscode`
package manager: `xmake`

pre install

```bash
xrepo install glfw
xrepo install glad
```

and then the setting referencing `xmake.lua`.

- `window.cpp`: the minimum runnable application
  ![](pic/window.png)
- `triangle.cpp`: working flow for drawing a triangle (VAO, VBO)
  ![](pic/trangle.png)
- `rectangle.cpp`: working flow for drawing a rectangle (EBO)
  ![](pic/rectangle.png)
- `glsl.cpp`: shader programming
  ![](pic/Peek%202023-07-13%2016-00.gif)
- `texture.cpp`: add texture
 ![](pic/Peek%202023-07-13%2019-16.gif)
