#define is_reading_b 0
#define is_reading_c 1
#define is_free -1

// Supponiamo che l'interruzione c sia più prioritaria dell'interruzione b.
// Serve l'accesso in mutua esclusione perchè non vogliamo sovrapposizioni tra i messaggi. 

isr_b(){
    if(!fine){
        if(TAS(lock)){
            if(possesso !=c and !end_b){
                possesso = is_reading_b;
            }
            lock = 0;
        }else{
            RTE;
        }

        switch (possesso){
            case is_reading_b:
                buff[tot] = PIABPRA;
                if(curr == 0 && fase2 && buff[tot] == 0){
                    cond_b = 1;
                }
                tot++;
                curr++;
                if(curr == N){
                    curr = 0;
                    msg++;
                    end_b = 1;
                    possesso = is_free;
                    if(end_c){
                        end_b = 0;
                        end_c = 0;
                        if(msg==k){
                            fase2=1;
                        }
                    }
                    if(fase2 && (cond_b && cond_c)){
                        fine = 1;
                    }
                    if(c_sus && !end_c){
                        tot++;
                        curr++;
                        buff[tot-1] = PIACPRA;
                        if(buff[tot-1] == 0){
                            cond_c=1;
                        }
                        possesso = is_reading_c;
                    }
                }
             case is_free:
                if(c_sus && !end_c){
                    tot++;
                    curr++;
                    buff[tot-1] = PIACPRA;
                    if(buff[tot-1] == 0){
                        cond_c=1;
                    }
                    possesso = is_reading_c;
                }
            case is_reading_c{
                if(c_sus){
                    idx = tot;
                    tot++;
                    curr++;
                    if(curr == N){
                        curr = 0;
                        msg++;
                        end_c = 1;
                        possesso = is_free;
                        if (end_b){
                            end_b = 0;
                            end_c = 0;
                            msg++;
                            if(msg == k){
                                fase2=1;
                            }
                        }
                        if(fase2 && (cond_b && cond_c)){
                            fine = 1;
                        }
                        if(!end_b){
                            buff[tot]=PIABPRA;
                            tot++;
                            curr++;
                            if(fase2 && buff[tot-1]==0){
                                cond_b = 1;
                            }
                            possesso = is_reading_b;
                        }
                    }
                    possesso = is_reading_c;
                }
            }
        }   
        RTE;
    }else{
        RTE;
    }
}