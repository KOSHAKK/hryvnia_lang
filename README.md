# hryvnia_lang  

## About Project  

**hryvnia_lang** is an experimental programming language built for educational purposes, inspired by the [Kaleidoscope](https://llvm.org/docs/tutorial/) tutorial. The project demonstrates the fundamentals of compiler construction and LLVM IR code generation.  

### Project Goals  
- ✅ Understand how parsing and lexical analysis work  
- ✅ Learn how to build an Abstract Syntax Tree (AST)  
- ✅ Implement interpretation and LLVM-based code generation  
- ✅ Gain hands-on experience creating a programming language from scratch  
- ✅ Learn the basics of compilers and LLVM  

### Features  
- ⚡ Simple syntax inspired by Kaleidoscope  
- ⚡ Basic numeric expressions and function support  
- ⚡ Easily extendable for new constructs and optimizations  

---

## Building  

```bash
# Clone repository with submodules
git clone --recurse-submodules https://github.com/KOSHAKK/hryvnia_lang.git

# Go to project folder
cd hryvnia_lang

# Create build directory
mkdir build
cd build

# Generate build files
cmake ..

# Build the project
cmake --build .
```
