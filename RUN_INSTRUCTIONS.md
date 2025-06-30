# 🚀 How to Run the AI Content Detector

## 📋 **Prerequisites Installation**

### **Step 1: Install Visual Studio Community**
1. Download from: https://visualstudio.microsoft.com/vs/community/
2. During installation, select **"Desktop development with C++"** workload
3. This includes the C++ compiler and build tools

### **Step 2: Install CMake**
1. Download from: https://cmake.org/download/
2. Choose "Windows x64 Installer"
3. During installation, select "Add CMake to the system PATH"

### **Step 3: Install vcpkg and Dependencies**
Open **Command Prompt as Administrator** and run:

```cmd
cd D:\
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
.\bootstrap-vcpkg.bat
.\vcpkg install opencv4 eigen3
```

## 🔨 **Building the Project**

### **Method 1: Using the Provided Script**
1. Open **"x64 Native Tools Command Prompt for VS"** (from Start Menu)
2. Navigate to the project:
   ```cmd
   cd D:\task
   ```
3. Run the build script:
   ```cmd
   build_and_run.bat
   ```

### **Method 2: Manual Build**
1. Open **"x64 Native Tools Command Prompt for VS"**
2. Navigate to the project:
   ```cmd
   cd D:\task
   ```
3. Create build directory:
   ```cmd
   mkdir build
   cd build
   ```
4. Configure with CMake:
   ```cmd
   cmake .. -DCMAKE_TOOLCHAIN_FILE=D:/vcpkg/scripts/buildsystems/vcpkg.cmake
   ```
5. Build the project:
   ```cmd
   cmake --build . --config Release
   ```

## 🎯 **Running the AI Detector**

### **Test the Installation**
```cmd
.\ai_detector.exe help
```

### **Detect AI Content in Images**
```cmd
.\ai_detector.exe detect-image path\to\your\image.jpg
```

### **Detect AI Content in Videos**
```cmd
.\ai_detector.exe detect-video path\to\your\video.mp4
```

### **Train a Custom Model**
```cmd
.\ai_detector.exe train path\to\training\data\ output_model.bin
```

### **Use a Specific Model**
```cmd
.\ai_detector.exe detect-image image.jpg my_model.bin
```

## 📊 **Expected Output**

### **Image Detection Example:**
```
Analyzing image: sample.jpg
AI Detection Confidence: 75.2%
Result: Likely AI-generated content
```

### **Video Detection Example:**
```
Analyzing video: sample.mp4
AI Detection Confidence: 68.9%
Result: Possibly AI-generated content
```

### **Help Command:**
```
AI Content Detector
Usage:
  ai_detector detect-image <image_path> [model_path]
  ai_detector detect-video <video_path> [model_path]
  ai_detector train <training_data_path> <output_model_path>
  ai_detector help
```

## 🔧 **Troubleshooting**

### **Common Issues:**

#### **1. "CMake not found"**
- Install CMake from https://cmake.org/download/
- Add to PATH during installation

#### **2. "Visual Studio not found"**
- Install Visual Studio Community with "Desktop development with C++"
- Make sure to select the C++ workload during installation

#### **3. "OpenCV not found"**
- Make sure vcpkg is properly installed
- Run: `.\vcpkg install opencv4 eigen3`

#### **4. "Permission denied"**
- Run Command Prompt as Administrator
- Make sure you have write permissions to the project directory

#### **5. "Build failed"**
- Check that you're using "x64 Native Tools Command Prompt for VS"
- Verify all dependencies are installed
- Check the error messages for specific issues

### **Alternative: Use PowerShell Script**
If you prefer PowerShell, you can use the provided script:
```powershell
.\install_dependencies.ps1
```

## 📁 **Project Structure**
```
D:\task\
├── build_and_run.bat          # Build script
├── install_dependencies.ps1   # Installation helper
├── CMakeLists.txt            # Build configuration
├── src\                      # Source files
├── include\                  # Header files
├── examples\                 # Usage examples
└── README.md                 # Documentation
```

## 🎉 **Success Indicators**

You'll know everything is working when:
1. ✅ CMake configuration succeeds
2. ✅ Build completes without errors
3. ✅ `ai_detector.exe` is created in the build directory
4. ✅ `.\ai_detector.exe help` shows the usage information

## 🚀 **Next Steps**

After successful installation:
1. **Test with sample images/videos**
2. **Train your own model** with labeled data
3. **Customize the features** for your specific use case
4. **Integrate into your applications**

## 📞 **Need Help?**

If you encounter issues:
1. Check the troubleshooting section above
2. Verify all dependencies are properly installed
3. Make sure you're using the correct command prompt
4. Check the error messages for specific guidance

The project is ready to run once you have the development environment set up! 