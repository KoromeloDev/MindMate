# MindMate

<p align="center">
  <img alt="Flathub Downloads" src="https://img.shields.io/flathub/downloads/io.github.koromelodev.mindmate">
  <img alt="Flathub Version" src="https://img.shields.io/flathub/v/io.github.koromelodev.mindmate">
</p>

### ❄️The project is frozen. I don't see much point in supporting the application as all the functionality is already available on the site❄️

MindMate is an unofficial application for ChatGPT. The app supports chat settings, can send system messages, delete and edit messages.

![Main window](https://github.com/KoromeloDev/MindMate/blob/main/screenshots/MainWindow.png?raw=true)

## How to install
### Linux:
<a href="https://flathub.org/apps/io.github.koromelodev.mindmate"><img width="240" alt="Download on Flathub" src="https://dl.flathub.org/assets/badges/flathub-badge-en.svg"/></a>

### Windows:
You can download the archive with the program in the [GitHub releases](https://github.com/KoromeloDev/MindMate/releases/latest) or build it yourself

## How to install from source code
<h3 align="center">
  Dependencies
</h3>
<p align="center">
  <img src="https://img.shields.io/badge/qt--base-6.7.3-blue" alt="qt-base 6.7.3"/>
  <img src="https://img.shields.io/badge/qt--svg-6.7.3-blue" alt="qt-svg 6.7.3"/>
  <img src="https://img.shields.io/badge/qt--multimedia-6.7.3-blue" alt="qt-multimedia 6.7.3"/>
  <img src="https://img.shields.io/badge/qt--translations-6.7.3-blue" alt="qt-translations 6.7.3"/>
</p>

<h3 align="center">
  Code
</h3>
<p align="center">
  <a href="https://www.codefactor.io/repository/github/koromelodev/mindmate"><img src="https://www.codefactor.io/repository/github/koromelodev/mindmate/badge" alt="CodeFactor" /></a>
  <img alt="GitHub issues" src="https://img.shields.io/github/issues/koromeloDev/MindMate">
  <img alt="Unit Testing" src="https://github.com/KoromeloDev/MindMate/actions/workflows/test.yml/badge.svg?event=push">
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

## How to use
To use the program, you will need an [OpenAI account](https://platform.openai.com/signup) to get an API key and enter it into the settings.

### Configuration
To configure the application, you can go to global settings. These settings affect the entire application.

![GeneralSettings](https://github.com/KoromeloDev/MindMate/blob/main/screenshots/GlobalSettingsWindow1.png?raw=true)
![ChatSettings](https://github.com/KoromeloDev/MindMate/blob/main/screenshots/GlobalSettingsWindow2.png?raw=true)

You can also configure each chat individually.

![ChatSettings](https://github.com/KoromeloDev/MindMate/blob/main/screenshots/ChatSettingsWindow.png?raw=true)

### Message role
Each message has a role. There are three roles in total:
1. User - a regular user message
2. System - used to guide the behavior of your model throughout the conversation
3. Assistant - ChatGPT returns a response using this role

![Role](https://github.com/KoromeloDev/MindMate/blob/main/screenshots/SystemMessageDescription.png?raw=true)
