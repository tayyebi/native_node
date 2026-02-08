# native_node Development Roadmap

**Project Timeline**: February 2026 - January 2027 (12 months)  
**Current Phase**: Foundation (20% complete)  
**Target**: Production-Ready v1.0

---

## Milestones Overview

| Milestone | Timeline | Status | Completion |
|-----------|----------|--------|------------|
| M1: JIT Foundation | Month 1-2 | 🔴 Not Started | 0% |
| M2: Security Complete | Month 3-4 | 🔴 Not Started | 0% |
| M3: Core Services | Month 4-5 | 🟡 In Progress | 25% |
| M4: Web Framework | Month 6-7 | 🔴 Not Started | 0% |
| M5: Alpha Release | Month 8 | 🔴 Not Started | 0% |
| M6: Production Ops | Month 9-10 | 🔴 Not Started | 0% |
| M7: Beta Release | Month 11 | 🔴 Not Started | 0% |
| M8: v1.0 Release | Month 12 | 🔴 Not Started | 0% |

---

## M1: JIT Foundation (Month 1-2)

**Goal**: Functional JIT compilation and execution of C++ scripts

### Sprint 1.1: LLVM Integration (2 weeks)
- [ ] Add LLVM/Clang as CMake dependency (FetchContent or submodule)
- [ ] Configure minimal LLVM build (host target only, no tools)
- [ ] Verify ClangREPL availability in build
- [ ] Create CMake option `-DENGINE_JIT=ON` to enable JIT features
- [ ] Update CI to build with JIT support
- [ ] Documentation: LLVM build configuration guide

**Deliverables**:
- LLVM integrated into build system
- JIT build option working
- CI passing with JIT enabled

### Sprint 1.2: Basic JIT Session (2 weeks)
- [ ] Implement `JITSession` class for ClangREPL interaction
- [ ] Create preloaded header "universe" (iostream, string, vector)
- [ ] Implement code compilation method
- [ ] Add basic error handling for compilation failures
- [ ] Write unit tests for JIT compilation
- [ ] Benchmark cold start performance

**Deliverables**:
- Can compile simple C++ snippets
- Error messages for syntax errors
- Cold start measured (<200ms baseline)

### Sprint 1.3: Script Execution Context (2 weeks)
- [ ] Implement `ExecutionContext` for script isolation
- [ ] Add symbol export/import mechanism
- [ ] Implement script state management
- [ ] Create script entry point convention (doGet, doPost, main)
- [ ] Add execution timeout handling
- [ ] Write integration tests for script execution

**Deliverables**:
- Scripts can execute in isolated contexts
- Multiple scripts can run concurrently
- Execution limits enforced

### Sprint 1.4: JIT Optimization (1 week)
- [ ] Optimize preloaded headers for faster startup
- [ ] Implement incremental compilation
- [ ] Profile and optimize JIT session creation
- [ ] Add JIT cache for frequently used code
- [ ] Achieve <100ms cold start target

**Deliverables**:
- Cold start <100ms
- Performance benchmarks documented
- Optimization guide for script authors

**Exit Criteria**:
- ✅ Can compile and execute C++ scripts in <100ms
- ✅ Error handling works correctly
- ✅ Performance targets met
- ✅ 80%+ test coverage on JIT code

---

## M2: Security Complete (Month 3-4)

**Goal**: Production-grade security sandbox fully implemented

### Sprint 2.1: Landlock Enhancement (2 weeks)
- [ ] Extend RulesetBuilder with all Landlock access types
- [ ] Implement dynamic rule addition/removal
- [ ] Add per-script policy management
- [ ] Create policy validation and testing framework
- [ ] Write comprehensive Landlock tests
- [ ] Document Landlock configuration

**Deliverables**:
- Complete Landlock implementation
- Policy management API
- Test suite for filesystem isolation

