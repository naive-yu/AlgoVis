# TSP Solver Project

## English

### Project Overview

This project is a comprehensive **algorithm visualization and demonstration platform** developed using C++ and the Qt framework. It not only provides an intuitive graphical user interface (GUI) for visualizing and solving the Traveling Salesman Problem (TSP) with state-of-the-art optimization algorithms, but also includes classic algorithmic problems such as the **Eight Queens Problem**, **Maze Pathfinding**, and even the **2048 Game**. The main features include:

- **Traveling Salesman Problem (TSP)**
   - **Ant Colony Optimization (ACO)**
   - **Genetic Algorithm (GA)**
   - **Particle Swarm Optimization (PSO)**
- **Eight Queens Problem**
- **Maze Pathfinding**
- **2048 Game**

### Key Features
- **Rich Algorithm Demonstrations**: Includes TSP, Eight Queens, Maze Pathfinding, 2048 and more, each with interactive visualization.
- **Real-Time Algorithm Visualization**: Dynamically displays the routes, chessboard, maze, or game state, offering insights into algorithm behavior.
- **Customizable Parameters**: Enables users to fine-tune algorithm parameters and adjust animation speed for better control.
- **Pause and Resume Functionality**: Allows users to pause and resume algorithm execution seamlessly.
- **Multi-Threaded Execution**: Ensures smooth and responsive UI interactions while computationally intensive algorithms run in the background.

### Build and Run Instructions
1. **Prerequisites**:
   - Qt 5 or 6
   - CMake
   - A C++ compiler (e.g., GCC, Clang, or MSVC)
2. **Build Steps**:
   ```bash
   mkdir build
   cd build
   cmake ..
   cmake --build .
   ```
3. **Run**:
   - Execute the generated binary (e.g., `TSP.exe` on Windows).

### Project Structure
- `src/`: Contains the core source code.
- `config/`: Includes configuration files for algorithm parameters.
- `ts/`: Stores translation files for internationalization.
- `build/`: Directory for build artifacts.

### License
This project is licensed under the MIT License.

---


## 中文

### 项目概述
本项目是一个使用 C++ 和 Qt 框架开发的**算法可视化与演示平台**。不仅包含旅行商问题 (TSP) 的多种优化算法演示，还集成了**八皇后问题**、**迷宫寻路问题**、**2048 游戏**等经典算法与趣味问题的可视化。主要功能包括：

- **旅行商问题 (TSP)**
   - **蚁群优化算法 (ACO)**
   - **遗传算法 (GA)**
   - **粒子群优化算法 (PSO)**
- **八皇后问题**
- **迷宫寻路问题**
- **2048 游戏**

### 核心功能
- **丰富的算法演示**：集成 TSP、八皇后、迷宫寻路、2048 等多种算法与趣味问题，均支持交互式可视化。
- **实时算法可视化**：动态显示算法路径、棋盘、迷宫或游戏状态，帮助用户深入理解算法行为。
- **参数自定义**：允许用户微调算法参数并调整动画速度，以获得更好的控制。
- **暂停与继续功能**：支持算法执行的无缝暂停与继续。
- **多线程执行**：确保计算密集型算法在后台运行时，用户界面依然流畅响应。

### 构建与运行说明
1. **环境依赖**：
   - Qt 5 或 6
   - CMake
   - C++ 编译器（如 GCC、Clang 或 MSVC）
2. **构建步骤**：
   ```bash
   mkdir build
   cd build
   cmake ..
   cmake --build .
   ```
3. **运行**：
   - 执行生成的二进制文件（如 Windows 上的 `TSP.exe`）。

### 项目结构
- `src/`：包含核心源代码。
- `config/`：包含算法参数的配置文件。
- `ts/`：存储用于国际化的翻译文件。
- `build/`：构建生成的文件目录。

### 许可证
本项目使用 MIT 许可证。