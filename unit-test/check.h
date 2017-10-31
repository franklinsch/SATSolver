#ifndef CHECK_H
#define CHECK_H

#define CHECK(EXP) do { \
                          if (!(EXP)) { \
                            return 1; \
                          } \
                        } while(0)

#endif // !CHECK_H