### Sprint 2.2: Seccomp Hardening (1 week)
- [ ] Implement per-script seccomp profiles
- [ ] Add seccomp filter testing utilities
- [ ] Create default secure policy
- [ ] Add syscall whitelisting API
- [ ] Write seccomp unit tests
- [ ] Document seccomp configuration

**Deliverables**:
- Per-script seccomp policies
- Test suite for syscall filtering
- Secure default configuration

### Sprint 2.3: Cgroups Integration (2 weeks)
- [ ] Implement cgroups v2 wrapper
- [ ] Add CPU, memory, PID limit enforcement
- [ ] Create per-script cgroup management
- [ ] Implement cgroup cleanup on script exit
- [ ] Write cgroups tests
- [ ] Document resource limits

**Deliverables**:
- Full cgroups v2 support
- Resource limit enforcement
- Automatic cleanup

### Sprint 2.4: Security Testing & Audit (2 weeks)
- [ ] Set up fuzzing for sandbox code
- [ ] Run static analysis (clang-tidy, cppcheck)
- [ ] Perform manual security audit
- [ ] Create security test suite
- [ ] Fix identified vulnerabilities
- [ ] Write security whitepaper

**Deliverables**:
- Security audit report
- Zero critical vulnerabilities
- Security documentation
- Test suite for security features

**Exit Criteria**:
- ✅ All sandbox features implemented
- ✅ Security audit passed
- ✅ Penetration testing passed
- ✅ Documentation complete

---

## M3: Core Services (Month 4-5)

**Goal**: Essential services fully functional

### Sprint 3.1: SQLite Enhancement (1 week)
- [ ] Optimize connection pool for concurrent access
- [ ] Implement WAL mode and checkpoint management
- [ ] Add prepared statement caching
- [ ] Create query timeout handling
- [ ] Write performance tests
- [ ] Document SQLite best practices

**Deliverables**:
- Optimized SQLite pool
- Performance benchmarks
- Usage documentation

### Sprint 3.2: PropertyStore API (1 week)
- [ ] Design PropertyStore API (get/set/delete/list)
- [ ] Implement SQLite-backed storage
- [ ] Add per-script isolation
- [ ] Implement quota tracking
- [ ] Write unit tests
- [ ] Create usage examples

**Deliverables**:
- Complete PropertyStore implementation
- API documentation
- Example code

### Sprint 3.3: MailApp Implementation (3 weeks)
- [ ] Choose SMTP library (libcurl vs mailio)
- [ ] Implement basic email sending
- [ ] Add attachment support
- [ ] Implement HTML email support
- [ ] Add IMAP receiving (if needed)
- [ ] Create email quota system
- [ ] Write comprehensive tests
- [ ] Document email configuration

**Deliverables**:
- Functional MailApp service
- Quota enforcement
- Configuration guide

### Sprint 3.4: Quota System (1 week)
- [ ] Design quota tracking database schema
- [ ] Implement quota checking API
- [ ] Add per-script limits (CPU, memory, emails, DB size)
- [ ] Create quota enforcement in execution context
- [ ] Add quota reset scheduling
- [ ] Write tests for quota system
- [ ] Document quota configuration

**Deliverables**:
- Complete quota system
- Admin configuration interface
- Monitoring dashboard

**Exit Criteria**:
- ✅ All services functional
- ✅ Quota system enforced
- ✅ Test coverage >80%
- ✅ Documentation complete

---

## M4: Web Framework (Month 6-7)

**Goal**: Full web framework integration with request handling

### Sprint 4.1: Framework Selection & Integration (2 weeks)
- [ ] Finalize Drogon vs Oat++ decision
- [ ] Integrate chosen framework via CMake
- [ ] Configure asynchronous I/O
- [ ] Set up routing system
- [ ] Write basic HTTP server tests
- [ ] Document framework choice rationale

**Deliverables**:
- Framework integrated
- Basic HTTP server running
- Routing system working

