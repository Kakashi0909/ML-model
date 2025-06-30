# 🔧 Installing C++ Compiler in Visual Studio 2022

## 📋 **Step-by-Step Instructions**

### **Step 1: Visual Studio Installer Should Be Open**

The Visual Studio Installer should have opened automatically. If not:
1. **Open Start Menu**
2. **Type:** "Visual Studio Installer"
3. **Click:** "Visual Studio Installer"

### **Step 2: Modify Visual Studio 2022**

1. **Find Visual Studio 2022** in the list
2. **Click the "Modify" button** (not "Launch")
3. **Wait for the installer to load**

### **Step 3: Select C++ Development Workload**

In the installer, you'll see a list of workloads. **Make sure to check:**

✅ **"Desktop development with C++"**

This workload includes:
- **MSVC v143 - VS 2022 C++ x64/x86 compiler** ← This is the C++ compiler!
- Windows 10/11 SDK
- CMake tools for Visual Studio
- Testing tools core features
- C++ AddressSanitizer

### **Step 4: Optional but Recommended**

Also check these if not already selected:
- ✅ **"Git for Windows"** (if not already installed)
- ✅ **"GitHub extensions for Visual Studio"**

### **Step 5: Install**

1. **Click "Modify"** at the bottom right
2. **Wait for installation** (this may take 5-15 minutes)
3. **Don't close the installer** during this process

## ✅ **Verification Steps**

After installation completes:

### **Check 1: Native Tools Command Prompt**
1. **Open Start Menu**
2. **Type:** `x64 Native Tools Command Prompt`
3. **You should see:** "x64 Native Tools Command Prompt for VS 2022"
4. **Click on it** to open

### **Check 2: Test C++ Compiler**
In the Native Tools Command Prompt, run:
```cmd
cl
```
You should see the Microsoft C/C++ compiler version information.

### **Check 3: Test Our Project**
```cmd
cd D:\task
build_and_run.bat
```

## 🔍 **Troubleshooting**

### **If "Desktop development with C++" is not visible:**
- Make sure you clicked "Modify" on Visual Studio 2022
- Try restarting the Visual Studio Installer
- Check if you have enough disk space (at least 5 GB free)

### **If installation fails:**
- **Run as Administrator:** Right-click Visual Studio Installer → "Run as administrator"
- **Check disk space:** Make sure you have at least 10 GB free
- **Disable antivirus:** Temporarily disable antivirus during installation
- **Check internet:** Make sure you have a stable internet connection

### **If Native Tools Command Prompt doesn't appear:**
- Restart your computer after installation
- Check if the installation completed successfully
- Try running the installer again

## 📊 **What You're Installing**

**C++ Compiler Components:**
- ✅ MSVC v143 compiler (x64 and x86)
- ✅ Windows SDK
- ✅ CMake integration
- ✅ Build tools
- ✅ Debugging tools

**File size:** ~5-10 GB additional
**Installation time:** 5-15 minutes

## 🚀 **After Installation**

Once the C++ compiler is installed:

1. **Open "x64 Native Tools Command Prompt for VS 2022"**
2. **Navigate to your project:**
   ```cmd
   cd D:\task
   ```
3. **Run the build script:**
   ```cmd
   build_and_run.bat
   ```

## 🎯 **Success Indicators**

You'll know the C++ compiler is working when:
- ✅ Native Tools Command Prompt opens without errors
- ✅ `cl` command shows compiler version
- ✅ `build_and_run.bat` starts configuring the project

## 📞 **Need Help?**

If you encounter issues:
1. Make sure you're running the installer as administrator
2. Check that you selected "Desktop development with C++"
3. Ensure you have enough disk space
4. Try restarting your computer after installation

The C++ compiler installation is the key step - once this is done, building the AI Content Detector will be straightforward! 