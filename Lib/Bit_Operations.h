#ifndef BIT_OPERATIONS_H
#define BIT_OPERATIONS_H

#define SET_BIT(REG, BIT)     ((REG) |= (1U << (BIT)))
#define CLEAR_BIT(REG, BIT)   ((REG) &= ~(1U << (BIT)))
#define TOGGLE_BIT(REG, BIT)  ((REG) ^= (1U << (BIT)))
#define GET_BIT(REG, BIT)     (((REG) >> (BIT)) & 1U)

#endif // BIT_OPERATIONS_H