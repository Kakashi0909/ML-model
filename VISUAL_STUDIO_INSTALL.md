# 🚀 Installing Visual Studio with C++ Development Tools

## 📥 **Step 1: Download Visual Studio Community**

The download page should have opened automatically. If not, visit:
**https://visualstudio.microsoft.com/vs/community/**

1. Click the **"Download Visual Studio"** button
2. Save the installer (vs_community.exe) to your Downloads folder
3. The file size is approximately 1-2 GB

## 🔧 **Step 2: Run the Installer**

1. **Navigate to your Downloads folder**
2. **Right-click** on `vs_community.exe`
3. **Select "Run as administrator"** (recommended)
4. **Wait for the installer to load**

## ⚙️ **Step 3: Configure Installation**

### **Important: Select the Correct Workload**

When the installer opens, you'll see a list of workloads. **Make sure to select:**

✅ **"Desktop development with C++"**

This workload includes:
- MSVC v143 - VS 2022 C++ x64/x86 compiler
- Windows 10/11 SDK
- CMake tools for Visual Studio
- Testing tools core features
- C++ AddressSanitizer

### **Optional but Recommended:**
- ✅ **"Git for Windows"** (if not already installed)
- ✅ **"GitHub extensions for Visual Studio"**

### **What NOT to select (unless you need them):**
- ❌ .NET desktop development
- ❌ Mobile development
- ❌ Game development
- ❌ Azure development

## 📦 **Step 4: Install**

1. **Click "Install"** at the bottom right
2. **Wait for installation** (this may take 10-30 minutes depending on your internet speed)
3. **Don't close the installer** during this process

## ✅ **Step 5: Verify Installation**

After installation completes:

1. **Click "Launch"** to open Visual Studio
2. **Sign in** with your Microsoft account (optional but recommended)
3. **Choose your settings** (you can use default settings)
4. **Close Visual Studio** (we don't need it open for our project)

## 🔍 **Step 6: Verify C++ Tools**

To verify the installation worked:

1. **Open Start Menu**
2. **Type:** `x64 Native Tools Command Prompt`
3. **You should see:** "x64 Native Tools Command Prompt for VS 2022"
4. **Click on it** to open

If you see this command prompt, the installation was successful!

## 🚀 **Step 7: Continue with AI Detector**

Once Visual Studio is installed:

1. **Open "x64 Native Tools Command Prompt for VS 2022"**
2. **Navigate to your project:**
   ```cmd
   cd D:\task
   ```
3. **Run the build script:**
   ```cmd
   build_and_run.bat
   ```

## 🔧 **Troubleshooting**

### **If you don't see "x64 Native Tools Command Prompt":**
- Make sure you selected "Desktop development with C++" workload
- Try restarting your computer
- Re-run the Visual Studio installer and modify the installation

### **If the installer fails:**
- Make sure you have enough disk space (at least 10 GB free)
- Check your internet connection
- Try running as administrator
- Temporarily disable antivirus software

### **If you get permission errors:**
- Always run the installer as administrator
- Make sure you have admin rights on your computer

## 📊 **Installation Summary**

**What you're installing:**
- ✅ Visual Studio Community 2022
- ✅ C++ compiler and build tools
- ✅ Windows SDK
- ✅ CMake integration
- ✅ Git integration (optional)

**File size:** ~10-15 GB total
**Installation time:** 10-30 minutes
**Required disk space:** ~15 GB

## 🎯 **Next Steps After Installation**

1. **Install CMake** (if not already installed)
2. **Set up vcpkg** for OpenCV and Eigen3
3. **Build the AI Content Detector**

The Visual Studio installation is the most important step - once this is done, the rest will be much easier! 