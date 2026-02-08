# Quick Start Guide for Contributors

**Want to help make native_node market-ready?** This guide shows you exactly what to work on and how to get started.

---

## 🎯 Top Priority Items (Start Here!)

These are the most critical features needed for market readiness. Pick one and dive in!

### 1. JIT Engine Integration (CRITICAL)
**Impact**: ⭐⭐⭐⭐⭐ (Blocks everything)  
**Difficulty**: 🔴 Hard  
**Time**: 6-8 weeks  
**Skills**: C++, LLVM, Compiler Design

**What to do**:
1. Integrate LLVM/ClangREPL into the build system
2. Implement basic JIT compilation pipeline
3. Create preloaded header "universe"
4. Achieve <100ms cold start target

**Getting Started**:
```bash
# 1. Read the architecture docs
cat RESEARCH.md | grep -A 50 "JIT"

# 2. Check the stub implementation
cat src/engine/jit_bootstrap.cpp
cat src/engine/engine.h

# 3. Look at the CMake integration point
grep -A 10 "ENGINE_JIT" CMakeLists.txt

# 4. See the prompt for detailed guidance
cat .github/prompts/implement-jit-backend.prompt.md
```

**Resources**:
- [ClangREPL Documentation](https://clang.llvm.org/docs/ClangREPL.html)
- [LLVM CMake Guide](https://llvm.org/docs/CMake.html)
- See `RESEARCH.md` section on JIT

**Create Issue**: [Template](/.github/ISSUE_TEMPLATE/jit-integration.md)

---

### 2. Complete Security Sandbox (CRITICAL)
**Impact**: ⭐⭐⭐⭐⭐ (Production blocker)  
**Difficulty**: 🟡 Medium-Hard  
**Time**: 4-6 weeks  
**Skills**: C++, Linux Kernel, Security

**What to do**:
1. Extend Landlock RulesetBuilder with full API
2. Implement per-script seccomp policies
3. Add cgroups v2 resource limits
4. Create security testing framework

**Getting Started**:
```bash
# 1. Check current implementation
cat src/sandbox/sandbox.cpp
cat src/sandbox/ruleset.cpp
cat src/sandbox/seccomp.cpp

# 2. Look at the tests
cat tests/landlock_test.cpp
cat tests/seccomp_test.cpp

# 3. See configuration examples
cat config/landlock_policy.conf
cat config/syscalls.conf

# 4. Read the implementation guide
cat .github/prompts/implement-sandbox.prompt.md
```

**Resources**:
- [Landlock Documentation](https://landlock.io/)
- [seccomp-bpf Guide](https://www.kernel.org/doc/html/latest/userspace-api/seccomp_filter.html)
- [cgroups v2 Documentation](https://www.kernel.org/doc/html/latest/admin-guide/cgroup-v2.html)

**Create Issue**: [Template](/.github/ISSUE_TEMPLATE/security-sandbox.md)

---

### 3. MailApp Service (HIGH PRIORITY)
**Impact**: ⭐⭐⭐⭐ (User-facing feature)  
**Difficulty**: 🟢 Medium  
**Time**: 3-4 weeks  
**Skills**: C++, SMTP/IMAP protocols

**What to do**:
1. Choose SMTP library (recommend: libcurl or mailio)
2. Implement send_email() API
3. Add attachment support
4. Create quota tracking for emails
5. Write comprehensive tests

**Getting Started**:
```bash
# 1. Check the services stub
cat src/services/services.h
cat src/services/services.cpp

# 2. Review the design
cat RESEARCH.md | grep -A 30 "Mail"

# 3. See implementation guide
cat .github/prompts/implement-services.prompt.md
```

**Example API Design**:
```cpp
namespace services {
    class MailApp {
    public:
        bool send(const std::string& to, 
                  const std::string& subject,
                  const std::string& body,
                  const std::vector<Attachment>& attachments = {});
        
        bool send_html(const std::string& to,
                      const std::string& subject,
                      const std::string& html_body);
    };
}
```

**Resources**:
- [libcurl SMTP example](https://curl.se/libcurl/c/smtp-mail.html)
- [mailio library](https://github.com/karastojko/mailio)

**Create Issue**: [Template](/.github/ISSUE_TEMPLATE/mailapp-service.md)

---

### 4. Web Framework Integration (HIGH PRIORITY)
**Impact**: ⭐⭐⭐⭐ (User-facing feature)  
**Difficulty**: 🟡 Medium-Hard  
**Time**: 5-6 weeks  
**Skills**: C++, Web Frameworks, HTTP

**What to do**:
1. Integrate Drogon or Oat++ framework
2. Implement request dispatcher
3. Create doGet/doPost handler system
4. Add session management
5. Implement HtmlService templating

**Getting Started**:
```bash
# 1. Check current web server
cat src/web/simple_http.cpp
cat src/web/static_server.cpp

# 2. Review design requirements
cat RESEARCH.md | grep -A 40 "Web"

# 3. See implementation guide
cat .github/prompts/web-framework-and-triggers.prompt.md
```

**Resources**:
- [Drogon Documentation](https://drogon.org/)
- [Oat++ Documentation](https://oatpp.io/)

**Create Issue**: [Template](/.github/ISSUE_TEMPLATE/web-framework.md)

---

## 🛠 Good First Issues (Easier Entry Points)

These are smaller, self-contained tasks perfect for first-time contributors:

### 5. SQLite Connection Pooling Enhancement
**Difficulty**: 🟢 Easy-Medium  
**Time**: 1-2 weeks  
**Skills**: C++, SQLite

**What to do**:
- Optimize connection pool for concurrent access
- Add prepared statement caching
- Implement connection timeout handling
- Write performance tests

```bash
cat src/services/sqlite_pool.cpp
cat tests/sqlite_pool_test.cpp
```

---

### 6. PropertyStore API Implementation
**Difficulty**: 🟢 Easy  
**Time**: 1 week  
**Skills**: C++, SQLite

**What to do**:
- Design simple key-value API (get/set/delete)
- Implement using SQLite backend
- Add per-script isolation
- Write unit tests

```bash
# Start from scratch in src/services/property_store.cpp
```

---

### 7. Logging System Enhancement
**Difficulty**: 🟢 Easy  
**Time**: 1 week  
**Skills**: C++

**What to do**:
- Integrate spdlog or similar library
- Add structured logging
- Implement log levels
- Add log rotation
- Write logging guide

---

### 8. Health Check Endpoints
**Difficulty**: 🟢 Easy  
**Time**: 3-5 days  
**Skills**: C++, HTTP

**What to do**:
- Implement `/health` endpoint
- Add `/readiness` probe
- Create system metrics endpoint
- Write tests

```bash
# Current status endpoint:
cat src/main.cpp | grep -A 20 "/api/status"
```

---

### 9. Binary Size Optimization
**Difficulty**: 🟢 Easy-Medium  
**Time**: 1 week  
**Skills**: CMake, Linker optimization

**What to do**:
- Enable LTO (Link Time Optimization)
- Strip unused symbols
- Optimize LLVM build configuration
- Measure and document size improvements

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=MinSizeRel -DMUSL_STATIC=ON
```

---

### 10. Admin UI Enhancement
**Difficulty**: 🟢 Easy-Medium  
**Time**: 2 weeks  
**Skills**: HTML/CSS/JavaScript

**What to do**:
- Enhance current placeholder UI
- Add script management interface
- Create metrics dashboard
- Implement log viewer

```bash
cat src/web/ui/index.html
```

---

## 📋 How to Contribute

### Step 1: Set Up Development Environment

```bash
# Clone the repository
git clone https://github.com/tayyebi/native_node.git
cd native_node

# Use Docker dev environment (recommended)
./scripts/dev_setup.sh build
./scripts/dev_setup.sh shell

# Or local build
cmake -S . -B build -G Ninja
cmake --build build
```

### Step 2: Pick an Issue

1. Check [open issues](https://github.com/tayyebi/native_node/issues)
2. Look for `good-first-issue` or `help-wanted` labels
3. Comment on the issue to claim it
4. Or create a new issue for something from this guide

### Step 3: Develop & Test

```bash
# Create a feature branch
git checkout -b feat/my-feature

# Make your changes
# ...

# Build and test
cmake --build build
ctest --test-dir build

# Run specific tests
./build/landlock_test
./tests/api_status_test.sh
```

### Step 4: Submit Pull Request

```bash
# Commit your changes
git add .
git commit -m "feat: implement feature X"

# Push to your fork
git push origin feat/my-feature

# Open PR on GitHub
```

**PR Requirements**:
- ✅ Code compiles without warnings
- ✅ Tests pass (existing and new)
- ✅ Documentation updated
- ✅ Follows project code style
- ✅ Includes test coverage

---

## 📚 Essential Reading

Before diving in, read these documents:

1. **[README.md](README.md)** - Project overview
2. **[RESEARCH.md](RESEARCH.md)** - Architecture and design
3. **[REPORT.md](REPORT.md)** - Current status and completed work
4. **[MARKET_READINESS.md](MARKET_READINESS.md)** - What's needed for production
5. **[ROADMAP.md](ROADMAP.md)** - Detailed development plan
6. **[CONTRIBUTING.md](CONTRIBUTING.md)** - Contribution guidelines

---

## 🎓 Required Skills by Feature

### JIT Engine
- **Essential**: C++17/20, LLVM IR, Compiler basics
- **Helpful**: ClangREPL experience, JIT concepts
- **Learning**: [LLVM Tutorial](https://llvm.org/docs/tutorial/)

### Security Sandbox
- **Essential**: C++, Linux system calls, Kernel APIs
- **Helpful**: Security experience, seccomp, Landlock
- **Learning**: [Linux Security Modules](https://www.kernel.org/doc/html/latest/security/)

### Services (Mail, DB)
- **Essential**: C++, SMTP/IMAP basics, SQLite
- **Helpful**: Email protocols, Database optimization
- **Learning**: [SQLite Documentation](https://sqlite.org/docs.html)

### Web Framework
- **Essential**: C++, HTTP protocol, Web frameworks
- **Helpful**: Async I/O, Template engines
- **Learning**: [Drogon Tutorial](https://github.com/drogonframework/drogon/wiki)

---

## 💬 Getting Help

- **GitHub Discussions**: Ask questions, share ideas
- **GitHub Issues**: Report bugs, request features
- **Code Comments**: Ask questions in PR reviews
- **Documentation**: Check the `/docs` folder (coming soon)

---

## 🏆 Recognition

Contributors will be:
- ✨ Listed in CONTRIBUTORS.md
- 🎖 Recognized in release notes
- 📢 Mentioned in project announcements
- 🎁 Eligible for contributor rewards (coming soon)

---

## 📊 Project Status Dashboard

Current completion by area:

```
Project Overall:     ████████░░░░░░░░░░░░ 20%

JIT Engine:          ██░░░░░░░░░░░░░░░░░░ 5%
Security Sandbox:    ████████░░░░░░░░░░░░ 40%
Core Services:       █████░░░░░░░░░░░░░░░ 25%
Web Framework:       ███░░░░░░░░░░░░░░░░░ 15%
Triggers:            ░░░░░░░░░░░░░░░░░░░░ 0%
Library System:      ░░░░░░░░░░░░░░░░░░░░ 0%
Admin UI:            ███░░░░░░░░░░░░░░░░░ 15%
Documentation:       ████████████░░░░░░░░ 60%
Testing:             ████░░░░░░░░░░░░░░░░ 20%
```

**Next Major Milestone**: M1 - JIT Foundation (Month 1-2)

---

## 🚀 Let's Build Something Amazing!

native_node has the potential to become the premier high-performance, secure, self-hosted serverless platform. Your contributions will help make that vision a reality.

**Ready to start?** Pick an issue from above and let's get coding! 💻

---

**Questions?** Open a [discussion](https://github.com/tayyebi/native_node/discussions) or issue.

**Want to chat?** Tag @maintainers in an issue.

---

*Last Updated: February 8, 2026*
