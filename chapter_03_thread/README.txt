1. 互斥量相关操作函数
◼ 互斥量的类型 pthread_mutex_t
◼ int pthread_mutex_init(pthread_mutex_t *restrict mutex, const pthread_mutexattr_t *restrict attr);
◼ int pthread_mutex_destroy(pthread_mutex_t *mutex);
◼ int pthread_mutex_lock(pthread_mutex_t *mutex);
◼ int pthread_mutex_trylock(pthread_mutex_t *mutex);
◼ int pthread_mutex_unlock(pthread_mutex_t *mutex);


2. 读写锁相关操作函数
◼ 读写锁的类型 pthread_rwlock_t
◼ int pthread_rwlock_init(pthread_rwlock_t *restrict rwlock, const pthread_rwlockattr_t *restrict attr);
◼ int pthread_rwlock_destroy(pthread_rwlock_t *rwlock);
◼ int pthread_rwlock_rdlock(pthread_rwlock_t *rwlock);
◼ int pthread_rwlock_tryrdlock(pthread_rwlock_t *rwlock);
◼ int pthread_rwlock_wrlock(pthread_rwlock_t *rwlock);
◼ int pthread_rwlock_trywrlock(pthread_rwlock_t *rwlock);
◼ int pthread_rwlock_unlock(pthread_rwlock_t *rwlock);
◼ 读写锁的特点：
    -> 如果有其它线程读数据，则允许其它线程执行读操作，但不允许写操作。
    -> 如果有其它线程写数据，则其它线程都不允许读、写操作。
    -> 写是独占的，写的优先级高。


3. 条件变量相关函数
◼ 条件变量的类型 pthread_cond_t
◼ int pthread_cond_init(pthread_cond_t *restrict cond, const pthread_condattr_t *restrict attr);
◼ int pthread_cond_destroy(pthread_cond_t *cond);
◼ int pthread_cond_wait(pthread_cond_t *restrict cond, pthread_mutex_t *restrict mutex);
◼ int pthread_cond_timedwait(pthread_cond_t *restrict cond, 
                             pthread_mutex_t *restrict mutex, 
                             const struct timespec *restrict abstime);
◼ int pthread_cond_signal(pthread_cond_t *cond);
◼ int pthread_cond_broadcast(pthread_cond_t *cond);


4. 信号量相关函数
◼ 信号量的类型 sem_t
◼ int sem_init(sem_t *sem, int pshared, unsigned int value);
◼ int sem_destroy(sem_t *sem);
◼ int sem_wait(sem_t *sem);
◼ int sem_trywait(sem_t *sem);
◼ int sem_timedwait(sem_t *sem, const struct timespec *abs_timeout);
◼ int sem_post(sem_t *sem);
◼ int sem_getvalue(sem_t *sem, int *sval);
