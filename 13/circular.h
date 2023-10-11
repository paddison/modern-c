#ifndef CIRCULAR_H
#define CIRCULAR_H 1
#include <stdlib.h>

/**
 * @brief an opaque type for a circular buffer for @b double values
 *
 * This data structure allows to add @b double values in rear and to take 
 * them out in front.
 * Each such structure has a maximal amount of elements that can be stored in 
 * it.
 */
typedef struct circular circular;

/** @brief the hidden implementation of the circular buffer type */
struct circular {
    size_t start;   /**< Position of element 0 */
    size_t len;     /**< Number of elements stored */
    size_t max_len; /**< Maximum capacity */
    double* tab;    /**< Array holding the data */
};

/** 
 * @brief Append a new element with value @value to the buffer @c c. 
 * @returns c if the new element could be appended, 0 otherwise.
 */
circular* circular_append(circular* c, double value);

/**
 * @brief Remove the oldest element from @c c and return its value.
 *
 * @returns the removed element if it exists, 0.0 otherwise.
 */
double circular_pop(circular* c);

/**
 * @brief Return a pointer to position @pos pos in buffer @c c.
 *
 * @returns a pointer to the @pos pos' element of the buffer, 0 otherwise.
 */
double* circular_element(circular* c, size_t pos);

/**
 * @brief Initialize a circular buffer @c c with maximally @max_len max_len
 * elements.
 *
 * Each buffer that is initialized with this function must be destroyed with 
 * a call to circular_destroy.
 */
circular* circular_init(circular* c, size_t max_len);

/**
 * @brief Destroy circular buffer @c c.
 *
 * @c c must have been initialized with a call to circular_init
 */
void circular_destroy(circular* c);

/**
 * @brief Allocate and initialize a circular buffer with maximally @len len
 * elements.
 *
 * Each buffer that is allocated with this function must be deleted with a call
 * to circular_delete.
 */
circular* circular_new(size_t len);

/**
 * @brief Delete a circular buffer @c c.
 *
 * @c c must have been allocated with a call to circular_new
 */
void circular_delete(circular* c);

/**
 * @brief Resize to capacity @max_len max_len.
 */
circular* circular_resize(circular* c, size_t max_len);

/**
 * @brief Return the number of elements stored.
 */
size_t circular_getlength(circular* c);

#endif

