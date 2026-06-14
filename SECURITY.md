# Security Policy

## Supported Versions

| Version | Supported |
|---------|-----------|
| Latest stable release | ✅ Active |
| Previous stable release | ⚠️ Critical fixes only |
| Dev branch | ❌ No guarantees |

---

## Reporting a Vulnerability

If you discover a security vulnerability in Volt Emulator, please **do not** open a public GitHub issue.

### Preferred Reporting Channel

Email: **security@volt-emu.dev**

Include in your report:
- A clear description of the vulnerability
- Steps to reproduce
- Potential impact assessment
- Suggested fix (if you have one)

### Response Timeline

| Stage | Timeline |
|-------|----------|
| Acknowledgement | Within 48 hours |
| Initial assessment | Within 1 week |
| Fix development | Depends on severity |
| Coordinated disclosure | After fix is released |

We will keep you informed throughout the process.

---

## Scope

### In Scope

- Memory safety issues (buffer overflows, use-after-free, etc.)
- Code execution vulnerabilities triggered by malformed game files
- Sandbox escapes (if a sandboxing model is implemented)
- Cryptographic implementation errors
- Path traversal issues in save/load operations

### Out of Scope

- Issues requiring physical access to the host machine
- Social engineering attacks
- Vulnerabilities in third-party dependencies (report to upstream)
- Nintendo Switch piracy enablement (this is a separate policy matter, not a security vulnerability)

---

## Philosophy

Volt Emulator processes untrusted data — game files may be malformed, corrupted, or intentionally crafted to exploit parsing code. We take memory safety seriously and prefer:

- Bounds-checked access patterns
- Safe integer arithmetic (no undefined overflow)
- RAII resource management
- AddressSanitizer in CI for detection of memory errors

---

## Known Limitations

Volt Emulator is an emulator, not a security sandbox. Running games from untrusted sources carries inherent risk as the emulator parses complex binary formats. We mitigate this through safe coding practices but do not guarantee isolation from malicious inputs.
