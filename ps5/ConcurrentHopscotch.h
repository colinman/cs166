#include<iostream>
#include<pthread.h>
using namespace std;

class Hopscotch {
private:
    static const int HOP_RANGE = 32;
    static const int ADD_RANGE = 256;
    static const int MAX_SEGMENTS = 1048576; // Including neighbourhodd for last hash location
    static const int MAX_TRIES = 2;
    int* BUSY;
    
    struct Bucket {
        
        unsigned int volatile _hop_info;
        int* volatile _key;
        int* volatile _data;
        unsigned int volatile _lock;
        unsigned int volatile _timestamp;
        pthread_mutex_t lock_mutex;
        pthread_cond_t lock_cv;
        
        Bucket(){
            _hop_info = 0;
            _lock = 0;
            _key = NULL;
            _data=NULL;
            _timestamp = 0;
            pthread_mutex_init(&lock_mutex,NULL);
            pthread_cond_init(&lock_cv, NULL);
        }
        
        void lock(){
            
            pthread_mutex_lock(&lock_mutex);
            while(1){
                if (_lock==0){
                    _lock =1;
                    pthread_mutex_unlock(&lock_mutex);
                    break;
                }
                pthread_cond_wait(&lock_cv, &lock_mutex);
                
            }
        }
        
        void unlock(){
            pthread_mutex_lock(&lock_mutex);
            _lock = 0;
            pthread_cond_signal(&lock_cv);
            pthread_mutex_unlock(&lock_mutex);
        }
        
    };
    
    Bucket* segments_arys;
    
public:
    Hopscotch();
    ~Hopscotch();
    bool contains(int* key);
    bool add(int *key, int *data);
    int* remove(int* key);
    void find_closer_bucket(Bucket**,int*,int &);
    
    
};
