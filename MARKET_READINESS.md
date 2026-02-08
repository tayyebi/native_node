# Market Readiness Assessment for native_node

**Last Updated**: February 8, 2026  
**Status**: Early Development / Pre-Alpha  
**Target Market Launch**: Q4 2026 - Q1 2027

---

## Executive Summary

The **native_node** project aims to deliver a production-grade C++ scripting engine and serverless runtime that combines the developer experience of platforms like Google Apps Script with the performance and control of native C++. This document outlines what remains to be implemented for the project to achieve market readiness.

### Current Maturity Level: **20-25%**

The project has established strong foundations:
- ✅ Architecture and design documentation
- ✅ Basic build system and CI/CD
- ✅ Security framework scaffolding (Landlock/seccomp)
- ✅ Initial SQLite integration
- ✅ Docker development environment
- ✅ Project governance (CONTRIBUTING, LICENSE)

However, critical production features are still in early stages or not yet implemented.

---

## Critical Path to Market Readiness

### Priority 1: Core Functionality (3-4 months)

These features are **essential** and block all other development:

#### 1.1 JIT Engine Implementation
**Status**: Stub only  
**Effort**: 6-8 weeks  
**Blocking**: Everything

- [ ] Integrate LLVM/ClangREPL (16+) via FetchContent or submodule
- [ ] Implement JIT session management and code compilation
- [ ] Create preloaded runtime headers ("universe")
- [ ] Implement script execution context with proper isolation
- [ ] Add compilation error handling and reporting
- [ ] Achieve <100ms cold start target through optimization
- [ ] Add basic REPL interface for interactive development

**Acceptance Criteria**:
- Can compile and execute a C++ script in <100ms
- Scripts can access pre-loaded standard library headers
- Proper error messages for compilation failures
- Memory safety guarantees for JIT-compiled code

#### 1.2 Complete Security Sandbox
**Status**: Partial implementation  
**Effort**: 4-6 weeks  
**Blocking**: Production deployment

- [ ] Complete Landlock ruleset builder with dynamic path rules
- [ ] Implement per-script seccomp policy application
- [ ] Add cgroups v2 integration (CPU, memory, PID limits)
- [ ] Create security policy validation and testing framework
- [ ] Document security model and threat mitigation strategies
- [ ] Add runtime security monitoring and alerts

**Acceptance Criteria**:
- Scripts cannot access unauthorized filesystem paths
- Strict syscall filtering prevents privilege escalation
- Resource limits prevent DoS attacks
- Security audit shows no critical vulnerabilities

#### 1.3 Essential Services
**Status**: Basic stubs  
**Effort**: 4-5 weeks  
**Blocking**: Script functionality

- [ ] MailApp: SMTP/IMAP client implementation
- [ ] SQLite: Connection pooling with WAL mode
- [ ] PropertyStore: Key-value API for script state
- [ ] Quota system: Track and enforce usage limits
- [ ] Service health monitoring and error recovery

**Acceptance Criteria**:
- Can send emails via SMTP with attachments
- SQLite handles concurrent access properly
- PropertyStore provides simple persistence API
- Quotas prevent abuse (emails/day, DB size, etc.)

---

### Priority 2: Production Operations (2-3 months)

Essential for reliable production deployment:

#### 2.1 Observability & Monitoring
**Status**: Minimal  
**Effort**: 3-4 weeks

- [ ] Structured logging framework (spdlog or similar)
- [ ] Metrics collection (Prometheus format)
- [ ] Distributed tracing support
- [ ] Health check and readiness endpoints
- [ ] Resource usage monitoring
- [ ] Alerting integration

#### 2.2 Testing & Quality Assurance
**Status**: Basic smoke tests only  
**Effort**: 4-6 weeks

- [ ] Comprehensive unit test suite (>80% coverage)
- [ ] Integration test framework
- [ ] Performance benchmarks and regression tests
- [ ] Load testing and stress testing
- [ ] Security testing automation (fuzzing, static analysis)
- [ ] CI/CD pipeline improvements

#### 2.3 Binary Distribution
**Status**: Build system ready, not optimized  
**Effort**: 2-3 weeks

- [ ] Static musl binary with LTO optimization
- [ ] Binary size reduction to target (~34MB)
- [ ] Cross-compilation for multiple architectures
- [ ] Container image builds (multi-arch)
- [ ] Installation packages (deb, rpm, etc.)
- [ ] Update and migration tooling

---

### Priority 3: Developer Experience (2-3 months)

Critical for adoption and usability:

#### 3.1 Documentation
**Status**: Architecture docs exist, API docs missing  
**Effort**: 4-5 weeks

- [ ] Complete API reference for script authors
- [ ] Administrator's guide (deployment, security)
- [ ] Tutorial series (beginner to advanced)
- [ ] Migration guides (from GAS, AWS Lambda, etc.)
- [ ] Best practices and patterns documentation
- [ ] Security guidelines for administrators

