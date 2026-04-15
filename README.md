# GraphicsProgrammingExtension
An experimental plugin to play around with the Unreal Engine rendering pipeline.

---

## 测试添加Global Shader
1. 在Shaders文件夹中添加了一个非常简单的usf，这个Shader可以直接在C++中被调用
2. 在MyGlobalShader.h中新增了两个FGlobalShader的子类，分别对应VS和PS
3. 在MyGlobalShader.cpp中将新类与usf中的Shader入口绑定，并向PS中传递了参数，还设置了PSO用来渲染一个全屏Shader
4. 在插件模块中通过RDG添加了一个Pass，并在插件加载时将其注册在了OverlayRenderDelegate中
5. 添加了一个CVar用于控制该Overlay Pass的渲染颜色和透明度

## 替换Shader传参的方式
1. 在MyGlobalShader.h中改用更主流的SHADER_USE_PARAMETER_STRUCT宏来绑定Shader中的参数
2. 在MyGlobalShader.cpp中改用SetShaderParameters()传参
3. 删除旧版传参方式及部分注释，如需参考可查看此[Commit](https://github.com/YE-ZA/GraphicsProgrammingExtension/commit/f6e50cf17b8d2a57058be25e77fd105a533f6795)