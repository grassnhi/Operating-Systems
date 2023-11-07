int bktask_assign_worker(unsigned int bktaskid, unsigned int wrkid){
    if(wrkid < 0 || wrkid > MAX_WORKER){
        return -1;
    }

    struct bktask_t *tsk = bktask_get_byid(bktaskid);

    if(tsk == NULL){
        return -1;
    }

    /* Advertise I AM WORKING */
    wrkid_busy[wrkid] = 1;

    worker[wrkid].func = tsk->func;
    worker[wrkid].arg = tsk->arg;
    worker[wrkid].bktaskid = tsk->bktaskid;

    printf("Assign tsk %d wrk %d \n", tsk->bktaskid, wrkid);

    return 0;
}