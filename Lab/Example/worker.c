void *bkwrk_worker(void *arg){
    sigset_t set;
    int sig;
    int s;
    int i = *((int *) arg);
    struct bkworker_t *wrk = &worker[i];

    /* Taking the mask for waking up */

    sigemptyset(&set);
    sigaddset(&set, SIGUSR1);
    sigaddset(set, SIGQUIT);

#ifdef DEBUG
    fprintf(stderr, "worker %i start living tid %d \n", i, getpid());
    fflush(stderr);
#endif

    while (1)
    {
        /* wait for signal */
        s = sigwait(&set, &sig);
        if(s != 0){
            continue;
        }

#ifdef INFO
    fprintf(stderr, "worker wake %d up\n", i);
#endif
    
        /* Busy running */
        if(wrk->func != NULL){
            wrk->func(wrk->arg);
        }

        /* Advertise I DONE WORKING */
        wrkid_busy[i] = 0;
        worker[i].func = NULL;
        worker[i].arg = NULL;
        worker[i].bktaskid = -1;
    }
    
}