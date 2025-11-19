# 🚀 ESP32 + ILI9341 Touchscreen Stepper Motor Controller  
**Take control at your fingertips! Control your stepper motor with a simple touch.**

## 🧠 About the Project  
This project showcases how to control a **28BYJ-48 stepper motor** using an **ESP32** and an **ILI9341 touchscreen display**. Through the touchscreen interface, you can easily set the motor's **direction**, **speed**, and **rotation count** — no buttons or knobs needed!

The code and schematic are based on my YouTube tutorial. If you're new to this, make sure to watch the video before diving into the code.

🎥 **Watch the full tutorial here: V1**  

[![V2](https://img.youtube.com/vi/Szm2ziKMi20/0.jpg)](https://youtu.be/Szm2ziKMi20)

🎥 **Watch the full tutorial here: V2** 
[![V2](https://img.youtube.com/vi/vlZQBQYyaFM/0.jpg)](https://youtu.be/vlZQBQYyaFM)

## ✨ Features

- 📱 Touchscreen interface for intuitive control  
- 🔁 Set rotation direction (CW/CCW)  
- 🐢🐇 Adjust motor speed  
- 🔢 Choose number of rotations  
- 🔧 Easy to customize and extend  

---

## 🧰 Components Used

- ESP32 Development Board  
- ILI9341 2.8” Touchscreen Display  
- 28BYJ-48 Stepper Motor  
- ULN2003 Motor Driver Board  
- Jumper Wires  
- Breadboard (optional)

---

## 📚 Required Libraries

- [TFT_eSPI](https://github.com/Bodmer/TFT_eSPI)  
- [AccelStepper (SwissByte fork)](https://github.com/swissbyte/AccelStepper)

## ⚙️ Getting Started

1. Clone this repository.  
2. Install required libraries via the Arduino Library Manager.  
3. Configure `User_Setup.h` inside `TFT_eSPI` to match your ESP32 and display pins.  
4. Open `esp32_28_Byj_Stepper_ILI9341_Turn_Control_V2.ino` in Arduino IDE.  
5. Upload the sketch to your ESP32.  
6. Wire up your components, power the board, and start controlling!

## 🎮 Other Projects & Games

- 🧩 [3D Models on Cults3D](https://cults3d.com/en/users/DsnDesign/3d-models)  
- 🕹️ [Air Hockey](https://play.google.com/store/apps/details?id=com.DsnMechanics.AirHockey)  
- 🖱️ [Click Challenge](https://play.google.com/store/apps/details?id=com.DsNMechanics.ClickChallenge)  
- 🔺 [Flying Triangles](https://play.google.com/store/apps/details?id=com.DsnMechanics.Triangle)  
- 🏙️ [SkyScraper](https://play.google.com/store/apps/details?id=com.DsnMechanics.SkyScraper)


## 📬 Contact & Follow

- YouTube: [DSN Mechanics](https://www.youtube.com/dsnmechanics/?sub_confirmation=1)  
- Instagram: [@dsnmechanics](https://www.instagram.com/dsnmechanics)  
- Email: dsnmechanic@gmail.com

## ⭐ Contribute

If you find this project useful, please **star** the repo and feel free to fork it.  
For feedback or questions, open an **issue** or contact me directly.

---

**Now, let’s make that stepper motor move — with style and precision!** 🎯⚙️
