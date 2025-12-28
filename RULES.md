# Coding rules

1. `using namespace` is not allowed.
2. All code must use **inline braces**.
3. [Variables must have sensible names](https://www.youtube.com/watch?v=-J3wNP6u5YU), the only exception are `i` in a `for-loop` and `root/head/cur` pointers for `linked list/tree`.
4. [Don't comment too much](https://www.youtube.com/watch?v=Bf7vDBBOBUA), try to write codes that make sense instead.
5. Use [`#include` guard](https://en.wikipedia.org/wiki/Include_guard) for all header files.
6. A function should only do **one thing** and thus it shouldn't be too long, ideally less than **30** lines.

---

# Github rules

1. Most branches should be named like this: **feat/CSG-(task number)-(task-name-with-hyphen)**.
    - For example, the branch for the task **Create window** with the number **1** should be named **feat/CSG-1-create-window**.
    - The only exception are **hotfix/CSG- and refactor/CSG-** branches.
2. **Please, write sensible commit message.**
3. Please resolve all errors and warnings before committing.
4. Before merging the branch, a pull request for the branch must be opened and all changes in it must be approved **by at least 2 people**.
    - A pull request can only be opened when **the code is ready for review**.
    - It can have the same name as the branch.
    - It should have a description about **what have been done in the code** and **how to use the new feature** to make review easier.
    - It should be linked to the corresponding Jira task to make it easier to manage.

##### Note: More rules might be added later on.
