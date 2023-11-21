#ifndef PTHREAD_H
#include <pthread.h>
#endif

typedef struct pthread_seqlock { /* TODO: implement the structure */
   int sequence;
   pthread_mutex_t lock;
} pthread_seqlock_t;

static inline void pthread_seqlock_init(pthread_seqlock_t *rw)
{
   /* TODO: ... */
   rw->sequence = 0;
   pthread_mutex_init(&rw->lock, NULL);
}

static inline void pthread_seqlock_wrlock(pthread_seqlock_t *rw)
{
   /* TODO: ... */
   pthread_mutex_lock(&rw->lock);
   rw->sequence++;
}

static inline void pthread_seqlock_wrunlock(pthread_seqlock_t *rw)
{
   /* TODO: ... */
   rw->sequence--;
   pthread_mutex_unlock(&rw->lock);
}

static inline unsigned pthread_seqlock_rdlock(pthread_seqlock_t *rw)
{
   /* TODO: ... */
   if(rw->sequence%2==0) {
	pthread_mutex_lock(&rw->lock);
	return 1;
   }
   return 0;
}


static inline unsigned pthread_seqlock_rdunlock(pthread_seqlock_t *rw)
{
   /* TODO: ... */
   pthread_mutex_unlock(&rw->lock);
   return 0;
}