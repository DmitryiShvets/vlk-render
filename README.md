# vlk-render

**vlk-render** — это простой рендеринг-проект на основе Vulkan, демонстрирующий работу с многообъектной сценой, текстурами, камерами и интерфейсом через Dear ImGui. Проект написан на C++ и предназначен для обучения и экспериментов с низкоуровневым графическим API Vulkan.

## 📆 Возможности

- ✅ Отрисовка 3D-сцены с несколькими объектами
- ✅ Поддержка нескольких текстур (`VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE`)
- ✅ Камера с управлением по плоскости XZ
- ✅ Перспективная и ортографическая проекция
- ✅ Импорт кастомных моделей (`.obj`)
- ✅ Интеграция Dear ImGui с Vulkan и GLFW
- ✅ Использование Uniform Buffer Objects (UBO)

## 🧹 Структура проекта

- `vlk_app.h / TestApp` — основной цикл, инициализация Vulkan, логика ImGui, управление сценой и камерой
- `simple_render_system.h` — отрисовка объектов
- `camera.h` — работа с матрицами вида и проекции
- `user_input_controller.h` — перемещение камеры через клавиатуру
- `buffer.h`, `texture.h` — абстракции над Vulkan-буферами и текстурами
- `imgui_impl_glfw.h`, `imgui_impl_vulkan.h` — реализация ImGui для Vulkan

## 🧱 Зависимости

- [Vulkan SDK](https://vulkan.lunarg.com/)
- [GLFW](https://www.glfw.org/)
- [GLM](https://github.com/g-truc/glm)
- [Dear ImGui](https://github.com/ocornut/imgui)
- [stb\_image](https://github.com/nothings/stb) — для загрузки текстур

## 🛠 Сборка

```bash
git clone https://github.com/yourname/vlk-render.git
cd vlk-render
mkdir build && cd build
cmake ..
make
```

> ⚠️ Убедитесь, что пути к ресурсам (текстурам и моделям) актуальны. В текущей версии они заданы как `../../../../test2/res/...`.

## 🎮 Управление

- `W`, `A`, `S`, `D` — перемещение камеры по плоскости XZ
- Интерфейс ImGui отображается поверх сцены

## 🖼 Скриншоты

<img width="794" height="620" alt="image" src="https://github.com/user-attachments/assets/4476fade-5c40-402e-aa7a-69f6739bde1c" />

<img width="794" height="625" alt="image" src="https://github.com/user-attachments/assets/9097b3e1-263d-4ddd-9f83-bcd0d1ebab2c" />

<img width="793" height="619" alt="image" src="https://github.com/user-attachments/assets/592bb7f3-ab2e-4fb3-9801-af843b1e8727" />


## 🧪 Примечания

**🔧 Основано на:** Vulkan, GLFW, ImGui\
**🎓 Назначение:** обучение, эксперименты, шаблон для рендеринг-движка\
**🧠 Язык:** C++