#### 3.2 Web Framework & Templating
**Status**: Static server only  
**Effort**: 5-6 weeks

- [ ] Integrate Drogon or Oat++ web framework
- [ ] Request dispatcher for doGet/doPost handlers
- [ ] HtmlService templating engine
- [ ] Session management
- [ ] WebSocket support for real-time features
- [ ] API rate limiting and authentication

#### 3.3 Admin Console
**Status**: HTML placeholder  
**Effort**: 4-5 weeks

- [ ] Script management interface (upload, edit, delete)
- [ ] Real-time execution logs viewer
- [ ] Metrics and performance dashboard
- [ ] Trigger scheduling interface
- [ ] Configuration management UI
- [ ] User and permission management

---

### Priority 4: Advanced Features (3-4 months)

Important for feature parity with competitors:

#### 4.1 Triggers & Scheduling
**Status**: Not implemented  
**Effort**: 3-4 weeks

- [ ] libcron integration for cron-style scheduling
- [ ] Event-driven trigger system
- [ ] TriggerBuilder API for script authors
- [ ] Trigger management and monitoring
- [ ] Webhook integration

#### 4.2 Library System
**Status**: Not implemented  
**Effort**: 4-5 weeks

- [ ] Versioned module loader with semantic versioning
- [ ] Library dependency resolution
- [ ] Public/private API distinction
- [ ] Library repository and publishing workflow
- [ ] Documentation generation for libraries

#### 4.3 Enterprise Features
**Status**: Not implemented  
**Effort**: 6-8 weeks

- [ ] Multi-tenancy support
- [ ] Role-based access control (RBAC)
- [ ] Audit logging and compliance reporting
- [ ] Custom authentication providers (LDAP, SAML, OAuth)
- [ ] High availability and clustering support

---

## Detailed Feature Gap Analysis

### Comparison with Google Apps Script

| Feature | Google Apps Script | native_node (current) | Gap Status |
|---------|-------------------|----------------------|------------|
| **Language** | JavaScript | C++ (JIT) | ⚠️ JIT not integrated |
| **Instant Deploy** | Yes | Planned | ❌ Not implemented |
| **Web Endpoints** | doGet/doPost | Planned | ⚠️ Stub only |
| **HTML Templates** | HtmlService | Planned | ❌ Not implemented |
| **Scheduling** | Time/event triggers | Planned | ❌ Not implemented |
| **Email (MailApp)** | Built-in | Planned | ❌ Not implemented |
| **Storage** | PropertiesService | SQLite + PropertyStore | ⚠️ Basic implementation |
| **Sandboxing** | Platform-managed | Landlock + seccomp | ⚠️ Partial |
| **Admin UI** | Full console | Basic placeholder | ⚠️ Needs work |
| **Versioned Libraries** | Yes | Planned | ❌ Not implemented |
| **Quotas** | Built-in | Planned | ❌ Not implemented |
| **Observability** | Built-in logging | Basic | ⚠️ Needs enhancement |

### Comparison with AWS Lambda

| Feature | AWS Lambda | native_node (current) | Gap Status |
|---------|-----------|----------------------|------------|
| **Runtime** | Multiple (Node, Python, etc.) | C++ (JIT) | ⚠️ JIT not integrated |
| **Cold Start** | 100-500ms | Target <100ms | ⚠️ Not measured |
| **Execution Limits** | 15 min, 10GB RAM | Configurable (cgroups) | ⚠️ Partial |
| **Networking** | Full | Restricted (seccomp) | ✅ By design |
| **Deployment** | ZIP/container | Single binary | ✅ Planned |
| **Monitoring** | CloudWatch | Custom | ❌ Basic only |
| **Scaling** | Automatic | Manual (for now) | ❌ Not implemented |
| **Cost Model** | Per-invocation | Self-hosted | ✅ Advantage |

---

## Risk Assessment

### Technical Risks

#### HIGH Risk: JIT Integration Complexity
**Impact**: Project viability  
**Mitigation**:
- Start with minimal LLVM configuration
- Use proven ClangREPL foundation
- Prototype early and validate performance
- Have fallback to interpreted mode if needed

#### HIGH Risk: Security Vulnerabilities
**Impact**: Production readiness blocked  
**Mitigation**:
- Multiple security layers (defense in depth)
- Regular security audits
- Fuzzing and penetration testing
- Bug bounty program before launch

#### MEDIUM Risk: Performance Targets
**Impact**: Market competitiveness  
**Mitigation**:
- Early and continuous benchmarking
- Profile-guided optimization
- Iterative performance improvements
- Clear performance regression detection

#### MEDIUM Risk: Binary Size Bloat
**Impact**: Deployment friction  
**Mitigation**:
- Minimal LLVM build configuration
- Link-time optimization (LTO)
- Strip unused symbols
- Consider optional features as plugins

