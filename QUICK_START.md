# 🚀 Quick Start Guide - Visual Studio Already Installed!

## ✅ **Good News!**
Visual Studio 2022 is already installed on your system!

## 🔧 **Next Steps:**

### **Option 1: Use Native Tools Command Prompt (Recommended)**

1. **Open Start Menu**
2. **Type:** `x64 Native Tools Command Prompt`
3. **Click:** "x64 Native Tools Command Prompt for VS 2022"
4. **Navigate to your project:**
   ```cmd
   cd D:\task
   ```
5. **Run the build script:**
   ```cmd
   build_and_run.bat
   ```

### **Option 2: Install Missing C++ Components**

If the Native Tools Command Prompt doesn't work:

1. **Open Visual Studio Installer** (search in Start Menu)
2. **Click "Modify"** on Visual Studio 2022
3. **Select "Desktop development with C++"** workload
4. **Click "Modify"** to install missing components
5. **Wait for installation to complete**
6. **Try Option 1 again**

## 🎯 **What to Expect:**

When you run `build_and_run.bat`, you should see:
```
========================================
AI Content Detector - Build and Run
========================================

Step 1: Creating build directory...
Step 2: Configuring with CMake...
Step 3: Building the project...
Step 4: Testing the build...

========================================
Build successful! AI Detector is ready.
========================================

Testing the help command:
AI Content Detector
Usage:
  ai_detector detect-image <image_path> [model_path]
  ai_detector detect-video <video_path> [model_path]
  ai_detector train <training_data_path> <output_model_path>
  ai_detector help
```

## 🔍 **Troubleshooting:**

### **If you get "CMake not found":**
- Install CMake from: https://cmake.org/download/
- Add to PATH during installation

### **If you get "OpenCV not found":**
- The build script will guide you through installing vcpkg and dependencies

### **If the Native Tools Command Prompt doesn't exist:**
- You need to install the C++ development workload (Option 2 above)

## 🎉 **Success!**

Once the build completes successfully, you can:

```cmd
# Test with an image
.\ai_detector.exe detect-image your_image.jpg

# Test with a video  
.\ai_detector.exe detect-video your_video.mp4

# Get help
.\ai_detector.exe help
```

**You're almost there! Just open the Native Tools Command Prompt and run the build script.** 