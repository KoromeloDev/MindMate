# MindMate
MindMate is an unofficial chat app with ChatGPT

To use the program, you will need an [OpenAI account](https://platform.openai.com/signup) to get an API key and enter it into the settings.

![Main window](https://github.com/KoromeloDev/MindMate/blob/main/screenshots/MainWindow.png?raw=true)

## Install from source code for linux

<h3 align="center">
  Dependencies
</h3>

<p align="center">
  <img src="https://img.shields.io/badge/qt--base-6.5.0-blue" alt="qt-base 6.5.0"/>
  <img src="https://img.shields.io/badge/qt--svg-6.5.0-blue" alt="qt-svg 6.5.0"/>
  <img src="https://img.shields.io/badge/qt--multimedia-6.5.0-blue" alt="qt-multimedia 6.5.0"/>
  <img src="https://img.shields.io/badge/qt--translations-6.5.0-blue" alt="qt-translations 6.5.0"/>
</p>

```bash
git clone https://github.com/KoromeloDev/MindMate.git ;\
cd MindMate ;\
cmake . -DCMAKE_BUILD_TYPE=MinSizeRel;\
cmake --build . ;\
sudo cmake --install .
```
