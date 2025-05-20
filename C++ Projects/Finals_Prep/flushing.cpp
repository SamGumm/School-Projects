#include <iostream>
/**
 * Using `"\n"` versus `std::endl` comes down to one key difference:

1. **Flushing the stream**

   * `std::endl` inserts a newline **and** flushes the output buffer (i.e. it calls `os.flush()`).
   * `"\n"` just writes a newline character; it does **not** force a flush.

2. **Performance implications**

   * Flushing is relatively expensive: it may cause a system call every time you do it.
   * In tight loops (e.g. logging inside `for` or real-time output), repeatedly using `std::endl` can drastically slow you down.
   * Using `"\n"` lets the library buffer data in larger chunks and flush less often (usually when the buffer is full or on program exit).

3. **When you **do** want a flush**

   * Interactive programs (e.g. a prompt that waits for user input) often need to flush so the user actually sees the prompt.
   * In that case, either use `std::flush` explicitly, or `std::endl` once:

     ```cpp
     std::cout << "Enter command: " << std::flush;
     // … or …
     std::cout << "Enter command: " << std::endl;
     ```
   * For normal “write and move on” lines, prefer `"\n"`.

4. **Best practice**

   * **Default** to `"\n"` for line breaks.
   * Only use `std::endl` (or `std::flush`) when you have a specific need to flush immediately.

---

In almost all cases where you’re just emitting text, `'\n'` is the lighter-weight, higher-performance choice.

 */

int main() {
    // Slower: flush on every iteration
    for (int i = 0; i < 100000; ++i) {
        std::cout << i << std::endl;  
    }

    // Faster: buffered, flush only when needed
    for (int i = 0; i < 100000; ++i) {
        std::cout << i << '\n';
    }
    std::cout << std::flush;  // flush once at the end
}