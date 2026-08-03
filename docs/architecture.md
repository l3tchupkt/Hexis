# Hexis Architecture

Hexis is built on a backend-first philosophy.

## Hexis V2 SDK
The core `libhexis` SDK provides the following primitives:
- **Event Bus**: Pub/Sub architecture for decoupling the GUI and Core.
- **Job System**: Asynchronous operations for long-running tasks.
- **Knowledge Graph**: Replaces flat array reporting with a directional graph.
- **VFS (Virtual Filesystem)**: Maps complex firmware partitions to a Unix-like structure.
- **Patch Engine**: Replaces destructive writes with transactional patches.
- **CAS & Project Manager**: Dual-format storage system (`.hexisproj` and `.hexis`) with deduplication.
- **Session Engine**: Wraps execution into reproducible sessions.
