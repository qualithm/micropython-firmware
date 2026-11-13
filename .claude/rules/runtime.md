---
description: "Runtime conventions for errors, logs, settings and CLIs"
paths:
  - "modules/**"
  - "cmodules/**"
  - "boards/**"
  - "tests/**"
  - "test/**"
---

# Runtime Conventions

One set of rules for errors, logs, settings and CLIs holds in every language, so logs and errors
from different repos read and search together. This is how they look here.

## Errors

- Python raises a built-in exception or one base exception per module (`<Component>Error`) with a
  subclass per failure. Messages are lowercase, with no trailing punctuation, and name what failed
  (`claim code expired`).
- C logic returns `0` or a negative code its header documents; the binding turns each into a
  MicroPython exception with the same kind of message.

## Logging

- Frozen modules don't print or log; a board has no `logging`. A line written for an operator is a
  `snake_case` event, then `key=value` fields with `snake_case` keys, never a sentence with values
  interpolated.
- Never log a request or response body, a query value, a credential or key material.

## Settings

- Settings are a `<Component>Config` class with production defaults.
- Units go in names: `_ms` for durations and `_bytes` for sizes (`timeout_ms`, `buffer_bytes`).
