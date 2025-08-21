# CS300
SNHU DSA: Analysis and Design


1. The problem I solved was to build an advising assistant that loads a course catalog from a CSV file and lets an advisor: (a) print all CS courses in alphanumeric order and (b) look up a single course to see its title and prerequisites. Earlier milestones required me to design algorithms and compare data structures; the final project implemented a full, menu-driven C++ program.

2. I approached it by writing clear pseudocode first (file read/parse/validate → build data structure → print functions), then choosing the structure that best matched the required operations. I compared vectors, hash tables, and BSTs using Big-O notation: vectors are simple but slow to search; hash tables are fast to look up but unordered; a BST is naturally ordered and still efficient on average. That analysis led me to implement a BST, so printing the full list is just an in-order traversal, and single-course lookups are quick.

3. The biggest roadblocks were input handling details: spaces in the file name, Xcode’s working directory, and messy lines in the CSV. I fixed these by switching to getline for filenames, adding a default file name, trimming tokens, uppercasing course numbers, doing a two-pass load so I could validate prerequisites, and adding robust path resolution that searches likely folders. I also printed clear warnings when a prerequisite wasn’t in the file, so data issues didn’t silently break the run.

4. This project expanded my design mindset to start from use cases and data first. I separated concerns: parsing and validation, domain model (Course), data structure (BST), and UI (menu). I now think in terms of “which operations must be fast?” and then pick or adapt the data structure to those operations instead of trying to force everything into one default container.

5. My code style improved toward maintainability and adaptability: small, single-purpose functions; consistent names; no magic numbers; clear error messages; and comments where intent isn’t obvious. I kept the interface between parts clean (e.g., a single parse function and print helpers), which makes it easy to swap the container (vector/hash/BST now, AVL later) without rewriting everything. I also added input validation and sensible defaults so the program behaves well in real environments, not just ideal ones.