### Sprint 4.2: Request Dispatcher (2 weeks)
- [ ] Implement request-to-script mapping
- [ ] Create Event object (request wrapper)
- [ ] Implement doGet/doPost handler convention
- [ ] Add parameter parsing (query, body, headers)
- [ ] Implement response handling
- [ ] Write integration tests
- [ ] Document handler API

**Deliverables**:
- Full request dispatch system
- Handler API documented
- Example web scripts

### Sprint 4.3: HtmlService (2 weeks)
- [ ] Design template syntax
- [ ] Implement template parser
- [ ] Add scriptlet evaluation (<? ?>)
- [ ] Implement template caching
- [ ] Add HTML escaping utilities
- [ ] Write template tests
- [ ] Create template examples

**Deliverables**:
- HtmlService implementation
- Template documentation
- Example templates

### Sprint 4.4: Session & Auth (1 week)
- [ ] Implement session management
- [ ] Add cookie handling
- [ ] Create basic authentication middleware
- [ ] Implement CSRF protection
- [ ] Write security tests
- [ ] Document authentication patterns

**Deliverables**:
- Session management
- Authentication framework
- Security best practices doc

**Exit Criteria**:
- ✅ Web framework fully functional
- ✅ Scripts can handle HTTP requests
- ✅ Templating works correctly
- ✅ Security features implemented

---

## M5: Alpha Release (Month 8)

**Goal**: Feature-complete alpha for early adopters

### Sprint 5.1: Integration & Polish (2 weeks)
- [ ] End-to-end testing of all features
- [ ] Performance optimization pass
- [ ] Bug fixing sprint
- [ ] Documentation review and updates
- [ ] Create getting started guide
- [ ] Build sample applications

**Deliverables**:
- Alpha release binary
- Complete documentation
- Sample applications

### Sprint 5.2: Alpha Launch (1 week)
- [ ] Prepare release notes
- [ ] Tag alpha release
- [ ] Publish binaries
- [ ] Announce to community
- [ ] Set up feedback channels
- [ ] Monitor initial usage

**Deliverables**:
- Public alpha release
- Release announcement
- Feedback collection system

**Exit Criteria**:
- ✅ All core features working
- ✅ Documentation complete
- ✅ No critical bugs
- ✅ Performance targets met

---

## M6: Production Operations (Month 9-10)

**Goal**: Production-ready observability and operations

### Sprint 6.1: Logging & Metrics (2 weeks)
- [ ] Integrate structured logging (spdlog)
- [ ] Implement metrics collection
- [ ] Add Prometheus endpoint
- [ ] Create log levels and filtering
- [ ] Implement distributed tracing
- [ ] Write operations guide

**Deliverables**:
- Complete logging system
- Metrics endpoint
- Operations documentation

### Sprint 6.2: Monitoring & Health (1 week)
- [ ] Implement health check endpoints
- [ ] Add readiness probes
- [ ] Create system metrics dashboard
- [ ] Implement alerting hooks
- [ ] Write monitoring guide

**Deliverables**:
- Health check system
- Monitoring integration
- Alert configuration

### Sprint 6.3: Testing Infrastructure (2 weeks)
- [ ] Expand unit test coverage to 85%+
- [ ] Create comprehensive integration tests
- [ ] Implement performance regression tests
- [ ] Set up load testing framework
- [ ] Add chaos testing
- [ ] Document testing practices

**Deliverables**:
- 85%+ test coverage
- Load testing suite
- Testing documentation

### Sprint 6.4: Deployment & Distribution (2 weeks)
- [ ] Optimize static binary build
- [ ] Reduce binary size to <40MB
- [ ] Create cross-compilation setup
- [ ] Build container images
- [ ] Create Kubernetes manifests
- [ ] Write deployment guide

**Deliverables**:
- Optimized binary
- Container images
- Deployment manifests

**Exit Criteria**:
- ✅ Production-grade observability
- ✅ Test coverage >85%
- ✅ Deployment automation complete
- ✅ Operations documented

---

## M7: Beta Release (Month 11)