### Market Risks

#### MEDIUM Risk: Competition from Established Platforms
**Mitigation**:
- Focus on unique value proposition (native performance + self-hosted)
- Target specific use cases (edge computing, IoT, high-performance)
- Build strong community early
- Excellent documentation and developer experience

#### LOW Risk: Limited C++ Adoption
**Mitigation**:
- Emphasize performance benefits
- Provide migration tools from other platforms
- Build comprehensive examples and templates
- Consider multi-language support in future

---

## Resource Requirements

### Engineering Team (Recommended)

For 6-month critical path:
- **2 Senior C++ Engineers**: Core JIT and runtime
- **1 Security Engineer**: Sandbox and security hardening
- **1 DevOps Engineer**: CI/CD, deployment, monitoring
- **1 Technical Writer**: Documentation
- **1 UI/UX Developer**: Admin console

Total: ~5-6 FTE for 6 months

### Infrastructure
- CI/CD pipeline (GitHub Actions or equivalent)
- Test infrastructure (various Linux distributions)
- Performance testing environment
- Security testing tools and services

---

## Go-to-Market Strategy

### Phase 1: Alpha Release (Month 3-4)
**Target Audience**: Early adopters, contributors  
**Features**: Core JIT, basic security, minimal services  
**Goal**: Validate architecture, gather feedback

### Phase 2: Beta Release (Month 6-8)
**Target Audience**: Friendly companies, pilot projects  
**Features**: Complete security, all services, basic UI  
**Goal**: Production validation, bug fixes, documentation

### Phase 3: Public Release (Month 10-12)
**Target Audience**: General market  
**Features**: All priority 1-3 features, polish  
**Goal**: Broad adoption, community building

### Target Use Cases
1. **Edge Computing**: Low-latency processing at CDN edges
2. **IoT Gateways**: Secure script execution on IoT devices
3. **Internal Automation**: Self-hosted alternative to cloud functions
4. **High-Performance APIs**: Native speed for compute-intensive tasks
5. **Secure Multi-Tenancy**: Isolated execution environments

---

## Success Metrics

### Technical Metrics
- ✅ Binary size < 50MB
- ✅ Cold start < 100ms
- ✅ Test coverage > 80%
- ✅ Zero critical security vulnerabilities
- ✅ 99.9% uptime in production deployments
- ✅ Memory usage < 100MB base + script requirements

### Adoption Metrics (12 months post-launch)
- 🎯 1,000+ GitHub stars
- 🎯 100+ production deployments
- 🎯 50+ active contributors
- 🎯 10+ community libraries published
- 🎯 1,000+ scripts deployed

### Business Metrics
- 🎯 5+ enterprise customers
- 🎯 Active community forum (100+ members)
- 🎯 10+ case studies published
- 🎯 Commercial support offerings available

---

## Immediate Next Steps

### Week 1-2: Planning & Setup
1. ✅ Document market readiness gaps (this document)
2. [ ] Create detailed technical roadmap with milestones
3. [ ] Set up project tracking (issues, milestones)
4. [ ] Establish development process and guidelines
5. [ ] Form core development team

### Week 3-6: JIT Foundation
1. [ ] Integrate LLVM/ClangREPL into build system
2. [ ] Implement basic JIT compilation pipeline
3. [ ] Create smoke tests for JIT functionality
4. [ ] Measure and optimize cold start time
5. [ ] Document JIT architecture

### Week 7-10: Security Hardening
1. [ ] Complete Landlock implementation
2. [ ] Implement cgroups resource limits
3. [ ] Add security testing framework
4. [ ] Conduct initial security audit
5. [ ] Document security model

### Week 11-14: Essential Services
1. [ ] Implement MailApp service
2. [ ] Optimize SQLite pooling
3. [ ] Create PropertyStore API
4. [ ] Add quota tracking system
5. [ ] Write service documentation

---

## Conclusion

The **native_node** project has a solid foundation and clear vision. To achieve market readiness, the project needs approximately **6-12 months of focused development** on core features, security, and production operations.

### Critical Success Factors:
1. **JIT Integration**: Must be completed early and validated
2. **Security**: No compromises on sandbox implementation
3. **Performance**: Must meet <100ms cold start target
4. **Documentation**: Essential for adoption
5. **Community**: Build early and sustain momentum

### Competitive Advantages:
- ✅ Native C++ performance
- ✅ Self-hosted with single binary deployment
- ✅ Advanced kernel-level security
- ✅ No vendor lock-in
- ✅ Minimal resource footprint

With proper execution, **native_node** can establish itself as the premier choice for high-performance, secure, self-hosted serverless computing.

---

**Document Version**: 1.0  
**Next Review**: April 2026  
**Maintained By**: Project maintainers
