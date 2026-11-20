# StyxKV

A lightweight, in-memory NoSQL key-value storage engine written in **C++23**, featuring Append-Only File (AOF) persistence and deterministic Unix epoch-based TTL expiration.

## Architecture & Key Features

* **Custom Storage & Data Types:** Built around a custom hash table supporting `String`, `List` (`LPUSH`, `RPUSH`, `LPOP`, `RPOP`), and `Set` (`SADD`, `SREM`) data structures alongside key management commands (`CREATE`, `SET`, `GET`, `DEL`, `EXISTS`, `TYPE`, `KEYS`).
* **Append-Only File (AOF) Persistence:** Logs state-mutating operations to disk (`styx.aof`) and reconstructs the in-memory state on startup via recovery replay.
* **Deterministic TTL Expiration:** Supports `EXPIRE`, `EXPIREAT`, and `TTL` with lazy deletion. Relative `EXPIRE` commands are normalized into absolute Unix epoch timestamps (`EXPIREAT`) prior to AOF logging to ensure consistent expiration across restarts.
* **Command Dispatcher:** Decoupled command parsing and execution pipeline built with the Command pattern.

## Project Structure

```text
styxKV/
├── include/styx/
│   ├── common/      # Core data types and variant definitions
│   ├── engine/      # CommandDispatcher and individual command implementations
│   ├── storage/     # HashTable and PersistenceManager (AOF)
│   └── utils/       # Overloaded visitor, string, and time utilities
├── src/             # Source implementations and CLI entry point
└── CMakeLists.txt
```
## Build & Run
 ``` bash
cmake -B build
cmake --build build
./bin/styxKV
```
## Supported Commands

| Category | Commands |
| :--- | :--- |
| **Key Management** | `CREATE`, `DEL`, `EXISTS`, `TYPE`, `KEYS` |
| **String** | `SET`, `GET` |
| **List** | `LPUSH`, `RPUSH`, `LPOP`, `RPOP` |
| **Set** | `SADD`, `SREM` |
| **Expiration (TTL)** | `EXPIRE`, `EXPIREAT`, `TTL` |

## Example Usage
``` text
styx> set user:1 baran
OK
styx> get user:1
"baran"
styx> expire user:1 60
(integer) 1
styx> ttl user:1
(integer) 58
styx> lpush tasks build test deploy
(integer) 3
styx> type tasks
list
```
## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