**Goal**: Production validation with beta users

### Sprint 7.1: Beta Preparation (2 weeks)
- [ ] Security audit round 2
- [ ] Performance optimization
- [ ] Bug fixing sprint
- [ ] Documentation polish
- [ ] Create migration guides
- [ ] Build enterprise features

**Deliverables**:
- Beta release candidate
- Security audit report
- Migration documentation

### Sprint 7.2: Beta Program (2 weeks)
- [ ] Recruit beta testers
- [ ] Deploy to beta environments
- [ ] Collect feedback
- [ ] Fix beta issues
- [ ] Update documentation
- [ ] Prepare for 1.0

**Deliverables**:
- Beta feedback report
- Bug fixes
- Updated documentation

**Exit Criteria**:
- ✅ Beta testing successful
- ✅ No critical issues
- ✅ Performance validated
- ✅ Ready for production

---

## M8: v1.0 Release (Month 12)

**Goal**: Public production release

### Sprint 8.1: Release Preparation (2 weeks)
- [ ] Final security review
- [ ] Performance validation
- [ ] Documentation finalization
- [ ] Release notes preparation
- [ ] Marketing materials
- [ ] Community setup

**Deliverables**:
- Release candidate
- Complete documentation
- Marketing materials

### Sprint 8.2: Launch (1 week)
- [ ] Tag v1.0 release
- [ ] Publish binaries and images
- [ ] Announce launch
- [ ] Monitor initial adoption
- [ ] Provide support
- [ ] Celebrate! 🎉

**Deliverables**:
- v1.0 release
- Public announcement
- Support channels active

**Exit Criteria**:
- ✅ Production-ready v1.0
- ✅ All features complete
- ✅ Documentation comprehensive
- ✅ Community active

---

## Post-v1.0 Roadmap (Future)

### v1.1: Enhanced Features (Q1 2027)
- [ ] Triggers and scheduling system (libcron)
- [ ] Library versioning system
- [ ] Advanced admin console features
- [ ] WebSocket support
- [ ] GraphQL support

### v1.2: Enterprise Features (Q2 2027)
- [ ] Multi-tenancy support
- [ ] RBAC implementation
- [ ] Audit logging
- [ ] Compliance features
- [ ] High availability clustering

### v2.0: Advanced Capabilities (Q3 2027)
- [ ] Multiple language support (Python, JavaScript)
- [ ] Distributed execution
- [ ] Advanced caching
- [ ] Machine learning integration
- [ ] Serverless edge deployment

---

## Resource Allocation

### Team Composition
- **Core Developers (2-3)**: JIT, runtime, services
- **Security Engineer (1)**: Sandbox, auditing
- **DevOps Engineer (1)**: CI/CD, deployment
- **Technical Writer (0.5)**: Documentation
- **UI Developer (0.5)**: Admin console

### Key Dependencies
- LLVM/Clang 16+
- Linux kernel 5.13+ (Landlock)
- SQLite 3.x
- libseccomp
- Web framework (Drogon/Oat++)
- Docker/Kubernetes

---

## Risk Management

### High-Priority Risks
1. **JIT Complexity**: Early prototyping and validation
2. **Security Vulnerabilities**: Continuous auditing
3. **Performance Issues**: Regular benchmarking
4. **Timeline Slippage**: Buffer time in schedule

### Mitigation Strategies
- Weekly progress reviews
- Continuous integration and testing
- Regular security audits
- Community involvement for feedback

---

## Success Metrics

### Technical Metrics
- Binary size: <40MB ✅
- Cold start: <100ms ✅
- Test coverage: >85% ✅
- Memory: <100MB base ✅
- Uptime: 99.9% ✅

### Adoption Metrics
- GitHub stars: 1,000+ by end of year
- Production deployments: 100+
- Contributors: 50+
- Libraries published: 10+

---

**Document Version**: 1.0  
**Last Updated**: February 8, 2026  
**Next Review**: March 1, 2026
