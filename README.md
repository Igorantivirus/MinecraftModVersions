<h1 align="center">Mine-Mod-Versions-With-Interface</h1>

[![Русская версия](https://img.shields.io/badge/Русская%20версия-blue)](README_RU.md)

<h2>Description</h2>

A console utility for easy switching between Minecraft mods.

[🔗 Link to a desktop version](https://github.com/Igorantivirus/Mine-Mods-Versions-With-Interface)

<h2>Dependencies</h2>

Programming language: **C++20**

<h2>Key Features</h2>

* **Multiple mod setups**
    The application allows you to store different mod setups for the same Minecraft version and easily switch between them.
* **Configuration preservation**
    When saving a setup, all auxiliary mod files are preserved, ensuring that mod configurations remain intact.
* **Version and mod management**
    You can remove mods and view available Minecraft versions.

<h2>In Development</h2>

* Support for locating the Minecraft folder on Linux  
  * Currently, `src/ProgrammPathMeneger.hpp` searches for the Windows AppData folder. **Linux support is in progress.**

<h2>Installation and Run</h2>

1. **Clone the repository**
   ```bash
   git clone "https://github.com/Igorantivirus/MinecraftModVersions"
   cd MinecraftModVersions
   ```
2. **Build & Run**
   ```bash
   g++ main.cpp -o mmv
   ./mmv
   ```

<h2>License</h2>

MIT License
