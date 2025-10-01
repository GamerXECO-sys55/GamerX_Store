<div align="center">

# 🎮 GamerX Store

### *The Ultimate Application Store for GamerX Linux*

[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)
[![GTK4](https://img.shields.io/badge/GTK-4.0-brightgreen.svg)](https://gtk.org/)
[![Flatpak](https://img.shields.io/badge/Flatpak-Ready-orange.svg)](https://flatpak.org/)
[![Version](https://img.shields.io/badge/Version-0.5.3-informational.svg)](https://github.com/GamerXECO-sys55/GamerX_Store/releases)

<img src="data/icons/hicolor/scalable/apps/io.github.gamerx.GamerXStore.svg" width="200" height="200" alt="GamerX Store Logo" />

</div>

<div align="center">
<img height="512" alt="Image" src="https://github.com/user-attachments/assets/6e2a3f5b-1a92-47ce-89b4-61864a452fd5" />

<div align="center">
<img height="512" alt="Image" src="https://github.com/user-attachments/assets/0a149911-7edb-48c4-84e7-d4e64be80c0d" />
</div>

## 🚀 **About**

**GamerX Store** is the official application store for **GamerX Linux**, designed with gamers and power users in mind. Built on cutting-edge **GTK4** technology, it delivers a modern, smooth, and beautiful interface for discovering and installing applications from Flatpak repositories.

### ✨ **Features**
- 🎯 **Modern UI/UX** - Sleek interface with smooth animations
- ⚡ **Lightning Fast** - Highly multi-threaded architecture  
- 🔄 **Queue Management** - Download multiple apps simultaneously
- 🎮 **Gaming Focus** - Optimized for gaming applications
- 🌙 **Dark Theme** - Beautiful dark theme with light option
- 🔍 **Smart Search** - Integrated GNOME Shell search provider

---

## 🛠️ **Installation**

```bash
# Clone the repository
git clone https://github.com/GamerXECO-sys55/GamerX_Store.git
cd GamerX_Store/gamerx-store

# Build and install
meson setup build --prefix=/usr/local
ninja -C build
sudo ninja -C build install

# Launch GamerX Store
gamerx-store window --auto-service
```

## 👨‍💻 **Developer**

<div align="center">

### **Mangesh Choudhary**
*Lead Developer & Creator of GamerX Linux*

[![GitHub](https://img.shields.io/badge/GitHub-GamerXECO--sys55-181717?style=for-the-badge&logo=github)](https://github.com/GamerXECO-sys55)

</div>

---

## 🎮 **GamerX Linux Integration**

GamerX Store is specifically designed for **GamerX Linux** - a high-performance gaming distribution:
- 🎯 **Optimized for Gaming** - Pre-configured for gaming performance
- 🔧 **Custom Repository** - Access to GamerX-specific packages  
- 🌟 **Hyprland Integration** - Seamless WM integration
- 📦 **Dual Sources** - Flathub + GamerX packages

## 🤝 **Contributing**

We welcome contributions! Here's how you can help:

- 🐛 **Bug Reports** - Found an issue? Let us know!
- 💡 **Feature Requests** - Have ideas? We'd love to hear them!
- 🔧 **Code Contributions** - Submit PRs for fixes and features
- 🎨 **UI/UX Design** - Make it even more beautiful
- 🌍 **Translations** - Help us reach more users

## 🔧 **Dependencies**

| Package | Version | Purpose |
|---------|---------|---------|
| **GTK4** | ≥ 4.0 | Modern UI framework |
| **libadwaita** | ≥ 1.7 | GNOME styling |
| **libdex** | ≥ 0.11.1 | Async operations |
| **flatpak** | ≥ 1.9 | Package management |
| **appstream** | ≥ 1.0 | Application metadata |
| **libsoup** | ≥ 3.6.0 | HTTP operations |

---

## 📄 **License**

This project is licensed under the **GNU General Public License v3.0**.

```
Copyright © 2025 Mangesh Choudhary
GamerX Store - Official Application Store for GamerX Linux
```

---

## 🙏 **Acknowledgments**

- **GNOME Team** - For GTK4 and libadwaita frameworks
- **Flatpak Community** - For universal package management
- **GamerX Linux Community** - For testing and feedback
