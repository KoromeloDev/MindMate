# MindMate

MindMate is an unofficial application for ChatGPT. The app supports chat settings, can send system messages, delete and edit messages.

![Main window](https://github.com/KoromeloDev/MindMate/blob/main/screenshots/MainWindow.png?raw=true)

## How to use

To use the program, you will need an [OpenAI account](https://platform.openai.com/signup) to get an API key and enter it into the settings.

## How to install

### Linux:

You can install MindMate using flatpak. To do this, you can go to [Flathub](https://flathub.org/apps/io.github.koromelodev.mindmate) or install using the terminal:
```bash
flatpak install flathub io.github.koromelodev.mindmate
```

## How to install from source code

<h3 align="center">
  Dependencies
</h3>

<p align="center">
  <img src="https://img.shields.io/badge/qt--base-6.5.0-blue" alt="qt-base 6.5.0"/>
  <img src="https://img.shields.io/badge/qt--svg-6.5.0-blue" alt="qt-svg 6.5.0"/>
  <img src="https://img.shields.io/badge/qt--multimedia-6.5.0-blue" alt="qt-multimedia 6.5.0"/>
  <img src="https://img.shields.io/badge/qt--translations-6.5.0-blue" alt="qt-translations 6.5.0"/>
</p>

### Linux:
```bash
git clone https://github.com/KoromeloDev/MindMate.git ;\
cd MindMate ;\
cmake . -DCMAKE_BUILD_TYPE=MinSizeRel ;\
cmake --build . ;\
sudo cmake --install .
```

### Windows:
```bash
git clone https://github.com/KoromeloDev/MindMate.git &^
cd MindMate &^
cmake . -DCMAKE_BUILD_TYPE=MinSizeRel &^
cmake --build . &^
cmake --install .
```
