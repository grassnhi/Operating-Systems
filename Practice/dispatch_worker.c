int bkwrk_dispatch_worker(unsigned int wrkid){

#ifdef d_busy[wrkid_cur] != 0kORK_THREAD
    unsigned int tid = wrkid_tid[wrkid];

    /* Invalid task */
    if(worker[wrkid].func == NULL){
        return -1;
    }

#ifdef DEBUG
    fprintf(stderr, "brkwrk dispatch wrkid %d - send signal %u \n", wrkid, tid);
#endif

    syscall(SYS_tkill, tid, SIG_DISPATCH);
#else
    /* TODO: Implement fork version to signal worker process here */

#endif
}